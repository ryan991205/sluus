#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

#include "ITrafficLight.h"
#include "Communicator.h"
#include "EWaterLockSides.h"
#include "ETrafficLights.h"


class TrafficLight : public ITrafficLight
{
	public:
		TrafficLight(EWaterLockSides side, ETrafficLights light, Communicator& TCP_Con);

		void Green();
		void Red();

	private:
		EWaterLockSides side;
		ETrafficLights light;
		Communicator& communicator;

		std::string GetLightNumber();

		// private copy constructor and assignment operator to prevent making copies
    TrafficLight(const TrafficLight& arg) : communicator(arg.communicator) { /* do nothing */ };
    TrafficLight& operator= (const TrafficLight&) { return *this; };
};

#endif
