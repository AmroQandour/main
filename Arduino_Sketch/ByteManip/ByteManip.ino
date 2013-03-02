#include <Streaming.h>

#define highWord(w) ((w) >> 16)
#define lowWord(w) ((w) & 0xffff)


int loWord, hiWord; 
byte loByte, hiByte, LidByte, HidByte; 



void setup()
{
  Serial.begin(9600);
}

int TempValue = 1023;

long result =0;  
int value = 0;
uint8_t payload[] = {0,0,0};

void loop(){
  
/*  HidByte = highByte('H');
  LidByte = lowByte('H');
  loByte = lowByte(TempValue);
  hiByte = highByte(TempValue); */
  
  
  payload[0] = lowByte('H'); Serial << (char)payload[0] << endl;
  payload[1] = highByte(TempValue); 
  payload[2] = lowByte(TempValue);  
  Serial << word(payload[1], payload[2]) << endl; 
  Serial << sizeof(payload) << endl;
  /*
  HidByte = highByte(payload[0]);
  LidByte = lowByte(payload[0]);
  loByte = lowByte(TempValue);
  hiByte = highByte(TempValue); */
  /*
  int identifier = word(HidByte, LidByte);
  value = word(hiByte, loByte); 
  
  Serial << (char)identifier << value << endl; */
  
  
  //payload[0]=highByte(TempValue);
  //payload[1]=lowByte(TempValue);

  

  
  delay(1000);
  
/*
  value=word(highByte(payload[0]), lowByte(payload[0])); 
  value=word(highByte(payload[1]), lowByte(payload[1])); 
  Serial << value << endl; */ 
  
}
  
  
  

