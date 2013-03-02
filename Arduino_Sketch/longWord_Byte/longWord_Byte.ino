
#include <Streaming.h>

#define makeLong(hi, low) (((long) hi) << 16 | (low))

#define highWord(w) ((w) >> 16)
#define lowWord(w) ((w) & 0xffff)

long longValue = 0x143;

int loWord, hiWord ; 

uint8_t payload[]={0,0,0,0}; 

void setup(){
  
  Serial.begin(9600); 
  
}

void loop(){
  
  loWord = lowWord(longValue); 
  hiWord = highWord(longValue);
  p
  int hello = makeLong(hiWord, loWord);
  
  Serial << hello << endl;
  
  
  
  
  delay(2000); 
  
  
}
  
