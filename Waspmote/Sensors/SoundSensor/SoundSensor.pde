void setup(){
  
  pinMode(DIGITAL6, OUTPUT);
  pinMode(DIGITAL1, OUTPUT); 
  
  digitalWrite(DIGITAL6, HIGH); 
  
  USB.begin(); 
  
}


void loop(){
  
  
  int value = analogRead(ANALOG6);
  
  USB.println(value); 
  
  analogWrite(DIGITAL1, value); 
}

