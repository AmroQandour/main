#include <XBee.h>
#include <SoftwareSerial.h>

uint8_t payload[] = { 0, 0 };

XBee xbee = XBee();
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x409029c5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int gas_measure = 0 ;

void setup(){ 
  
  pinMode(A0, INPUT); 
  
  xbee.begin(9600);
}

void loop(){
  
  gas_measure = 5*analogRead(A0)/1023;
  Serial.println(gas_measure); 
  
  payload[0]= gas_measure >> 8 & 0xff; 
  payload[1]= gas_measure & 0xff;
  
  Serial.print(payload[0]); Serial.print(payload[1]);
  
  xbee.send(zbTx); 
  
  if(xbee.readPacket(1000)){
    
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      
         if (txStatus.getDeliveryStatus() == SUCCESS) {
          
          Serial.print("No Errors|Message Sent");
          
        }
        
        else if (xbee.getResponse().isError()) {
          Serial.println("There are Errors"); 
        }
       
    }
    
    
  }
  
  
  delay(5000);      
        
    
  
}
