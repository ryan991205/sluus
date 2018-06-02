#ifndef DOOR_H
#define DOOR_H

#include <thread>

#include "Valve.h"
#include "TrafficLight.h"
#include "EventGenerator.h"
#include "interfaces/IDoor.h"
#include "EWaterLockSides.h"
#include "EValves.h"
#include "EDoorStates.h"
#include "Communicator.h"


class Door : public IDoor
{
	private:
		Valve* lowerValve;
		Valve* middleValve;
		Valve* upperValve;
		TrafficLight* insideLight;
		TrafficLight* outsideLight;
		EventGenerator* eventGenerator;

		std::thread* pollThread;
		bool continueDoorStatePolling;

		void PollDoorState();

	public:
		Door(EWaterLockSides side, EventGenerator* eventGenerator, Communicator* const TCP_Con);
		~Door();

		TrafficLight* GetTrafficLight(ETrafficLights trafficLight);

		void StartPollingDoorSateOnPollThread();
		void KillPollThread();

		Valve* GetValve(EValves valve);

		void Open();

		void Close();

		void Stop();

		EDoorStates GetState();

		// private copy constructor and assignment operator to prevent making copies
    Door(const Door&) { /* do nothing */ };
    Door& operator= (const Door&) { return *this; };

		protected:
			Communicator* communicator;
			EWaterLockSides side;

			std::string sideAsString(EWaterLockSides side);
	};

#endif
