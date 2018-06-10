#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include <thread>

#include "IWaterSensor.h"
#include "Communicator.h"
#include "EWaterLevels.h"


class WaterSensor : public IWaterSensor
{
	public:
		WaterSensor(Communicator& TCP_Con);

		EWaterLevels GetWaterLevel();

	private:
		Communicator& communicator;

  	WaterSensor(const WaterSensor& arg) : communicator(arg.communicator) { /* do nothing */ };
    WaterSensor& operator= (const WaterSensor&) { return *this; };
};

#endif
