#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "ITrafficLight.h"
#include "EWaterLockSides.h"
#include "ETrafficLights.h"


class TrafficLight : public ITrafficLight
{
	private:
		EWaterLockSides side;
		ETrafficLights light;

	public:
		TrafficLight(EWaterLockSides side, ETrafficLights light);

		void Green();

		void Red();

		// private copy constructor and assignment operator to prevent making copies
    TrafficLight(const TrafficLight&) { /* do nothing */ };
    TrafficLight& operator= (const TrafficLight&) { return *this; };
};

#endif
