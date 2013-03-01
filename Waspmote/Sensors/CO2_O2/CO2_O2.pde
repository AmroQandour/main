// Program 2: Carbon Dioxide, Carbon Monoxide, Temperature, Nitrogen Dioxide 


#define MAC "0013A200406918B0" 
#define NODE "NODE-ID:2"
  
packetXBee* paq_sent;
char Oxygen[50];
char Carbon[50];
float O2=0;
float CO2 =0; 
char tosend[100];





void blink(){
  
  Utils.blinkLEDs(1000);
}

void setup(){
  
  SensorGas.setBoardMode(SENS_ON);
  blink();
  // SensorGas.configureSensor(SENS_O2,100); //Carbon Dioxide
  // SensorGas.setSensorMode(SENS_ON, SENS_O2); 
  
  SensorGas.configureSensor(SENS_CO2,1); //Carbon Dioxide
  SensorGas.setSensorMode(SENS_ON, SENS_CO2);
  
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
  blink();
  
  
  
}

void get_CarbonD(){
  CO2 = 0.2 - SensorGas.readValue(SENS_CO2);
  Utils.float2String( CO2 , Carbon, 2); // %
  sprintf(tosend, "Carbon= %s\r\0", Carbon);
}


void loop(){
    
    delay(30000);
    get_CarbonD();
    blink();
    
   // O2 = SensorGas.readValue(SENS_O2) ; 
    
   // Utils.float2String( 30*O2, Oxygen, 2); // %
    
    //sprintf(tosend, "Oxygen= %s\r\0", Oxygen);
    paq_sent = (packetXBee*) calloc (1, sizeof(packetXBee));
    paq_sent->mode=UNICAST;
    paq_sent->packetID=0x52;
    paq_sent->opt=0; 
    xbee802.hops=0;
    xbee802.setOriginParams(paq_sent, MAC_TYPE);
    xbee802.setDestinationParams(paq_sent, MAC, tosend , MAC_TYPE, DATA_ABSOLUTE);
    xbee802.sendXBee(paq_sent); 
    if( !xbee802.error_TX )
    {
      blink();
    }
    free(paq_sent);
    paq_sent=NULL;

}
  
  
  
  
