
#include <XBee.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

uint16_t result = 0; 

void setup() {
 
  
  // start serial
  xbee.begin(9600);
  
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
          
        result = (rx.getData(0) << 8) & 0xff00; 
        result |=(uint16_t)rx.getData(1); 
          
        Serial << "H" << result << endl;
      }     
     
    }
}
