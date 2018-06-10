#ifndef IWATERLOCKEVENTGENERATOR_H
#define IWATERLOCKEVENTGENERATOR_H

#include "EEvents.h"


class IWaterLockEventGenerator
{
	public:
		virtual ~IWaterLockEventGenerator() {};

		virtual EEvents GetEvent() = 0;

		virtual void WaterLevelChanged() = 0;

		virtual void DoorStateChanged() = 0;

		virtual void Emergency() = 0;
};

#endif
