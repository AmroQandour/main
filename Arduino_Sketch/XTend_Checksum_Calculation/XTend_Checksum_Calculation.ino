#include <Streaming.h>

void setup(){
  Serial.begin(9600); 
  
}

void loop(){
  
  int addition = 0x01+0x52+0xFF+0xFF+0x00+0x31+0x32+0x33;
  
  Serial << _HEX(checksum(addition)) << endl ; 
  
  Serial << "Sum of Bytes= " << _BIN(addition) << endl; 
  
  uint8_t loByte = lowByte(addition);
  
  Serial << "loByte = " << _BIN(loByte) << endl; 
  
  loByte = 0xFF - loByte ; 
  
  Serial << "Checksum= " << _HEX(loByte) << endl; 
 
  int calc_checksum = (addition + loByte ) ;
  
  Serial << "Verification Checksum (must be 0xFF)= " << _HEX(lowByte(calc_checksum)) << endl; 
  
  delay(2000); 

}

int checksum(int sdata){

	int addition;
	uint8_t loByte;
	uint8_t checksum; 
	
	addition = sdata;
	loByte = lowByte(addition);
	checksum = 0xFF - loByte ; 
	
	return checksum;
}
