void setup(){
  
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  
  pinMode(9, OUTPUT); 
  
  pinMode(8, OUTPUT); 
  
}

float sens= 0 ;

float sens_value;

float outputValue=0; 

void loop(){
  
  sens = analogRead(A0);
  sens_value= sens*5/1023;
  if(sens_value < 2){
   digitalWrite(8, HIGH);
   delay(1000);
   digitalWrite(8,LOW); 
  }
   
  Serial.println(sens);
  Serial.println("");
  outputValue = map(sens, 0, 1023, 0, 255); 
  analogWrite(9, outputValue); 
  Serial.println(5*outputValue/255);
  
 
  
  
  
  delay(10000);
}

  
  
  
