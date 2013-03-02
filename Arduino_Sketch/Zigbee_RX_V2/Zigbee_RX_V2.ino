
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


int loByte = 0 ;
int hiByte = 0 ; 
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
          
          case 'T': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;
          
          case 'H': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;
                    
          case 'A': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;
                    
          case 'P':  
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;
                    
          case 'M':  
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;
          
          case 'L':  
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;          
                    
          case 'm': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;   
                    
          case 'h': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break; 
          
          case 'p':
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;                              
                    
          case 't': 
                    for(int i =0 ; i < rx.getDataLength() ; i++){
                  
                      Serial << _BYTE(rx.getData(i));} 
                    break;                                        
                      
          case 'a': 
                    hiByte = word(rx.getData(1), rx.getData(2)); 
                    loByte = word(rx.getData(3), rx.getData(4));              
                    Serial << "a" << makeLong(hiByte, loByte); 
                    break; 
                              
        }
        Serial << endl; 
    }
  }
}
