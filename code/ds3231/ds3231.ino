// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"


// Pinout of the RTC modules:
// from top (chip side) pins on top
//   1 - Vdd
//   2 - SDA
//   3 - SCL
//   4 - no-connect
//   5 - GND
// power the RTC from a GPIO pin
#define RTC_PWR_PIN 16  // AKA "XPO" to module's Vdd
#define SDA_PIN 2  // on ESP8266
#define SCL_PIN 4


// interval in ms to print the current time
static unsigned long interval = 2000;



RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long lastMillis = 0;



void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(115200);

  // power the RTC from a GPIO
  pinMode(RTC_PWR_PIN, OUTPUT);
  digitalWrite(RTC_PWR_PIN, HIGH);
  // setup the I2C pins for the RTC
  Wire.begin(SDA_PIN, SCL_PIN);  // SDA, SCL

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  Serial.println();
  Serial.println("Set clock by sending a line of 6 integers:");
  Serial.println("yyyy mm dd hh mm ss");
}


void loop () {
  if (Serial.available()) {
    // command line format:
    //   yyyy mm dd hh mm ss
    int year = Serial.parseInt();
    int month = Serial.parseInt();
    int day = Serial.parseInt();
    int hour = Serial.parseInt();
    int minute = Serial.parseInt();
    int second = Serial.parseInt();

    // get here as quickly as possible
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    Serial.print("Setting time to: ");
    printDateTime(year, month, day, hour, minute, second);

    // now consume the rest of the input line
    Serial.readString();
  }

  // no delays in the loop so there is little delay between sending the time
  // string and setting the RTC time
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - lastMillis) >= interval) {
    lastMillis = currentMillis;
    DateTime now = rtc.now();
    printDateTime(now.year(), now.month(), now.day(),
                  now.hour(), now.minute(), now.second());
  }
}


/*
 * Utility function to print a datetime in RFC3339 format
 * https://tools.ietf.org/html/rfc3339#page-6
 */
char *printDateTime(int year, int month, int day,
                    int hour, int minute, int second)
{
  char buf[32];
  sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02dZ",
          year, month, day, hour, minute, second);
  Serial.println(buf);
}

