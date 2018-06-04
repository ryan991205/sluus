#ifndef DOOR_H
#define DOOR_H

#include <thread>

#include "IValve.h"
#include "ITrafficLight.h"
#include "IWaterLockEventGenerator.h"
#include "IDoor.h"
#include "Communicator.h"
#include "EWaterLockSides.h"
#include "EValves.h"
#include "ETrafficLights.h"
#include "EDoorStates.h"


class Door : public IDoor
{
	private:
		IValve* lowerValve;
		IValve* middleValve;
		IValve* upperValve;
		ITrafficLight* insideLight;
		ITrafficLight* outsideLight;
		IWaterLockEventGenerator* eventGenerator;

		std::thread* pollThread;
		bool continueDoorStatePolling;

		void PollDoorState();

	public:
		Door(EWaterLockSides side, IWaterLockEventGenerator* eventGenerator, Communicator* const TCP_Con);
		~Door();

		ITrafficLight* GetTrafficLight(ETrafficLights trafficLight);
		IValve* GetValve(EValves valve);

		void StartPollingDoorSateOnPollThread();
		void KillPollThread();

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

			std::string SideAsString();
	};

#endif
