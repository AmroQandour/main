int sensorPin = A0; 

float sensorValue=0;

float sensorVoltage=0;

void setup(){
  
  Serial.begin(9600);
  pinMode(12, OUTPUT); 
  
}

void loop(){
  
  sensorValue = analogRead(sensorPin);
  sensorVoltage = 5*sensorValue/1023;
  Serial.print((sensorVoltage)); Serial.println(" Volts");
  delay(5000);
  
  if (sensorVoltage > 3){
    digitalWrite(12, HIGH);   // set the LED on
    delay(1000);              // wait for a second
    digitalWrite(12, LOW);    // set the LED off
    delay(1000);              // wait for a second
  }
    
}
  
