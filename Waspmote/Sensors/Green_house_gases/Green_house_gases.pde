/*
 *  ------Green House Gases Monitoring--------
 *
 *  Explanation: Green house gases consist mostly of: Water vapor, Carbon Dioxide, Methane, Nitrous Oxide, and Ozone.
 *               This program measures Carbon Dioxide, Methane and Ozone concentrations in atmosphere.  
 *
 *  Design:                Amro Qandour
 *  Version:               1.0 
 *  Implementation:        Amro Qandour
 */

#define M1 "TEMPERATURE=" 
#define M2 "Created Temp.txt file"
#define M3 "METHANE=" 
#define M4 "Created Meth.txt file"
#define M7 "O3=" 
#define M8 "Created o3.txt file"
#define M13 "CARBON DIOXIDE=" 
#define M14 "Created co2.txt file"
#define M15 "SD Write OK" 
#define M16 "Exit Folder" 
#define M17 "The size of formattedStrings is="
#define M18 "Packet Sent Successfully to Meshlium!"
#define ppm " ppm"

packetXBee* paq_sent; 
char tempString[15];
char CH3String[15];    
char CO2String[15]; 
char O3String[15]; 
char* macHigh="          ";
char* macLow="           ";
uint8_t packID=0; 

float _CH3 = 0 ;
float CH3 = 0; 
float _CO2 = 0 ;
float CO2 = 0; 
float O3 = 0; 
float _O3 = 0;

void setup()
{
  
  USB.begin();
  
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
  
  SensorGas.setBoardMode(SENS_ON);
  SensorGas.configureSensor(SENS_CO2,1);
  SensorGas.configureSensor(SENS_SOCKET4A,1,1);
  SensorGas.configureSensor(SENS_SOCKET2B,1,2.2);
  
  SD.ON();
  USB.println(SD.flag, DEC);
  
  RTC.ON();
  RTC.setTime("01:01:01:01:01:00:00");
  RTC.setAlarm2("01:00:10", RTC_OFFSET, RTC_ALM2_MODE4);
  
  PWR.setLowBatteryThreshold(3.45);
  USB.println(PWR.getBatteryLevel(), DEC);
  USB.println(PWR.getBatteryVolts(), DEC);
  check();
  
  
  enableInterrupts(BAT_INT);
  
}

void check() 
{
  int counter = 0;  
  while(xbee802.getOwnMac()==1&&counter<4)
  {
    xbee802.getOwnMac();
    counter++;
  }
  
  Utils.hex2str(xbee802.sourceMacHigh,macHigh,4);

  Utils.hex2str(xbee802.sourceMacLow,macLow,4);
  
  USB.print("MAC Address=");USB.print(macHigh);USB.println(macLow);
  
}

