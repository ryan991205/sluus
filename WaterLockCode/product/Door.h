#ifndef DOOR_H
#define DOOR_H

#include <stdio.h> // for NULL, remove when we have a diffrent lib

#include "Valve.h"
#include "TrafficLight.h"
#include "interfaces/IDoor.h"
#include "EWaterLockSides.h"
#include "EValves.h"
#include "EDoorStates.h"
#include "Communicator.h"


class Door : public IDoor
{
	private:
		EWaterLockSides side;
		Valve* lowerValve;
		Valve* middleValve;
		Valve* upperValve;
		TrafficLight* insideLight;
		TrafficLight* outsideLight;
		Communicator* communicator;

		std::string sideAsString(EWaterLockSides side);

	public:
		Door(EWaterLockSides side, Communicator* const TCP_Con);
		~Door();

		TrafficLight* GetTrafficLight(ETrafficLights trafficLight);

		Valve* GetValve(EValves valve);

		void Open();

		void Close();

		void Stop();

		EDoorStates GetState();

		// private copy constructor and assignment operator to prevent making copies
    Door(const Door&) { /* do nothing */ };
    Door& operator= (const Door&) { return *this; };
	};

#endif
