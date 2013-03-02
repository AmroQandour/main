#include <XBee.h>
#include <SoftwareSerial.h>
#include <Streaming.h> 


SoftwareSerial mySerial(2, 3);
XBee xbee = XBee();

uint8_t payload[]={0, 0, 0, 0, 0, 0, 0};

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x409029c5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


void setup() {

  xbee.begin(9600);
  mySerial.begin(9600);
  
  pinMode(8, OUTPUT); 
  digitalWrite(8, HIGH);
  
}


void loop() {
  
    
    payload[0] = 'X';
    payload[1] = 'B';
    payload[2] = 'E';
    payload[3] = 'E'; 
    payload[4] = 'T';
    payload[5] = 'X';
    payload[6] = '\n';  
    xbee.send(zbTx);    
    mySerial << "XTEND" << endl;
    delay(2000);
}