void loop()
{
  
  PWR.sleep(ALL_OFF);
  
  if( intFlag & RTC_INT )
  { 
    
    USB.begin();
    xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL); delay(1000);
    xbee802.ON(); delay(1000);    
    SensorGas.setBoardMode(SENS_ON); delay(1000); 
    SensorGas.configureSensor(SENS_CO2,1); delay(1000);
    SensorGas.configureSensor(SENS_SOCKET4A,1,1); delay(1000);
    SensorGas.configureSensor(SENS_SOCKET2B,1,2.2); delay(1000);
    SD.ON(); delay(1000);
    
/* Temperature Program: - 
 * Gets the temperature sensor value
 * Saves value on SD card 
 * Puts the value into a string which later will be sent to Meshlium and saved on a DATABASE
 */
    
    Utils.float2String(((100*(SensorGas.readValue(SENS_TEMPERATURE)))-50), tempString, 3);
    
    USB.print(M1); USB.println(tempString); //debug
    
    char* tempSave = (char*) calloc(15,sizeof(char));
    tempFunction(tempSave);          
    
    SD.mkdir("Temperature");
    SD.cd("Temperature"); 
    
    if (SD.create("Temp.txt")) {
    USB.println("Created Temp.txt file"); 
    } //debug 
    if(SD.appendln("Temp.txt", tempSave)){ 
    USB.println(M15);
    } //debug 
    if(SD.cd("..")){
    USB.println(M16); //debug 
    }
    free(tempSave);
    tempSave = NULL; 
    
/* Methane Program: 
 * Gets the sensor value from the Methane sensor
 * Converts the output into a known concentration
 * Saves the value on SD Card
 * Puts the value in a String and then sent to Meshlium and saved on a Database
 * Methane Sensor ppm output = 10^((log( (V_i - V_s)/Vs) *R_l / 48.58)/ -0.455931)
 */
     
    SensorGas.setSensorMode(SENS_ON, SENS_SOCKET4A);
    delay(10000);
    
    CH3 = SensorGas.readValue(SENS_SOCKET4A) ; 
    _CH3 = pow( 10, ((log(((5-CH3)/CH3)/48.58))/-0.455931) ) ; 
       
    Utils.float2String(_CH3, CH3String, 3); 
    USB.print(M3); USB.print(CH3String); USB.println(ppm); 
    
    char* CH3Save = (char*) calloc(15,sizeof(char));
    methFunction(CH3Save); 
    
    SD.mkdir("Methane");
    SD.cd("Methane");
    if (SD.create("Meth.txt")){
        USB.println(M4); 
      }//debug 
        
    if(SD.appendln("Meth.txt", CH3Save)){ 
      USB.println(M15); //debug
    }
    if(SD.cd("..")){
      USB.println(M16); //debug
    }
    
    free(CH3Save);
    CH3Save=NULL;
    
    SensorGas.setSensorMode(SENS_OFF, SENS_SOCKET4A);   
    
    delay(5000);
    
/* Nitrogen Dioxide Program: - 
 * Gets the value from the NO2 Sensor
 * Saves the value on SD card
 * Puts it in a String and sends it to Meshlium database
 */ 
  
    SensorGas.setSensorMode(SENS_ON, SENS_SOCKET2B);
    delay(30000);   
    
    O3 = SensorGas.readValue(SENS_SOCKET2B) ; 
    _O3 = pow(10, log( ( ( (1.8 -O3)/O3) / 500) ) / 1.871  ) ; 
    
    Utils.float2String(_O3, O3String, 3);  
    USB.print(M7); USB.print(O3String); USB.println(ppm); //debug
    
    char* O3Save = (char*) calloc(15,sizeof(char));   
    O3Function(O3Save); 
    
    SD.mkdir("O3"); 
    SD.cd("O3");
   
    if (SD.create("o3.txt")){
        USB.println(M8); 
      } //debug 
    if(SD.appendln("o3.txt", O3Save)){ 
      USB.println(M15);//debug 
      }
    if(SD.cd("..")){
      USB.println(M16); 
      } //debug 
    
    free(O3Save);
    O3Save=NULL;  
    SensorGas.setSensorMode(SENS_OFF, SENS_SOCKET2B);
    
    
/* Sending to Meshlium
 * Takes all the measurments and puts them in a formatted string and sends it to Meshlium
*/ 

    packID=packID + 1;
    char* data_to_meshlium = (char*) calloc(1,sizeof(char)); 
    formatStrings(data_to_meshlium);
    USB.print(M17); USB.println(Utils.sizeOf(data_to_meshlium)); //debug
    
    paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
    paq_sent->mode=UNICAST; 
    paq_sent->MY_known=0; 
    paq_sent->packetID=packID; 
    paq_sent->opt=0; 
    xbee802.hops=0; 
    xbee802.setOriginParams(paq_sent, MAC_TYPE);
    xbee802.setDestinationParams(paq_sent, "0013A200406C277C", data_to_meshlium, MAC_TYPE, DATA_ABSOLUTE);
    if( xbee802.sendXBee(paq_sent) )
    {
      Utils.blinkLEDs(2000);
    }
    if( !xbee802.error_TX )
    {
      XBee.println(M18);
    }
    
    free(paq_sent);
    paq_sent=NULL;             
    free(data_to_meshlium);
    data_to_meshlium=NULL;   
    delay(2000);
    
    intFlag &= ~(RTC_INT); // Clear flag
    
    RTC.clearAlarmFlag();
    
    RTC.setAlarm2("01:00:10", RTC_OFFSET, RTC_ALM2_MODE4);
    
  }
  
  if (intFlag & BAT_INT)
  { 
    Utils.blinkLEDs(4000);
    Utils.blinkLEDs(4000);
    intFlag &= ~(BAT_INT);
    intCounter--;
    intArray[BAT_POS]--; 
    disableInterrupts(RTC_INT);     
  }

}


void tempFunction(char* _tempSave)
{
  sprintf(_tempSave,"Temp:%s%c%c", tempString, '\r', '\n');
}


void methFunction(char* _CH3Save)
{
  sprintf(_CH3Save,"Methane:%s%c%c", CH3String,'\r','\n');
}

void O3Function(char* _O3Save)
{
  sprintf(_O3Save,"O3:%s%c%c", O3String,'\r','\n');
}

void CO2Function(char* _CO2Save) 
{
  sprintf(_CO2Save,"CO2:%s%c%c", CO2String,'\r','\n');
}

void formatStrings(char* data_to_send)
{
  sprintf(data_to_send, "%c%c%c%s%s%c%d%c%d%c%s%c%s%c%s%c%s%c%c%c", '*', '*', '*', macHigh, macLow, '&',  PWR.getBatteryLevel(), '&',  PWR.getBatteryVolts(), '&', tempString, '&', CH3String, '&', CO2String, '&', O3String, '&', '\r', '\n');
}


  
  
