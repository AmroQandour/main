#include <XBee.h>
#include <SoftwareSerial.h>
#include <Streaming.h> 
#include <PString.h>
#include <SPI.h>



XBee xbee = XBee();

uint8_t payload[]={0,0,0,0,0,0}; 

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x409029c5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();



char inData[]="";


void setup() {

  xbee.begin(9600);
  

}


void loop() {
  
  // Temperature
    float Temp = 12.35; 
    float2String(Temp, inData, 2); 
    
    payload[0] = 'T';
    payload[1] = (byte)inData[0]; 
    payload[2] = (byte)inData[1]; 
    payload[3] = (byte)inData[2]; 
    payload[4] = (byte)inData[3]; 
    payload[5] = (byte)inData[4]; 
    
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
