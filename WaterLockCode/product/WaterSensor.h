#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include "IWaterSensor.h"
#include "EWaterLevels.h"


class WaterSensor : IWaterSensor
{
	public:
		WaterSensor();

		EWaterLevels GetWaterLevel();

	private:
		// private copy constructor and assignment operator to prevent making copies
  	WaterSensor(const WaterSensor&) { /* do nothing */ };
    WaterSensor& operator= (const WaterSensor&) { return *this; };
};

#endif
