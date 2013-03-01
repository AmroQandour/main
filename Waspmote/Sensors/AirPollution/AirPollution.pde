/*
 *  ------Air Pollution Monitoring--------
 *
 *  Explanation: This program will monitor the concentrations of air pollutants in a specific environment. The sensors used in this implementation
 *               include: Nitrogen Dioxide, Carbon Monoxide, Ammonia, Iso-Butane, Ethanol, Toulene, Hydrogen Sulphide, and Methane.            
 *
 *  Design:                Amro Qandour
 *  Version:               1.0 
 *  Implementation:        Amro Qandour
 */
 
 
#define M1 "TEMPERATURE=" 
#define M2 "Created Temp.txt file"
#define M3 "METHANE=" 
#define M4 "Created Meth.txt file"
#define M19 "AMMONIA=" 
#define M20 "Created Ammonia.txt file"
#define M5 "AIR POLLUTION_1=" 
#define M6 "Created air1.txt file"
#define M21 "AIR POLLUTION_2=" 
#define M22 "Created air2.txt file"
#define M7 "NO2=" 
#define M8 "Created no2.txt file"
#define M9 "HUMIDITY=" 
#define M10 "Created humd.txt file"
#define M11 "CARBON MONOXIDE=" 
#define M12 "Created CO.txt file"
#define M15 "SD Write OK" 
#define M16 "Exit Folder" 
#define M17 "The size of formattedStrings is="
#define M18 "Packet Sent Successfully to Meshlium!"
#define ppm " ppm"

//----------Sensor Thresholds----------

#define max_temp 50 
#define max_CH3 5000 
#define max_NO2 0.8          
#define max_CO 1000
#define max_NH3 1000
#define max_AIR1 1000
#define max_AIR2 1000

//-------------------------------------

packetXBee* paq_sent; 
char tempString[15];
char CH3String[15];    
char AIR1String[15];
char AIR2String[15];
char NO2String[15]; 
char HumdString[15]; 
char COString[15];
char NH3String[15];
char* macHigh="          ";
char* macLow="           ";
uint8_t packID=0; 
float _CH3 = 0 ;
float CH3 = 0; 
float _CO = 0 ;
float CO = 0;  
float _NH3 = 0 ;
float NH3 = 0;
float _NO2 = 0 ;
float NO2 = 0;
float AIR1 = 0;
float _AIR1 =0;
float AIR2 =0;
float _AIR2 =0;

void setup()
{
  USB.begin();

  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
  
  check();

  SensorGas.setBoardMode(SENS_ON);
  SensorGas.configureSensor(SENS_SOCKET4A,1,1);
  SensorGas.configureSensor(SENS_SOCKET2B,1,2.2);
  SensorGas.configureSensor(SENS_SOCKET2A,1,1);
  SensorGas.configureSensor(SENS_SOCKET3B, 1,13.3);
  SensorGas.configureSensor(SENS_CO2,1);
  
  SD.ON();
  USB.println(SD.flag, DEC);
  USB.print("Disk Size: ");
  USB.println(SD.getDiskSize());
  USB.print("Disk Free: ");  
  USB.println(SD.getDiskFree());
  
  delay(4000);
  
  RTC.ON();
  RTC.setTime("01:01:01:01:01:00:00");
  
  PWR.setLowBatteryThreshold(3.45);
  USB.println(PWR.getBatteryLevel(), DEC);
  USB.println(PWR.getBatteryVolts(), DEC);
  
  RTC.setAlarm2("01:00:20", RTC_OFFSET, RTC_ALM2_MODE4); 
   
}

