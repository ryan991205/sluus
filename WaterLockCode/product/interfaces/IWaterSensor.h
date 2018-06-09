#ifndef IWATERSENSOR_H
#define IWATERSENSOR_H

#include "EWaterLevels.h"


class IWaterSensor
{
	public:
		virtual ~IWaterSensor() {};

		virtual EWaterLevels GetWaterLevel() = 0;
};

#endif
