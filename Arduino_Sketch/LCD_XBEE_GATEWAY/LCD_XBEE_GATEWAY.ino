#include "SoftwareSerial.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,6,7,9,13);

SoftwareSerial xbee(4,5); 

String inputString=""; 

void setup(){
  
   //Serial.begin(9600); 
  
   lcd.begin(16, 2);
   
   xbee.begin(9600); 
   
   inputString.reserve(20);
   
   pinMode(2,OUTPUT); 
   pinMode(3,OUTPUT);
   
}

void loop(){
  
  if(xbee.available()){
    
    delay(200); 
    
    lcd.clear(); 
    
    inputString = ""; 
    
    while(xbee.available() > 0) {
      
      inputString += (char)xbee.read(); 
      
       
      //lcd.print((char)xbee.read()); 
    } 
    
    lcd.print(inputString);
    
  }
  
  if(inputString == "LIGHT_ON") digitalWrite(2, HIGH); 
  if(inputString == "LIGHT_OFF") digitalWrite(2,LOW);
  if(inputString == "COFFEE_ON") digitalWrite(3,HIGH);
  if(inputString == "COFFEE_OFF") digitalWrite(3,LOW);
  
  
  
}
