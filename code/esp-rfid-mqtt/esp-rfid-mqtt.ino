
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // MQTT library
#include <SoftwareSerial.h> // communication between ESP and RFID reader
#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
#include "util.h"


// Ensure the ESP shield 5V pin is NOT connected to the RFID shield.
//   Clip off the pin is an easy but "permanent" solution.
//   This should already have been done.

// Use a male-female jumper wire to connect the ESP's 5V pin to the RFID shield's
//   external power input (+) pin.

// jumper wires from:
//   GPIO 13 to shield 2
//   GPIO 15 to shield 3
// select "SW-UART" on RFID board
// move UART switch to "HW" on the ESP board


// For the buzzer, connect these pins between the ESP and RFID reader:
//   GPIO 12 to shield 9
//   GPIO 14 to shield 10
#define BUZZER1 12
#define BUZZER2 14


const char ssid[] = "VU-Media";
const char password[] = "";  // unused for unencrypted connection

const char mqtt_server[] = "mqtt.agnd.net";
const int mqtt_port = 1883;
const char mqtt_user[] = "USER";
const char mqtt_pass[] = "PASS";



// Create instances of the serial driver and the RFID driver
SoftwareSerial softSerial(13, 15); //RX, TX (from ESP's view)
RFID nano;


// instances of the WiFi and network drivers
WiFiClient netClient;
PubSubClient client(netClient);


byte mac[6];
String macString = "";
String clientId = "ESP8266-RFID-";

  
void setup()
{
  board_init();  // for the functions in util.h

  Serial.begin(115200);  // Serial monitor must match this rate

  // Setup the pins for the buzzer
  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);
  digitalWrite(BUZZER2, LOW); //Pull half the buzzer to ground and drive the other half.

  while (!Serial);  //wait until open

  // now get the RFID reader setup
  Serial.println();
  Serial.println("Initializing RFID reader...");

  // nano defaults to 115200 baud rate, keep it the same!
  softSerial.begin(115200);
  while (!softSerial);  // wait until open

  if (setupNano(nano, softSerial) == false)
  {
    Serial.println("Module failed to initialize. Check wiring?");
    status(ERROR);
    delay(1000);
    //Serial.println("Restarting...");
    //ESP.restart();
    //while (1); //Freeze!
  }

  nano.setReadPower(500); //5.00 dBm. Higher values may cause USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  // Uncomment this to see the commands and responses sent/received
  //   between the ESP and Reader
  //nano.enableDebugging(Serial);
  status(GOOD);

  /////////////////////////////////////////////////
  // WiFi setup
  /////////////////////////////////////////////////
  
  // Setup client-only mode, "station"
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  //WiFi.begin(ssid);  // for unencrypted SSID
  WiFi.begin(ssid, password);  // encrypted SSIDs

  while (WiFi.status() != WL_CONNECTED) {
    status(WAITING);
    Serial.println(WiFi.status());
  }

  Serial.println("");
  WiFi.printDiag(Serial);
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // read our MAC address and create a unique client ID
  WiFi.macAddress(mac);

  for (int i=0; i<6; i++) {
    if (mac[i] < 0x10) {
      macString += "0";
    }
    macString += String(mac[i], HEX);
  }

  clientId += macString;
  
  Serial.println("");
  Serial.print("MAC: ");
  Serial.println(macString);


  /////////////////////////////////////////////////
  // setup the MQTT connection
  /////////////////////////////////////////////////
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  
  status(GOOD);  // done with setup
}



/////////////////////////////////////////////////
// maintain a connection to the MQTT server
//
// publish a message to "clientId/status" topic when we connect
// this is also where we subscribe to channels
/////////////////////////////////////////////////
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.println(clientId);
    
    // Attempt to connect
    // client.connect(clientId, username, password)
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      if (client.publish(String(clientId + "/status").c_str(), "hello world!")) {
        Serial.println("published!");
      }
      // ... and resubscribe
      if (client.subscribe(String(clientId + "/control").c_str())) {
        Serial.println("subscribed!");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



/////////////////////////////////////////////////
// called when a mesage is posted to a topic
// we are subscribed to
/////////////////////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}







void loop()
{

  /////////////////////////////////////////////////
  // make sure we are connected to MQTT server
  /////////////////////////////////////////////////
  if (!client.connected()) {
    reconnect();
  }
  /////////////////////////////////////////////////
  // this must be called periodically in order to
  // maintain the MQTT connection
  // (unless you are only connecting once and sleeping...
  /////////////////////////////////////////////////
  client.loop();


  byte myEPC[12]; //Most EPCs are 12 bytes
  byte myEPClength;
  byte responseType = 0;

  myEPClength = sizeof(myEPC); //Length of EPC is modified each time .readTagEPC is called
  Serial.println(F("Searching for tag"));
  responseType = nano.readTagEPC(myEPC, myEPClength, 500); //Scan for a new tag up to 500ms

  if (responseType == RESPONSE_SUCCESS) {
    //Print EPC
    Serial.print(F(" epc["));
    String epcString = "";
    
    for (byte x = 0 ; x < myEPClength ; x++)
    {
      if (myEPC[x] < 0x10) {
        epcString += "0";
      }
      epcString += String(myEPC[x], HEX);
    }
    Serial.print(epcString);
    Serial.println(F("]"));

    /////////////////////////////////////////////////
    // this is how to publish a message to a topic
    //   client.publish(topic, data);
    if (client.publish(String(clientId + "/epc").c_str(), epcString.c_str())) {
      //Beep! Piano keys to frequencies: http://www.sengpielaudio.com/KeyboardAndFrequencies.gif
      tone(BUZZER1, 2093, 150); //C
      delay(150);
      tone(BUZZER1, 2349, 150); //D
      delay(150);
      tone(BUZZER1, 2637, 150); //E
      delay(150);
    }
  }

  status(GOOD);

  //delay(2000);
}
