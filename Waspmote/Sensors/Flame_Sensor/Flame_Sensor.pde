void setup(){
  
  USB.begin(); 
  SensorGas.setBoardMode(SENS_ON); 
  
  
  pinMode(4, OUTPUT); 
  
  digitalWrite(4, LOW);  // LED1 
  
  pinMode(7, OUTPUT);
  
  digitalWrite(7, LOW); // LED2
  
  
}

void loop(){
  
  int flame=Utils.map(analogRead(6),0,1024,0,100);
  
  if(flame<90){
    
    digitalWrite(4,HIGH);
    Utils.blinkLEDs(2000); 
  }
  else digitalWrite(4,LOW); 
  
  USB.print("Flame= ");
  USB.println(flame); 
  delay(2000); 
  USB.print("Temperature= ");
  USB.println(analogRead(ANALOG7)); 
  delay(2000); 

}

