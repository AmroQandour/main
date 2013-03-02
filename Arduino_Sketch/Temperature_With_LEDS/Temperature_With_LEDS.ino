int sensorPin = A0; 

float sensorValue=0;

float sensorVoltage=0;

int buttonState=0;


void setup(){
  
  Serial.begin(9600);
  pinMode(2, INPUT);
  
  pinMode(12, OUTPUT);   
  pinMode(8, OUTPUT);   
  pinMode(7, OUTPUT); 
  pinMode(4, OUTPUT); 
  
}

void loop(){
  
  sensorValue = analogRead(sensorPin);
  sensorVoltage = 5*sensorValue/1023;
  Serial.print((sensorVoltage)); Serial.println(" Volts");
  
  if(sensorVoltage > 3){
  
  buttonState = digitalRead(2); 
  
  }
  
  switch(buttonState){
    
    case 1: turnLEDs();
            break;
            
    case 0: break;
  }
  
  delay(1000);
    
}

void turnLEDs(){
  digitalWrite(12, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(12, LOW);    // set the LED off
  delay(100);              // wait for a second
  digitalWrite(8, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(8, LOW);    // set the LED off
  delay(100);              // wait for a second
  digitalWrite(7, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(7, LOW);    // set the LED off
  delay(100);              // wait for a second
  digitalWrite(4, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(4, LOW);    // set the LED off
  delay(100);              // wait for a second
  buttonState = 0 ;
}

  
