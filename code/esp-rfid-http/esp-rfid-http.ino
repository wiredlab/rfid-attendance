
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
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


// Create instances of the serial driver and the RFID driver
SoftwareSerial softSerial(13, 15); //RX, TX (from ESP's view)
RFID nano;

// instances of the WiFi and network drivers
WiFiClient client;
HTTPClient http;


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
    Serial.println("Restarting...");
    ESP.restart();
    while (1); //Freeze!
  }

  nano.setReadPower(500); //5.00 dBm. Higher values may cause USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

  // Uncomment this to see the commands and responses sent/received
  //   between the ESP and Reader
  //nano.enableDebugging(Serial);
  status(GOOD);

  
  // Setup client-only mode, "station"
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin(ssid);  // for unencrypted SSID
  //WiFi.begin(ssid, password);  // encrypted SSIDs

  while (WiFi.status() != WL_CONNECTED) {
    status(WAITING);
    Serial.println(WiFi.status());
  }

  Serial.println("");
  WiFi.printDiag(Serial);
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  status(GOOD);  // done with setup
}


void loop()
{
  Serial.println(F("Press a key to scan for a tag"));
  while (!Serial.available()); //Wait for user to send a character
  Serial.read(); //Throw away the user's character

  byte myEPC[12]; //Most EPCs are 12 bytes
  byte myEPClength;
  byte responseType = 0;

  while (responseType != RESPONSE_SUCCESS)//RESPONSE_IS_TAGFOUND)
  {
    myEPClength = sizeof(myEPC); //Length of EPC is modified each time .readTagEPC is called

    responseType = nano.readTagEPC(myEPC, myEPClength, 500); //Scan for a new tag up to 500ms
    Serial.println(F("Searching for tag"));
  }

  //Beep! Piano keys to frequencies: http://www.sengpielaudio.com/KeyboardAndFrequencies.gif
  tone(BUZZER1, 2093, 150); //C
  delay(150);
  tone(BUZZER1, 2349, 150); //D
  delay(150);
  tone(BUZZER1, 2637, 150); //E
  delay(150);

  //Print EPC
  Serial.print(F(" epc["));
  for (byte x = 0 ; x < myEPClength ; x++)
  {
    if (myEPC[x] < 0x10) Serial.print(F("0"));
    Serial.print(myEPC[x], HEX);
    Serial.print(F(" "));
  }
  Serial.println(F("]"));



  http.begin("http://whiteaudio.com/index.html");
  int httpCode = http.GET();
  String response = http.getString();
  http.end();

  Serial.print("HTTP response code: ");
  Serial.println(httpCode);
  Serial.println(response);

  status(GOOD);

  delay(2000);
}


