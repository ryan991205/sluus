#ifndef IWATERLOCKEVENTGENERATOR_H
#define IWATERLOCKEVENTGENERATOR_H


class IWaterLockEventGenerator
{
	public:
		virtual ~IWaterLockEventGenerator() {};

		virtual void WaterLevelChanged() = 0;

		virtual void DoorStateChanged() = 0;

		virtual void Emergency() = 0;
};

#endif
