#include <SoftwareSerial.h>
#include <Streaming.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,6,7,9,13);



// software serial #1: TX = digital pin 2, RX = digital pin 3
SoftwareSerial xbee(4, 5);

// software serial #2: TX = digital pin 4, RX = digital pin 5
SoftwareSerial xtend(2, 3);

// software serial #3: TX = digital pin 6, RX = digital pin 7
SoftwareSerial rfid(8,10);


long previous=0;


int rf_success = 0;

String inputString=""; 


void setup(){

  lcd.begin(16, 2);

  xbee.begin(9600); 

  //xtend.begin(9600); 

  rfid.begin(9600); 

  Serial.begin(9600); 

  inputString.reserve(20);

  pinMode(2,OUTPUT); 
  pinMode(3,OUTPUT);

  while(rf_success!=1){

    //rfid.listen(); 

    if(rfid.available() > 0) {
      rf_success = 1; 
      lcd.write("Accepted"); 
      //while(rfid.available() > 0 ){
      //lcd.write(char(rfid.read())); 
      //}
      break;
    }

  }

}

void loop(){




  xbee.listen(); 

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









//xtend.listen(); 
/*
  if(xtend.available()){
 
 Serial << _BYTE(xtend.read()); 
 }*/




/*
  do{
 
 zigbee.listen(); delay(2000);
 xtend.listen(); delay(2000);
 rfid.listen(); delay(2000);
 
 }
 while(!zigbee.available() && !xtend.available() && rfid.listen()); 
 
 
 if(zigbee.available()){
 
 Serial << _BYTE(zigbee.read()); 
 }
 
 if(xtend.available()){
 
 Serial << _BYTE(xtend.read()) ;
 }
 
 if(rfid.available()){
 
 Serial << _BYTE(rfid.read());  
 }  
 
 */




