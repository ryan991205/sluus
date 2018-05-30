#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include "IWaterSensor.h"
#include "EWaterLevels.h"
#include "Communicator.h"


class WaterSensor : IWaterSensor
{
	public:
		WaterSensor(Communicator* const TCP_Con);

		EWaterLevels GetWaterLevel();

	private:
	Communicator* communicator; 
		// private copy constructor and assignment operator to prevent making copies
  	WaterSensor(const WaterSensor&) { /* do nothing */ };
    WaterSensor& operator= (const WaterSensor&) { return *this; };
};

#endif
