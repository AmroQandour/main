#include <XBee.h>
#include <SoftwareSerial.h>
#include <Streaming.h> 
#include <PString.h>
#include <SPI.h>

// Barometric Pressure----------------

#define NWS_BARO 30.03

// Pin definitions
#define MPL115A1_ENABLE_PIN 9
#define MPL115A1_SELECT_PIN 10

// Masks for MPL115A1 SPI i/o
#define MPL115A1_READ_MASK  0x80
#define MPL115A1_WRITE_MASK 0x7F 

// MPL115A1 register address map
#define PRESH   0x00    // 80
#define PRESL   0x02    // 82
#define TEMPH   0x04    // 84
#define TEMPL   0x06    // 86

#define A0MSB   0x08    // 88
#define A0LSB   0x0A    // 8A
#define B1MSB   0x0C    // 8C
#define B1LSB   0x0E    // 8E
#define B2MSB   0x10    // 90
#define B2LSB   0x12    // 92
#define C12MSB  0x14    // 94
#define C12LSB  0x16    // 96
#define C11MSB  0x18    // 98
#define C11LSB  0x1A    // 9A
#define C22MSB  0x1C    // 9C
#define C22LSB  0x1E    // 9E

// Unit conversion macros
#define FT_TO_M(x) ((long)((x)*(0.3048)))
#define KPA_TO_INHG(x) ((x)*(0.295333727))
#define KPA_TO_MMHG(x) ((x)*(7.50061683))
#define KPA_TO_PSIA(x) ((x)*(0.145037738))
#define KPA_TO_KGCM2(x) ((x)*(0.0102))
#define INHG_TO_PSIA(x) ((x)*(0.49109778))
#define DEGC_TO_DEGF(x) ((x)*(9.0/5.0)+32)

//-----------
#define makeLong(hi, low) (((long) hi) << 16 | (low))
#define highWord(w) ((w) >> 16)
#define lowWord(w) ((w) & 0xffff)

XBee xbee = XBee();

uint8_t payload[]={0, 0, 0};

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x409029c5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int inByte =0;
float pressure_pKa = 0;
float temperature_c= 0;
long altitude_ft = 0;
uint16_t loWord, hiWord ; 

void setup() {

  xbee.begin(9600);
  SPI.begin();
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  pinMode(MPL115A1_SELECT_PIN, OUTPUT);
  pinMode(MPL115A1_ENABLE_PIN, OUTPUT);
  
  digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    // set the chip select inactive, select signal is CS LOW
  digitalWrite(MPL115A1_SELECT_PIN, HIGH);
}


void loop() {
  
  // Temperature
    inByte = round(map(analogRead(A3), 0, 1023, 0, 5)); 
    payload[0] = 'T'; 
    payload[1] = highByte(inByte); 
    payload[2] = lowByte(inByte);  
    xbee.send(zbTx);
    delay(1000);
   
    
   // Humidity 
    inByte = round(map(analogRead(A4), 0, 1023, 0, 5));  
    payload[0] = 'H';
    payload[1] = highByte(inByte); 
    payload[2] = lowByte(inByte);  
    xbee.send(zbTx);
    delay(1000);
    
    
   // MQ-6 Gas Sensor 
    inByte = analogRead(A0); 
    payload[0] = 'A';
    payload[1] = highByte(inByte); 
    payload[2] = lowByte(inByte);  
    xbee.send(zbTx);
    delay(1000);
    
   
   // Barometric Pressure Sensor:------------------------------- 
    digitalWrite(MPL115A1_ENABLE_PIN, HIGH); delay(60);
    pressure_pKa = calculatePressurekPa();
    temperature_c = calculateTemperatureC();
    altitude_ft = FT_TO_M(calculateAltitudeFt(pressure_pKa));
    digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    //kPa; 
    payload[0] = 'P'; 
    payload[1] = highByte((int)pressure_pKa);
    payload[2] = lowByte((int)pressure_pKa);
    xbee.send(zbTx); 
    delay(1000);
   
    //inHG:
    payload[0] = 'm'; 
    payload[1] = highByte((int)KPA_TO_INHG(pressure_pKa));
    payload[2] = lowByte((int)KPA_TO_INHG(pressure_pKa));
    xbee.send(zbTx); 
    delay(1000); 
    
    //in mmHG:
    payload[0] = 'h'; 
    payload[1] = highByte((int)KPA_TO_MMHG(pressure_pKa));
    payload[2] = lowByte((int)KPA_TO_MMHG(pressure_pKa));
    xbee.send(zbTx); 
    delay(1000); 
    
     //in PSI:
    payload[0] = 'p'; 
    payload[1] = highByte((int)KPA_TO_PSIA(pressure_pKa));
    payload[2] = lowByte((int)KPA_TO_PSIA(pressure_pKa));
    xbee.send(zbTx); 
    delay(1000); 
    
    //Temp: 
    payload[0] = 't'; 
    payload[1] = highByte((int)temperature_c);
    payload[2] = lowByte((int)temperature_c);
    xbee.send(zbTx); 
    delay(1000); 
    
    //Alt: 
    
    loWord = lowWord(altitude_ft);
    hiWord = highWord(altitude_ft);
    payload[0] = 'a'; 
    payload[1] = lowByte(loWord);
    payload[2] = highByte(loWord);
    payload[3] = lowByte(hiWord);
    payload[4] = highByte(hiWord); Serial << makeLong(hiWord,loWord) << endl;
    /*
    //Receiver: 
     uint16_t ac1 =  word(payload[1],payload[0]);
     uint16_t ac2 = word(payload[3],payload[2]);
     Serial << makeLong(ac2,ac1) << endl;
    */      
    //payload[0] = 'a';
    //payload[1] = highWord(altitude_ft);
    //payload[2] = lowWord(altitude_ft);    
    xbee.send(zbTx);
    delay(1000);
    
    //-------------------------------------------------------
     
    
}

