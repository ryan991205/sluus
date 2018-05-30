#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

#include "ITrafficLight.h"
#include "EWaterLockSides.h"
#include "ETrafficLights.h"
#include "Communicator.h"

class TrafficLight : public ITrafficLight
{
	private:
		EWaterLockSides side;
		ETrafficLights light;
		Communicator* communicator;

		std::string GetLightNumber();

	public:
		TrafficLight(EWaterLockSides side, ETrafficLights light, Communicator* const TCP_Con);

		void Green();

		void Red();

		// private copy constructor and assignment operator to prevent making copies
    TrafficLight(const TrafficLight&) { /* do nothing */ };
    TrafficLight& operator= (const TrafficLight&) { return *this; };
};

#endif
