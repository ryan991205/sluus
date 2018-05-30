#ifndef WATERLOCK_H
#define WATERLOCK_H

#include <thread>

#include "Door.h"
#include "RepeatingDoor.h"
#include "LockableDoor.h"
#include "EventGenerator.h"
#include "WaterSensor.h"
#include "Communicator.h"
#include "EEvents.h"
#include "EStates.h"
#include "EDoorTypes.h"


class WaterLock
{
	public:
		WaterLock(EDoorTypes lowWaterDoor, EWaterLockSides lowWaterDoorSide, EDoorTypes highWaterDoor, int port);
		~WaterLock();

		EventGenerator* GetEventGenerator();

	private:
		Door* openDoor;
		Door* lowWaterDoor;
		Door* highWaterDoor;
		WaterSensor* waterSensor;
		Communicator* communicator;

		///// Sate Machine Members /////
		EventGenerator eventGenerator;
		EStates state;
		ENormalOperationSubStates normalOperationSubState;
		EOneDoorOpenSubStates oneDoorOpenSubState;
		EBothDoorsClosedSubStates bothDoorsClosedSubState;
		///// Sate Machine Members /////

		bool continueEventPolling;
		std::thread* pollThread;

		void StartPollingEventsOnPollThread();
		void PollEvents();
		void KillPollThread();

		void RaiseWater(EWaterLevels waterLevel);
		void LowerWater();

		///// Sate Machine Functions /////
		void HandleEvent(EEvents ev);
		void HandleNormalOperationState(EEvents ev);
		void HandleEmergencyState(EEvents ev);
		void HandleOneDoorOpenStates(EEvents ev);
		void HandleBothDoorsClosedState(EEvents ev);
		void HandleOpeningState(EEvents ev);
		void HandleClosingState(EEvents ev);
		void HandleOpenState(EEvents ev);
		void HandleOutsideGreenLitState(EEvents ev);
		void HandleInsideGreenLitState(EEvents ev);
		void HandleRaisingWaterState(EEvents ev);
		void HandleLoweringWaterState(EEvents ev);

		void Entry_NormalOperationState();
		void Exit_NormalOperationState();

		void Entry_EmergencyState();
		void Exit_EmergencyState();

		void Entry_OneDoorOpenState();
		void Exit_OneDoorOpenState();

		void Entry_BothDoorsClosedState();
		void Exit_BothDoorsClosedState();

		void Entry_OpeningState();
		void Exit_OpeningState();

		void Entry_ClosingState();
		void Exit_ClosingState();

		void Entry_OpenState();
		void Exit_OpenState();

		void Entry_OutsideGreenLitState();
		void Exit_OutsideGreenLitState();

		void Entry_InsideGreenLitState();
		void Exit_InsideGreenLitState();

		void Entry_RaisingWaterState();
		void Exit_RaisingWaterState();

		void Entry_LoweringWaterState();
		void Exit_LoweringWaterState();
		///// Sate Machine Functions /////

		void StopDoors();
		void CloseLowWaterDoorValves();
		void CloseHighWaterDoorValves();
		void CloseAllValves();
		void TrafficLightsRed();

		// private copy constructor and assignment operator to prevent making copies
  	WaterLock(const WaterLock&) { /* do nothing */ };
    WaterLock& operator= (const WaterLock&) { return *this; };
};

#endif
