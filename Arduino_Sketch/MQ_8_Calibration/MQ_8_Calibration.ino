#include <Streaming.h>

float inADC = 0 ;

float inPercentage =0;

float inVoltage = 0; 

float Rs = 0 ;

float ROut=0;


void setup(){
  
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
}

void loop(){
  
  inADC = analogRead(A0);   
  
  Serial << "ADC= " << inADC << endl; 
  
  inPercentage = map(inADC, 0,1023, 0,100);
  
  Serial << "Percentage = " << inPercentage << endl;
   
  inVoltage = inADC*5/1024;
 
  Serial << "Voltage= " << inVoltage << endl;
 
  Serial << "---------------------" << endl;
 
  Rs = (50000/inVoltage) - 10000;

  Serial << "Rs= " << Rs << endl; // should be around 70k 
  
  ROut = Rs / (10000);
  
  Serial << "Rs/Ro= " << ROut << endl; // should be around (70);   
  
  delay(2000);
  
  Serial << "---------------------" << endl;
}


