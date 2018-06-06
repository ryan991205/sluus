#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>

#include "ITrafficLight.h"
#include "Communicator.h"
#include "EWaterLockSides.h"
#include "ETrafficLights.h"
//#include "ETrafficLightColors.h"

class TrafficLight : public ITrafficLight
{
	public:
		TrafficLight(EWaterLockSides side, ETrafficLights light, Communicator* const TCP_Con);

		void Green();

		void Red();

		//ETrafficLightColors GetColor();

	private:
		EWaterLockSides side;
		ETrafficLights light;
		Communicator* communicator;

		std::string GetLightNumber();

		// private copy constructor and assignment operator to prevent making copies
    TrafficLight(const TrafficLight&) { /* do nothing */ };
    TrafficLight& operator= (const TrafficLight&) { return *this; };
};

#endif
