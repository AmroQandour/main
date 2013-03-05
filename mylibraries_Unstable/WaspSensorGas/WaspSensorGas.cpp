/*
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
   
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *	Board:			Waspmote Sensor Gas board Version (First edition non-pro version). 
 *  Version:		0.1
 *	Implementation: Amro Qandour
 *  Based on the original Waspmote Gas Board Library by Libelium (http://www.libelium.com)
 *	
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
 
#include "WaspSensorGas.h"
#include "Wire.h"


// Constructors ////////////////////////////////////////////////////////////////

WaspSensorGas::WaspSensorGas()
{
	pinMode(13,OUTPUT);
	pinMode(12,OUTPUT);
	pinMode(11,OUTPUT);
	pinMode(10,OUTPUT);//SENS_PW_3V3
	pinMode(9,OUTPUT); //SENS_PW_5V
	pinMode(8,OUTPUT);	
	pinMode(7,OUTPUT);	
	
	digitalWrite(13,LOW);
	digitalWrite(12,LOW);
	digitalWrite(11,LOW);
	digitalWrite(10,LOW);
	digitalWrite(9,LOW);
	digitalWrite(8,LOW);
	digitalWrite(7,LOW);
}

// Public Methods //////////////////////////////////////////////////////////////

void	WaspSensorGas::setBoardMode(uint8_t mode)
{
	switch( mode )
	{
		case	SENS_ON :	digitalWrite(9,HIGH); //Choose digital pin to supply 5 Vols to the board. 
					
					break;
		case	SENS_OFF:	digitalWrite(9,LOW);	//
					break;
	}
}

void	WaspSensorGas::configureSensor(uint16_t sensor, uint8_t gain)
{
	configureSensor(sensor,gain,0);
}

void	WaspSensorGas::configureSensor(uint16_t sensor, uint8_t gain, float resistor)
{
	switch( sensor )
	{
		case	SENS_SOCKET2B	:	configureResistor(SENS_R1,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI2,gain);
						break;
		case	SENS_SOCKET3C	:	configureResistor(SENS_R2,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI3,gain);
						break;
		case	SENS_SOCKET4A	:	configureResistor(SENS_R3,resistor);
						delay(DELAY_TIME);
						configureAmplifier(SENS_AMPLI4,gain);
						break;
	}
}

void	WaspSensorGas::setSensorMode(uint8_t mode, uint16_t sensor)
{
	if( mode==SENS_ON )
	{
		switch( sensor )
		{
			case	SENS_SOCKET2B:		
							digitalWrite(13,HIGH);
							break;
			case	SENS_SOCKET4A	:	digitalWrite(12,HIGH);
							digitalWrite(11,HIGH);
							break;
		}
	}
	
	if( mode==SENS_OFF )
	{
		switch( sensor )
		{
			case	SENS_SOCKET2B:		digitalWrite(13,LOW);
							break;
			case	SENS_SOCKET4A	:	digitalWrite(12,LOW);
							digitalWrite(11,LOW);
							break;
		}
	}
}

float	WaspSensorGas::readValue(uint16_t sensor)
{
	uint16_t aux=0;
	switch( sensor )
	{
		case	SENS_TEMPERATURE	:	aux=analogRead(A0);
							break;
		case	SENS_SOCKET2B		:	aux=analogRead(A1);
							break;
		case	SENS_SOCKET3C		:	aux=pulse(SENS_SOCKET3C);
							break;
		case	SENS_SOCKET4A		:	aux=analogRead(A3);
							break;
	}
	return	(aux);
}

// Private Methods //////////////////////////////////////////////////////////////

void WaspSensorGas::configureResistor(uint8_t ampli, float resistor)
{
	switch( ampli )
	{
		case	SENS_R1	:	setResistor(B0101100,resistor);
					break;
		case	SENS_R2	:	setResistor(B0101110,resistor);
					break;
		case	SENS_R3	:	setResistor(B0101010,resistor);
					break;
	}
}

void WaspSensorGas::setResistor(uint8_t address, float value)
{
	float auxiliar = 0;
	uint8_t resist=0;
	
	auxiliar = 128*value;
	auxiliar = auxiliar/100;
	resist = (uint8_t) 128-auxiliar;

	Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.write(B00000000);
	Wire.write(resist);
	Wire.endTransmission();
	delay(DELAY_TIME);
}

void WaspSensorGas::configureAmplifier(uint8_t ampli, uint8_t gain)
{
	switch( ampli )
	{
		case	SENS_AMPLI1	:	setAmplifier(B0101000,gain);
						break;
		case	SENS_AMPLI2	:	setAmplifier(B0101100,gain);
						break;
		case	SENS_AMPLI3	:	setAmplifier(B0101110,gain);
						break;
		case	SENS_AMPLI4	:	setAmplifier(B0101010,gain);
						break;
	}
}

void WaspSensorGas::setAmplifier(uint8_t address, uint8_t value)
{
	uint8_t ampli=0;
	value--;

	ampli=(uint8_t) 128-(128/100)*value;
	Wire.begin();
	delay(100);
	Wire.beginTransmission(address);
	Wire.write(B00010000);
	Wire.write(ampli);
	Wire.endTransmission();
	delay(DELAY_TIME);
}

uint16_t WaspSensorGas::pulse(uint16_t sensor)
{
	uint16_t aux=0;
	
	switch( sensor )
	{
		case	SENS_SOCKET3C	:	digitalWrite(7, HIGH); 
  						delay(2);
						digitalWrite(8, HIGH);
						delay(4);
						aux = analogRead(A2);
						delay(1);
						digitalWrite(8, LOW);
						delay(7); 
						digitalWrite(7, LOW);
						delay(236);
						break;
	}
	return aux;
}

WaspSensorGas SensorGas=WaspSensorGas();
