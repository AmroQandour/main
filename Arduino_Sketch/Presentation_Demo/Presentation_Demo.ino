#include <XBee.h>
#include <SoftwareSerial.h>
#include <Streaming.h> 
#include <PString.h>

XBee xbee = XBee();
uint8_t payload[]={0, 0, 0, 0, 0, 0};

XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x409029c5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

float inByte =0;
int ADCValue =0;
char myString[12];
float pressure_pKa = 0;
float temperature_c= 0;
long altitude_ft = 0;
int loWord, hiWord ;
char inString[7] = "      ";

void setup() {

  xbee.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A5, INPUT);
}


void loop() {   
    
    
    
    // MQ-2 Gas Sensor 
    // MQ-2 Gas Sensor 
    inByte = map(analogRead(A2), 0, 1023, 0 , 100);
    float2String(inByte, inString, sizeof(inByte));     
    payload[0] = 'M';
    payload[1] = inString[0]; 
    payload[2] = inString[1];  
    payload[3] = inString[2]; 
    payload[4] = inString[3]; 
    payload[5] = inString[4];
    Serial << _BYTE(payload[1]) << _BYTE(payload[2]) << _BYTE(payload[3]) << _BYTE(payload[4]) << _BYTE(payload[5]) << endl;  
    xbee.send(zbTx);
    delay(2000);
    
    
     // MQ-4 Gas Sensor 
    inByte = map(analogRead(A2), 0, 1023, 0 , 5);
    float2String(inByte, inString, sizeof(inByte));     
    payload[0] = 'L';
    payload[1] = inString[0]; 
    payload[2] = inString[1]; 
    payload[3] = inString[2]; 
    payload[4] = inString[3]; 
    payload[5] = inString[4];
    Serial << _BYTE(payload[1]) << _BYTE(payload[2]) << _BYTE(payload[3]) << _BYTE(payload[4]) << _BYTE(payload[5]) << endl;  
    xbee.send(zbTx);
    delay(2000); 
    
}

void float2String (float fl, char str[], int N) {

	boolean neg = false;
 
	if( fl<0 ){
		neg = true;
		fl*=-1;
	}
 
	float numeroFloat=fl; 
	int parteEntera[10];
	int cifra; 
	long numero=(long)numeroFloat;  
	int size=0;
  
	while(1){
		size=size+1;
		cifra=numero%10;
		numero=numero/10;
		parteEntera[size-1]=cifra; 
		if (numero==0){
			break;
		}
	}

	int indice=0;
	if( neg ){
		indice++;
		str[0]='-';
	}
	for (int i=size-1; i>=0; i--)
	{
		str[indice]=parteEntera[i]+'0'; 
		indice++;
	}

	str[indice]='.';
	indice++;

	numeroFloat=(numeroFloat-(int)numeroFloat);
	for (int i=1; i<=N ; i++)
	{
		numeroFloat=numeroFloat*10;
		cifra= (long)numeroFloat;          
		numeroFloat=numeroFloat-cifra;
		str[indice]=char(cifra)+48;
		indice++;
	}
	//str[indice]='\0';
}
