#include <WaspSensorGas.h>
#include <Wire.h>

void setup(){
  Serial.begin(9600);
  SensorGas.setBoardMode(SENS_ON);
}

void loop(){
  
  float Temp = SensorGas.readValue(SENS_TEMPERATURE); 
  
  Serial.println(Temp); 



}
