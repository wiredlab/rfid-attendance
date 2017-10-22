// TODO: document extra libraries to be installed via IDE
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define ESP8266_LED 5
#include "util.h"


// The MAC address of the specific module must be registered
// with the Valpo WiFi network.  It is good for 11 months.
// Go to vu-wifi.valpo.edu and select "MAC Self Registration"
//
// This step should not be necessary for a GE100 student to do.
const char ssid[] = "VU-Media";
const char password[] = "";  // unused for unencrypted connection


WiFiClient client;
HTTPClient http;


void setup() {
  pinMode(ESP8266_LED, OUTPUT);

  Serial.begin(115200);
  delay(500);
 
  Serial.println();

  // Setup client-only mode, "station"
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin(ssid);  // for unencrypted SSID
  // WiFi.begin(ssid, password);  // encrypted SSIDs

  while (WiFi.status() != WL_CONNECTED) {
    status(WAITING);
    Serial.println(WiFi.status());
  }
  status(GOOD);

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 

void loop() {
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

// vim: ts=2 sw=2
