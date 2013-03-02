

#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();

int statusLed = 11;
int errorLed = 12;
int dataLed = 10;

uint8_t option = 0;
uint8_t data =0;

char buf[1];

void flashLed(int pin, int times, int wait) { 
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(dataLed,  OUTPUT);
  
  // start serial
  xbee.begin(9600);
  
  flashLed(statusLed, 3, 50);
}

// continuously reads packets, looking for RX16 or RX64
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        // got a rx packet
          
                xbee.getResponse().getRx16Response(rx16);
               
        	option = rx16.getOption();
        	data = rx16.getData(0);
               
                //Serial.print((char)data); 
                
              
                 for(int i=0 ; i < rx16.getDataLength(); i++){
                 Serial.print((char)rx16.getData(i));
                  
                  
                }
                
                Serial.println("");
      
                
                
        }
        
      
        
        // TODO check option, rssi bytes    
        
      } else {
      	// not something we were expecting
        flashLed(errorLed, 1, 25);    
      }
 
     
}
