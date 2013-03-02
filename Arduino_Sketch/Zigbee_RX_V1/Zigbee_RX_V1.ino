
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

#define makeLong(hi, low) (((long) hi) << 16 | (low))
#define highWord(w) ((w) >> 16)
#define lowWord(w) ((w) & 0xffff)


uint8_t loByte = 0 ;
uint8_t hiByte = 0 ; 
int inData = 0;
float inByte = 0; 

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
          
        switch(rx.getData(0)){
          
          case 'T': //Serial << "T" << word(rx.getData(1), rx.getData(2));
                    Serial << "T" << _BYTE(rx.getData(1)) << _BYTE(rx.getData(2)) << _BYTE(rx.getData(3)) << _BYTE(rx.getData(4)) << _BYTE(rx.getData(5)) << endl;
                    break;
          
          case 'H': Serial << "H" << word(rx.getData(1), rx.getData(2));
                    break;
                    
          case 'A': Serial << "L" << word(rx.getData(1), rx.getData(2));
                    break;
                    
          case 'P': Serial << "P" << word(rx.getData(1), rx.getData(2));
                    break;
                    
          case 'm': Serial << "m" << word(rx.getData(1), rx.getData(2));
                    break;   
                    
          case 'h': Serial << "h" << word(rx.getData(1), rx.getData(2));
                    break; 
          
          case 'p': Serial << "p" << word(rx.getData(1), rx.getData(2));
                    break;                              
                    
          case 't': Serial << "t" << word(rx.getData(1), rx.getData(2));
                    break;                                        
           /*           
          case 'a': hiByte = word(rx.getData(4), rx.getData(3)); 
                    loByte = word(rx.getData(2), rx.getData());              
                    Serial << "a" << makeLong(hiByte, loByte);
                    break;*/
                              
        }
        Serial << endl; 
    }
  }
}
