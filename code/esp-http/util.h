
/*
 * Configuration for the SparkFun WiFi Shield - ESP8266
 */

 #define ESP8266_LED 5  // attached to GPIO 5


/*
 * Call this in setup() to get the utilities ready to use.
 */
void board_init() {
  pinMode(ESP8266_LED, OUTPUT);
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
 * Indicate a status with an LED                                                
 */                                                                             
enum status_t {                                                                 
  WAITING,                                                                      
  GOOD,                                                                         
  ERROR,                                                                        
};  


void status(status_t s) {                                                       
  switch (s) {                                                                  
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