long calculateAltitudeFt(float pressure_kPa) {
    
    float delta;
    long altitude_ft;
    
    delta = KPA_TO_PSIA(pressure_kPa) / INHG_TO_PSIA( NWS_BARO );
    altitude_ft = (1 - pow(delta, (1 / 5.25587611))) / 0.0000068756;
    
    return altitude_ft;
}


float calculateTemperatureC() {
    
    unsigned int uiTadc;
    unsigned char uiTH, uiTL;
    
    unsigned int temperature_counts = 0;
    
    writeRegister(0x22, 0x00);  // Start temperature conversion
    delay(2);                   // Max wait time is 0.7ms, typ 0.6ms
    
    // Read pressure
    uiTH = readRegister(TEMPH);
    uiTL = readRegister(TEMPL);
    
    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;
    
    // Temperature is a 10bit value
    uiTadc = uiTadc >> 6;
    
    // -5.35 counts per °C, 472 counts is 25°C
    return 25 + (uiTadc - 472) / -5.35;
}


float calculatePressurekPa() {
    
    // See Freescale document AN3785 for detailed explanation
    // of this implementation.
    
    signed char sia0MSB, sia0LSB;
    signed char sib1MSB, sib1LSB;
    signed char sib2MSB, sib2LSB;
    signed char sic12MSB, sic12LSB;
    signed char sic11MSB, sic11LSB;
    signed char sic22MSB, sic22LSB;
    signed int sia0, sib1, sib2, sic12, sic11, sic22, siPcomp;
    float decPcomp;
    signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
    signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
    unsigned int uiPadc, uiTadc;
    unsigned char uiPH, uiPL, uiTH, uiTL;
    
    writeRegister(0x24, 0x00);      // Start Both Conversions
    //writeRegister(0x20, 0x00);    // Start Pressure Conversion
    //writeRegister(0x22, 0x00);    // Start temperature conversion
    delay(2);                       // Max wait time is 1ms, typ 0.8ms
    
    // Read pressure
    uiPH = readRegister(PRESH);
    uiPL = readRegister(PRESL);
    uiTH = readRegister(TEMPH);
    uiTL = readRegister(TEMPL);
    
    uiPadc = (unsigned int) uiPH << 8;
    uiPadc += (unsigned int) uiPL & 0x00FF;
    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;
    
    // Placing Coefficients into 16-bit Variables
    // a0
    sia0MSB = readRegister(A0MSB);
    sia0LSB = readRegister(A0LSB);
    sia0 = (signed int) sia0MSB << 8;
    sia0 += (signed int) sia0LSB & 0x00FF;
    
    // b1
    sib1MSB = readRegister(B1MSB);
    sib1LSB = readRegister(B1LSB);
    sib1 = (signed int) sib1MSB << 8;
    sib1 += (signed int) sib1LSB & 0x00FF;
    
    // b2
    sib2MSB = readRegister(B2MSB);
    sib2LSB = readRegister(B2LSB);
    sib2 = (signed int) sib2MSB << 8;
    sib2 += (signed int) sib2LSB & 0x00FF;
    
    // c12
    sic12MSB = readRegister(C12MSB);
    sic12LSB = readRegister(C12LSB);
    sic12 = (signed int) sic12MSB << 8;
    sic12 += (signed int) sic12LSB & 0x00FF;
    
    // c11
    sic11MSB = readRegister(C11MSB);
    sic11LSB = readRegister(C11LSB);
    sic11 = (signed int) sic11MSB << 8;
    sic11 += (signed int) sic11LSB & 0x00FF;
    
    // c22
    sic22MSB = readRegister(C22MSB);
    sic22LSB = readRegister(C22LSB);
    sic22 = (signed int) sic22MSB << 8;
    sic22 += (signed int) sic22LSB & 0x00FF;
    
    // Coefficient 9 equation compensation
    uiPadc = uiPadc >> 6;
    uiTadc = uiTadc >> 6;
    
    // Step 1 c11x1 = c11 * Padc
    lt1 = (signed long) sic11;
    lt2 = (signed long) uiPadc;
    lt3 = lt1*lt2;
    si_c11x1 = (signed long) lt3;
    
    // Step 2 a11 = b1 + c11x1
    lt1 = ((signed long)sib1)<<14;
    lt2 = (signed long) si_c11x1;
    lt3 = lt1 + lt2;
    si_a11 = (signed long)(lt3>>14);
    
    // Step 3 c12x2 = c12 * Tadc
    lt1 = (signed long) sic12;
    lt2 = (signed long) uiTadc;
    lt3 = lt1*lt2;
    si_c12x2 = (signed long)lt3;
    
    // Step 4 a1 = a11 + c12x2
    lt1 = ((signed long)si_a11<<11);
    lt2 = (signed long)si_c12x2;
    lt3 = lt1 + lt2;
    si_a1 = (signed long) lt3>>11;
    
    // Step 5 c22x2 = c22*Tadc
    lt1 = (signed long)sic22;
    lt2 = (signed long)uiTadc;
    lt3 = lt1 * lt2;
    si_c22x2 = (signed long)(lt3);
    
    // Step 6 a2 = b2 + c22x2
    lt1 = ((signed long)sib2<<15);
    lt2 = ((signed long)si_c22x2>1);
    lt3 = lt1+lt2;
    si_a2 = ((signed long)lt3>>16);
    
    // Step 7 a1x1 = a1 * Padc
    lt1 = (signed long)si_a1;
    lt2 = (signed long)uiPadc;
    lt3 = lt1*lt2;
    si_a1x1 = (signed long)(lt3);
    
    // Step 8 y1 = a0 + a1x1
    lt1 = ((signed long)sia0<<10);
    lt2 = (signed long)si_a1x1;
    lt3 = lt1+lt2;
    si_y1 = ((signed long)lt3>>10);
    
    // Step 9 a2x2 = a2 * Tadc
    lt1 = (signed long)si_a2;
    lt2 = (signed long)uiTadc;
    lt3 = lt1*lt2;
    si_a2x2 = (signed long)(lt3);
    
    // Step 10 pComp = y1 + a2x2
    lt1 = ((signed long)si_y1<<10);
    lt2 = (signed long)si_a2x2;
    lt3 = lt1+lt2;
    
    // Fixed point result with rounding
    //siPcomp = ((signed int)lt3>>13);
    siPcomp = lt3/8192;
    
    // decPcomp is defined as a floating point number
    // Conversion to decimal value from 1023 ADC count value
    // ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
    decPcomp = ((65.0/1023.0)*siPcomp)+50;
    
    return decPcomp;
}


unsigned int readRegister(byte thisRegister) {
    
    byte result = 0;
    
    // select the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, LOW);
    
    // send the request
    SPI.transfer(thisRegister | MPL115A1_READ_MASK);
    result = SPI.transfer(0x00);
    
    // deselect the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
    
    return result;  
}


void writeRegister(byte thisRegister, byte thisValue) {
    
    // select the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, LOW);
    
    // send the request
    SPI.transfer(thisRegister & MPL115A1_WRITE_MASK);
    SPI.transfer(thisValue);
    
    // deselect the MPL115A1
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
}

