/*
Library for use with Honeywell HIH-4030 Humidity Sensor
Copyright 2010, Jeremy Bartlett, nBit Consulting LLC

temperature compensation formula:
	True RH = (Sensor RH) / (1.0546 - .00216T)  where "T" is degrees celsius
*/

#include "hih4030.h"
#include "WProgram.h"

HIH4030::HIH4030(int pin, float mVolts, float adcResolution)
: SLOPE(30.68), ZERO_OFFSET(958), TEMP_COMP_FIXED_VAL(1.0546), TEMP_COMP_COEFF(0.00216),
pin(pin), mVolts(mVolts), adcResolution(adcResolution)
{
}

HIH4030::~HIH4030(void)
{
}

HIH4030::HIH4030(const HIH4030& hih4030)
: SLOPE(hih4030.SLOPE), ZERO_OFFSET(hih4030.ZERO_OFFSET), 
TEMP_COMP_FIXED_VAL(hih4030.TEMP_COMP_FIXED_VAL), TEMP_COMP_COEFF(hih4030.TEMP_COMP_COEFF)
{
	
}

float HIH4030::getRh(int temperature)
{
	float rh = getSensorRh();
	float trueRh = rh / (TEMP_COMP_FIXED_VAL - (TEMP_COMP_COEFF * temperature));
	return trueRh;
}

float HIH4030::getSensorRh()
{
	float raw = (float)getRawValue();
	float mv = transposeADCToMV(raw);
#if HIH4030_SERIAL_DEBUGGING > 0
	Serial.print("transposed val:");
	Serial.println(mv);
#endif
	mv -= ZERO_OFFSET;
#if HIH4030_SERIAL_DEBUGGING > 0
	Serial.print("transposed val w/ zero offset:");
	Serial.println(mv);
#endif
	float val = mv / SLOPE;
#if HIH4030_SERIAL_DEBUGGING > 0
	Serial.print("getSensorRh(): ");
	Serial.println(val);
#endif
	return val;
}

float HIH4030::transposeADCToMV(float val)
{
	float mv = val * (mVolts / adcResolution);
	return mv;
}

int HIH4030::getRawValue()
{
	int val =  analogRead(pin);
#if HIH4030_SERIAL_DEBUGGING > 0
	Serial.print("getRawValue(): ");
	Serial.println(val);
#endif
	return val;
}
