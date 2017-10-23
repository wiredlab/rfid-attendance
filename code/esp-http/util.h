
/*
 * Configuration for the SparkFun WiFi Shield - ESP8266
 */

 #define ESP8266_LED 5  // attached to GPIO 5


/*
 * Call this in setup() to get the utilities ready to use.
 */
void board_init() {
  pinMode(ESP8266_LED, OUTPUT);
  digitalWrite(ESP8266_LED, HIGH);
}


/*
 * Convenience function to flash a pattern
 * On for ontime ms,
 * Off for offtime ms,
 * repeated n times.
 */
void blink(int ontime, int offtime, int repeat) {
  int n = repeat;
  while (n > 0) {
    digitalWrite(ESP8266_LED, LOW);
    delay(ontime);
    digitalWrite(ESP8266_LED, HIGH);
    delay(offtime);
    n--;
  }
}


/*
 * Make the blinkiness to have well-defined meanings.
 * Then use status() to easily blink something the user
 * understands.
 */
enum status_t {
  BLIP,
  WAITING,
  GOOD,
  ERROR,
};

void status(status_t s) {
  switch (s) {
    case BLIP:
      blink(20, 0, 1);
    case WAITING:
      blink(500, 100, 1);
      break;
    case GOOD:
      blink(100, 100, 2);
      break;
    case ERROR:
      blink(100, 100, 5);
    default:
      break;
  }
}




/*
 *
 * Functions for RFID reader
 *
 */
//only when using the library
#ifdef TMR_SR_OPCODE_VERSION
// Gracefully handles a reader that is already configured and already reading
// continuously Because Stream does not have a .begin() we have to do this
// outside the library
boolean setupNano(RFID &nano, Stream &uart)
{
  nano.begin(uart); //Tell the library to communicate over software serial port

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while(uart.available()) uart.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE) {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();
    Serial.println(F("Module continuously reading. Asking it to stop..."));
    delay(1500);
  }

  //Test the connection
  nano.getVersion();
  Serial.print("connection test response: ");
  Serial.println(nano.msg[0], HEX);
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2
  nano.setAntennaPort(); //Set TX/RX antenna ports to 1
  nano.setRegion(REGION_NORTHAMERICA); //Set to North America

  return (true); //We are ready to rock
}
#endif
