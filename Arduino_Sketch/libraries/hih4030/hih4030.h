/*
Library for use with Honeywell HIH-4030 Humidity Sensor
Copyright 2010, Jeremy Bartlett, nBit Consulting LLC

temperature compensation formula:
	True RH = (Sensor RH) / (1.0546 - .00216T)  where "T" is degrees celsius
*/

#ifndef HIH_4030_H
#define HIH_4030_H

#include "WProgram.h"

#ifndef HIH4030_SERIAL_DEBUGGING
#define HIH4030_SERIAL_DEBUGGING 0
#endif

class HIH4030
{
	public:
		HIH4030(int, float, float);
		HIH4030(const HIH4030&);
		~HIH4030(void);
		float getRh(int);
	private:
		const float SLOPE;
		const float ZERO_OFFSET;
		const float TEMP_COMP_FIXED_VAL;
		const float TEMP_COMP_COEFF;
		float mVolts;
		float adcResolution;
		float transposeADCToMV(float);
		float getSensorRh();
		int getRawValue();
		int pin;
};



#endif
