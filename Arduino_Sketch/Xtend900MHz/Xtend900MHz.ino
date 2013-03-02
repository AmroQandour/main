#include <SoftwareSerial.h>
#include <Streaming.h>

//API STRUCTURE
struct API_FRAME {
  
  byte delimiter;
  byte MSB;
  byte LSB;  
  //String data;
  uint8_t data; 
  byte calc_checksum; 
}; //  API_CMD;






API_FRAME api; 
API_FRAME * cmd = &api; 

SoftwareSerial mySerial(2, 3);


char dataIn[]= "\nBarcelona Starting Line Up:\nVictor Valdes\nAlvez-Puyol-Pique-Abidal\nXavi-Busquetes-Iniesta\nSanchez-Messi-Villa";  


void setup()  
{ 
  
  Serial.begin(9600);
  pinMode(8, OUTPUT); 
  digitalWrite(8, HIGH);
  mySerial.begin(9600);
  
}

void loop() // run over and over
{

    cmd->data=72;
    cmd->delimiter=0x7E;
    cmd->MSB=highByte(sizeof(api.data));
    cmd->LSB=lowByte(sizeof(api.data));
    

    mySerial << api.delimiter << api.MSB << api.LSB << api.data << endl;
  
    //mySerial.write(dataIn);

    Serial  << _HEX(api.delimiter) << _BYTE(api.MSB) << _BYTE(api.LSB) << api.data << endl; 
  
    delay(5000);
  
}
