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
		Door(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
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
		EWaterLockSides side;
		Communicator& communicator;

		std::string SideAsString();

	private:
		IWaterLockEventGenerator& eventGenerator;
		IValve& lowerValve;
		IValve& middleValve;
		IValve& upperValve;
		ITrafficLight& insideLight;
		ITrafficLight& outsideLight;

		std::thread* pollThread;
		bool continueDoorStatePolling;

		void PollDoorState();

		// private copy constructor and assignment operator to prevent making copies
  	Door& operator= (const Door&) { return *this; };
		Door(const Door& arg) : side(arg.side)
													, communicator(arg.communicator)
													, eventGenerator(arg.eventGenerator)
													, lowerValve(arg.lowerValve)
													, middleValve(arg.middleValve)
													, upperValve(arg.upperValve)
													, insideLight(arg.insideLight)
													, outsideLight(arg.outsideLight)
														{ /* do nothing */ };
	};

#endif
