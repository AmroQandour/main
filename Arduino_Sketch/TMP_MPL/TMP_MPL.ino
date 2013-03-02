#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 3, 6, 8, 5, 4);

int tmp102Address = 0x48;

#define NWS_BARO 30.13

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

void setup() {
    
    // initialize serial i/o
    Serial.begin(9600);
    
    // initialize SPI interface
    SPI.begin();
    Wire.begin();
    lcd.begin(16,2); 
    
    pinMode(MPL115A1_SELECT_PIN, OUTPUT);
    pinMode(MPL115A1_ENABLE_PIN, OUTPUT);
    pinMode(A3, INPUT);
    pinMode(A0, INPUT);
    
    // sleep the MPL115A1
    digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    // set the chip select inactive, select signal is CS LOW
    digitalWrite(MPL115A1_SELECT_PIN, HIGH);
    
   /* // spam welcome banner
    Serial.print("\n************************************************************************************\n");
    Serial.print("* Using Altimiter Setting: ");
    Serial.print(NWS_BARO, 2);
    Serial.print(" in Hg                                             *\n");
    Serial.print("* Visit http://www.weather.gov for your current local value.                       *\n");
    Serial.print("************************************************************************************\n"); */ 
    
}

void loop(){
  
  float celsius = getTemperature();
//  Serial.print("Celsius: ");
//  Serial.println(celsius);

  float fahrenheit = (1.8 * celsius) + 32;
//  Serial.print("Fahrenheit: ");
//  Serial.println(fahrenheit);
  lcd.cursor();
  
  lcd.print("TMP102_Sensor"); delay(2000); lcd.clear();   
  lcd.print(celsius); lcd.print(" C"); 
  lcd.setCursor(0,1);
  lcd.print(fahrenheit); lcd.print(" F"); 
  

  delay(5000); //just here to slow down the output. You can remove this
  
  lcd.clear();
  
  float pressure_pKa = 0;
  float temperature_c= 0;
  long altitude_ft = 0;
    
    // wake the MPL115A1
    digitalWrite(MPL115A1_ENABLE_PIN, HIGH);
    delay(20);  // give the chip a few ms to wake up
    
    pressure_pKa = calculatePressurekPa();
    temperature_c = calculateTemperatureC();
    altitude_ft = calculateAltitudeFt(pressure_pKa);
    
    // put the MPL115A1 to sleep, it has this feature why not use it
    // while in shutdown the part draws ~1uA
    digitalWrite(MPL115A1_ENABLE_PIN, LOW);
    
    // print table of altitude, pressures, and temperatures to console
    lcd.print("MPL1151A_Sens"); delay(2000);  lcd.clear(); 
//    Serial.print(altitude_ft);
    lcd.print("Altitude(Feet)= "); lcd.setCursor(0,1); lcd.print(altitude_ft); delay(3000); lcd.clear(); 
//    Serial.print(" ft | ");
//    Serial.print(FT_TO_M(altitude_ft));
    lcd.print("Altitude(Meters)= "); lcd.setCursor(0,1); lcd.print(FT_TO_M(altitude_ft)); delay(3000); lcd.clear(); 
//    Serial.print(" m | ");
//    Serial.print(KPA_TO_INHG(pressure_pKa), 2);
    lcd.print("HG= "); lcd.print(KPA_TO_INHG(pressure_pKa),2); delay(3000); lcd.clear(); 
//    Serial.print(" in Hg | ");
//    Serial.print(KPA_TO_MMHG(pressure_pKa), 0);
    lcd.print("mmHg= "); lcd.print(KPA_TO_MMHG(pressure_pKa),0); delay(3000); lcd.clear(); 
//    Serial.print(" mm Hg | ");
//    Serial.print(KPA_TO_PSIA(pressure_pKa), 2);
    lcd.print("PSI= "); lcd.print(KPA_TO_PSIA(pressure_pKa), 2); delay(3000); lcd.clear(); 
//    Serial.print(" psia | ");
    //Serial.print(KPA_TO_KGCM2(pressure_pKa), 3);
    ///Serial.print(" kg/cm2 | ");
//    Serial.print(pressure_pKa, 1);
    lcd.print("kPa= "); lcd.print(pressure_pKa, 1); delay(3000); lcd.clear(); 
//    Serial.print(" kPa | ");
    
    // At a res of -5.35 counts/°C, digits lower than 0.1°C are not significant
//    Serial.print(temperature_c, 1);
    lcd.print("Tc= "); lcd.print(temperature_c, 1); delay(3000); lcd.clear(); 
//    Serial.print(" C | ");
//    Serial.print(DEGC_TO_DEGF(temperature_c), 1);
    lcd.print("Tf= "); lcd.print(DEGC_TO_DEGF(temperature_c), 1); delay(3000); lcd.clear(); 
//    Serial.print(" F\n");
    
    // wait a few seconds before looping
    delay(5000);
    
    float sens=0; 
    float sens_value=0; 
    sens = analogRead(A3);
    sens_value= ((0.0004*celsius + 0.149)*sens)-(0.0617*celsius + 24.436);
    lcd.print("HIH-4030"); delay(2000); lcd.clear();   
    lcd.print("ADC= "); lcd.print(sens); 
    lcd.setCursor(0, 1);
    lcd.print("RH%= "); lcd.print(sens_value);
    delay(5000); 
    lcd.clear();
    
    
    float gas=0;
    gas = (5*analogRead(A0))/1023;
    lcd.print("Gas MQ-6"); delay(2000); lcd.clear();   
    lcd.print("G.Input= "); lcd.print(gas); 
    delay(5000); 
    lcd.clear();
}

float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}

long calculateAltitudeFt(float pressure_kPa) {
    
    float delta;
    long altitude_ft;
    
    
    // See http://en.wikipedia.org/wiki/Barometric_formula
    // If you're a pilot you need to know what's going on here,
    // otherwise just know these steps calculate the barometric altitude (ft)
    // based on the ratio of absolute barometric pressure (psia) to the
    // altimiter setting (psia).
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
    
