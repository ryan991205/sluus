#ifndef DOOR_H
#define DOOR_H

#include <stdio.h> // for NULL, remove when we have a diffrent lib

#include "Valve.h"
#include "TrafficLight.h"
#include "interfaces/IDoor.h"
#include "EWaterLockSides.h"
#include "EValves.h"
#include "EDoorStates.h"


class Door : public IDoor
{
	private:
		EWaterLockSides side;
		Valve* lowerValve;
		Valve* middleValve;
		Valve* upperValve;
		TrafficLight* insideLight;
		TrafficLight* outsideLight;

	public:
		Door(EWaterLockSides side);
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
