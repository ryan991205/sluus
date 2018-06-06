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
	public:
		Door(EWaterLockSides side, IWaterLockEventGenerator& _eventGenerator, Communicator& TCP_Con,
				 IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 ITrafficLight& _outsideLight);
		~Door();

		ITrafficLight* GetTrafficLight(ETrafficLights trafficLight);
		IValve* GetValve(EValves valve);

		void Open();
		void Close();
		void Stop();

		EDoorStates GetState();

		void StartPollingDoorSateOnPollThread();
		void KillPollThread();

	protected:
		Communicator& communicator;
		EWaterLockSides side;

		std::string SideAsString();

		//protected copy constructor to prevent making copies
		Door(const Door& arg) : side(arg.side), eventGenerator(arg.eventGenerator), communicator(arg.communicator),
														lowerValve(arg.lowerValve), middleValve(arg.middleValve),
														upperValve(arg.upperValve), insideLight(arg.insideLight),
														outsideLight(arg.outsideLight)
														{ /* do nothing */ };

	private:
		IValve& lowerValve;
		IValve& middleValve;
		IValve& upperValve;
		ITrafficLight& insideLight;
		ITrafficLight& outsideLight;
		IWaterLockEventGenerator& eventGenerator;

		std::thread* pollThread;
		bool continueDoorStatePolling;

		void PollDoorState();

		// private assignment operator to prevent making copies
  	Door& operator= (const Door&) { return *this; };
	};

#endif
