#include <Streaming.h>

float inADC = 0 ;

float inVoltage = 0; 

float RH = 0;



void setup(){
  
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
}

void loop(){
  
  inADC = analogRead(A0);   
  
  Serial << "ADC= " << inADC << endl; 
   
  inVoltage = inADC*5/1024;
 
  Serial << "Voltage= " << inVoltage << endl;
 
  Serial << "---------------------" << endl;
 
  RH = (inVoltage - 0.958)/0.0307; //RH% 

  Serial << "RH= " << RH << endl; // should be around 70k    
  
  delay(2000);
  
  Serial << "---------------------" << endl;
}