void check() 
{
  int counter = 0;  
  while(xbee802.getOwnMac()==1&&counter<4){
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
    delay(5000);
    
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
    
    
    /* Air Contaminents-1 Program: - 
     * Gets the sensor value 
     * Saves it on SD card
     * converts the output to a known concentration
     * puts the value in a string and sent to Meshlium to be saved on a Database
    */
    
    
    SensorGas.setSensorMode(SENS_ON, SENS_SOCKET2A); 
    delay(30000);
    char* airSave = (char*) calloc(15,sizeof(char));
    Utils.float2String((SensorGas.readValue(SENS_SOCKET2A)), AIR1String, 3);
    USB.print(M5); USB.println(AIR1String); //debug
    
    
    
    air1Function(airSave); 
    
    SD.mkdir("Air1"); 
    SD.cd("Air1");
    
    if (SD.create("air1.txt")){
        USB.println(M6); 
      } //debug
    if(SD.appendln("air1.txt", airSave)){ 
      USB.println(M15); 
      } //debug
    if(SD.cd("..")){
      USB.println(M16); //debug
    }
    
    free(airSave); 
    airSave=NULL; 
    SensorGas.setSensorMode(SENS_OFF, SENS_SOCKET2A); 
    
    delay(5000);
    
    /* Air Contaminents-2 Program: - 
     * Gets the sensor value 
     * Saves it on SD card
     * converts the output to a known concentration
     * puts the value in a string and sent to Meshlium to be saved on a Database
    */  
    
    
    
    /* Nitrogen Dioxide Program: - 
     * Gets the value from the NO2 Sensor
     * Saves the value on SD card
     * Puts it in a String and sends it to Meshlium database
    */ 
    
     
    SensorGas.setSensorMode(SENS_ON, SENS_SOCKET2B);
    delay(30000);   
    
    NO2 = SensorGas.readValue(SENS_SOCKET2B) ; 
    _NO2 = pow(10, log( ( ( (1.8 -NO2)/NO2) / 500) ) / 1.871  ) ; 
    
    Utils.float2String(_NO2, NO2String, 3);  
    USB.print(M7); USB.print(NO2String); USB.println(ppm); //debug
    
    char* NO2Save = (char*) calloc(15,sizeof(char));   
    NO2Function(NO2Save); 
    
    SD.mkdir("NO2"); 
    SD.cd("NO2");
     
    if (SD.create("no2.txt")){
        USB.println(M8); 
      } //debug 
    if(SD.appendln("no2.txt", NO2Save)){ 
      USB.println(M15);//debug 
      }
    if(SD.cd("..")){
      USB.println(M16); 
      } //debug 
    
    free(NO2Save);
    NO2Save=NULL;  
    SensorGas.setSensorMode(SENS_OFF, SENS_SOCKET2B);
    
    delay(5000);
    
    
    
    /* Humditiy Program: - 
     * Measures humidity 
     * Puts the value in a string 
     * Saves value on SD Card 
    */
    
    char* humdSave = (char*) calloc(15,sizeof(char));
    Utils.float2String((((32*(SensorGas.readValue(SENS_HUMIDITY)))-26)), HumdString, 3);
    USB.print(M9); USB.println(HumdString); 
    
    humFunction(humdSave); 
    
    SD.mkdir("Humd"); 
    SD.cd("Humd");
     
    if (SD.create("hum.txt")) {
        USB.println(M10); 
      } //debug 
    if(SD.appendln("hum.txt", humdSave)){ 
      USB.println(M15);//debug 
      }
    if(SD.cd("..")){
      USB.println(M16); //debug
    }
    
    free(humdSave);
    humdSave=NULL;
    delay(5000);
    
    
    
    /* Carbon Monoxide Program: 
     * Gets the carbon monoxide output from CO sensor 
     * translates the output into a known concentration
     * saves the value on SD card
     * puts the value into a string
    */ 
    
    CO = SensorGas.readValue(SENS_SOCKET3B); 
    _CO = pow(10, ( log( ( ( 5 - CO)/CO )/1.5624) / (log(0.8)/log(10)) ) );  
    
    Utils.float2String(_CO, COString, 3);   
    USB.print(M11); USB.print(COString); USB.println(ppm); 
    
    char* COSave = (char*) calloc(15,sizeof(char)); 
    COFunction(COSave); 
    
    SD.mkdir("CO");
    SD.cd("CO");
    
    if (SD.create("co.txt")){
        USB.println(M12); 
      }//debug 
    if(SD.appendln("co.txt", COSave)){ 
      USB.println(M15);//debug 
      }
    if(SD.cd("..")){
      USB.println(M16); //debug
    }
    
    free(COSave);
    COSave = NULL; 
    delay(5000);
    
    
    /* Ammonia Program: 
     * Gets the carbon dioxide output from CO sensor 
     * translates the output into a known concentration
     * saves the value on SD card
     * puts the value into a string
    */ 
    
    
     
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
      Utils.blinkLEDs(5000);
    }
    if( !xbee802.error_TX )
    {
      XBee.println(M18);
    
      delay(1000);
    }
    free(paq_sent);
    paq_sent=NULL;               
    free(data_to_meshlium);
    data_to_meshlium=NULL;
    
    intFlag &= ~(RTC_INT);
    RTC.clearAlarmFlag();
    RTC.setAlarm2("01:00:20", RTC_OFFSET, RTC_ALM2_MODE4); 
    
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

void air1Function(char *_airSave)
{
  sprintf(_airSave,"Air-1:%s%c%c", AIR1String,'\r','\n');
}

void NO2Function(char* _NO2Save)
{
  sprintf(_NO2Save,"NO2:%s%c%c", NO2String,'\r','\n');
}

void humFunction(char* _humSave) 
{
  sprintf(_humSave,"Humd:%s%c%c", HumdString,'\r','\n');
}

void COFunction(char* _COSave) 
{
  sprintf(_COSave,"CO:%s%c%c", COString,'\r','\n');
}


void formatStrings(char* data_to_send)
{
  sprintf(data_to_send, "%c%c%c%s%s%c%d%c%d%c%s%c%s%c%s%c%s%c%c%c", '*', '*', '*', macHigh, macLow, '&',  PWR.getBatteryLevel(), '&',  PWR.getBatteryVolts(), '&', tempString, '&', CH3String, '&', COString, '&', NO2String, '&', '\r', '\n');
}

/* Alarm Generation Function:
 * Should be used when the sensors are calibrated.
 * Change thresholds to suit application requirements. 
*/

void Alarm_Generation(char* _alarm_to_meshlium)
{ 
  
  strcat(_alarm_to_meshlium, "$$$"); 
  
  if((atoi(tempString) > max_temp))
  {
    strcat(_alarm_to_meshlium, tempString);
    strcat(_alarm_to_meshlium, "&");   
  }
  
  if((atoi(CH3String) > max_CH3))
  {
    strcat(_alarm_to_meshlium, CH3String);
    strcat(_alarm_to_meshlium, "&");
  }
  
  if((atoi(AIR1String) > max_AIR1))
  {
    strcat(_alarm_to_meshlium, AIR1String);
    strcat(_alarm_to_meshlium, "&");
  }
  
  if((atoi(NO2String) > max_NO2))
  {
    strcat(_alarm_to_meshlium, NO2String);
    strcat(_alarm_to_meshlium, "&");
  }
  
  if((atoi(COString) > max_CO))
  {
    strcat(_alarm_to_meshlium, COString);
    strcat(_alarm_to_meshlium, "&");
  }
  
}
