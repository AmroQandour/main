#include <SoftwareSerial.h>
#include <XBee.h>
#include <Streaming.h>



SoftwareSerial mySerial(2, 3);
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


void setup() {
 
  
  // start serial
  xbee.begin(9600);
  pinMode(8, OUTPUT);
 // pinMode(10, INPUT); //PWM
 // pinMode(6, INPUT); //PWM
  digitalWrite(8, HIGH);
  mySerial.begin(9600);
  
  

  
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
  /*
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=10) { 
    // sets the value (range from 0 to 255):
    analogWrite(10, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=10) { 
    // sets the value (range from 0 to 255):
    analogWrite(6, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } */
  
   if(mySerial.available()){
     
     Serial << _BYTE((mySerial.read()));
   }
  
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
           
        for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));
                    }
      }
    }
    
} 
         
