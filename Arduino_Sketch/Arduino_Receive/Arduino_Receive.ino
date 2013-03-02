

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
uint8_t data = 0;

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
      // got something
      
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
        // got a rx packet
        
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
                xbee.getResponse().getRx16Response(rx16);
        	option = rx16.getOption();
        	data = rx16.getData(0);
                for(int i=0 ; i < rx16.getDataLength(); i++){
                  Serial.print(rx16.getData(i));
                  
                }
                Serial.println(""); 
                
        } else { 
          
                xbee.getResponse().getRx64Response(rx64);
        	option = rx64.getOption();
        	data = rx64.getData(0);
                 for(int i=0 ; i < rx64.getDataLength(); i++){
                  Serial.print(rx64.getData(i));
                  
                }
                Serial.println("");
        }
        
      
        
        // TODO check option, rssi bytes    
        
      } else {
      	// not something we were expecting
        flashLed(errorLed, 1, 25);    
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
      // or flash error led
    } 
}
