#include "WaterLock.h"

#include <stdexcept>

#include "EValves.h"

#include <iostream> // Note: debug


// Note: exceptions = logic_error
WaterLock::WaterLock(EDoorTypes lowWaterDoor, EWaterLockSides lowWaterDoorSide, EDoorTypes highWaterDoor, int port)
{
	std::cout << "WaterLock::WaterLock(): Hello!" << std::endl;

	communicator = new Communicator(port);

	waterSensor = new WaterSensor(&eventGenerator, communicator);

	switch(lowWaterDoor)
	{
		case Normal    : this->lowWaterDoor = new Door(lowWaterDoorSide, &eventGenerator, communicator);          break;
		case Repeating : this->lowWaterDoor = new RepeatingDoor(lowWaterDoorSide, &eventGenerator, communicator); break;
		case Lockable  : this->lowWaterDoor = new LockableDoor(lowWaterDoorSide, &eventGenerator, communicator);  break;
		default        : throw std::logic_error("WaterLock::WaterLock(): lowWaterDoor == unsuported door");     												  break;
	}

	EWaterLockSides highWaterDoorSide;
	if(lowWaterDoorSide == Left)
	{
		highWaterDoorSide = Right;
	}
	else
	{
		highWaterDoorSide = Left;
	}

	switch(highWaterDoor)
	{
		case Normal    : this->highWaterDoor = new Door(highWaterDoorSide, &eventGenerator, communicator);          break;
		case Repeating : this->highWaterDoor = new RepeatingDoor(highWaterDoorSide, &eventGenerator, communicator); break;
		case Lockable  : this->highWaterDoor = new LockableDoor(highWaterDoorSide, &eventGenerator, communicator);  break;
		default        : throw std::logic_error("WaterLock::WaterLock(): highWaterDoor == unsuported door");     												    break;
	}

	EDoorStates lowWatDoor = this->lowWaterDoor->GetState();
	EDoorStates highWatDoor = this->highWaterDoor->GetState();

	if((lowWatDoor == DoorOpen) && (highWatDoor == DoorOpen))
	{
		throw std::logic_error("WaterLock::WaterLock(): both doors are open");
	}
	else if(lowWatDoor == DoorOpen)
	{
		openDoor = this->lowWaterDoor;
	}
	else if(highWatDoor == DoorOpen)
	{
		openDoor = this->highWaterDoor;
	}
	else
	{
		openDoor = nullptr;
	}

	state = ST_NormalOperation;
	Entry_NormalOperationState();

	pollThread = nullptr;
	StartPollingEventsOnPollThread();
}

WaterLock::~WaterLock()
{
	KillPollThread();
	delete waterSensor;
	delete lowWaterDoor;
	delete highWaterDoor;
	delete communicator;
}

EventGenerator* WaterLock::GetEventGenerator()
{
	return &eventGenerator;
}

void WaterLock::RaiseWater(EWaterLevels waterLevel)
{
	switch(waterLevel)
	{
		case Low              : highWaterDoor->GetValve(Lower)->Open();  break;
		case BelowMiddleValve : highWaterDoor->GetValve(Lower)->Open();  break;
		case AboveMiddleValve : highWaterDoor->GetValve(Middle)->Open(); break;
		case AboveUpperValve  : highWaterDoor->GetValve(Upper)->Open();  break;
		default               : throw std::logic_error("WaterLock::RaiseWater(): waterLevel == unsuported water level"); break;
	}
}

void WaterLock::LowerWater()
{
	lowWaterDoor->GetValve(Lower)->Open();
}

void WaterLock::StartPollingEventsOnPollThread()
{
	KillPollThread();

	pollThread = new std::thread(&WaterLock::PollEvents, this);
}

void WaterLock::PollEvents()
{
	continueEventPolling = true;
	while(continueEventPolling)
	{
		std::cout << "WaterLock::PollEvents(): loop!" << std::endl; // Note: debug

		HandleEvent(eventGenerator.GetEvent());
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void WaterLock::KillPollThread()
{
	if(pollThread != nullptr)
	{
		if(pollThread->joinable())
		{
			continueEventPolling = false;
			pollThread->join();
		}

		delete pollThread;
		pollThread = nullptr;
	}
}

void WaterLock::HandleEvent(EEvents ev)
{
	if(ev == EV_NoEvent)
	{
		return; //Note: no new event
	}

	switch(state)
	{
		case ST_NormalOperation : HandleNormalOperationState(ev); break;
		case ST_Emergency 			: HandleEmergencyState(ev); 			break;
		default : break;
	}
}

void WaterLock::HandleNormalOperationState(EEvents ev)
{
	if((ev == EV_Emergency) || (ev == EV_EmergencyButtonPressed))
	{
		if(normalOperationSubState == ST_OneDoorOpen)
		{
			switch(oneDoorOpenSubState)
			{
				case ST_Opening         : Exit_OpeningState();         break;
				case ST_Closing         : Exit_ClosingState();         break;
				case ST_Open            : Exit_OpenState();            break;
				case ST_OutsideGreenLit : Exit_OutsideGreenLitState(); break;
				case ST_InsideGreenLit  : Exit_InsideGreenLitState();  break;
				default : throw std::logic_error("WaterLock::HandleNormalOperationState(): oneDoorOpenSubState == unsupported state"); break;
			}

			Exit_OneDoorOpenState();
		}
		else if(normalOperationSubState == ST_BothDoorsClosed)
		{
			switch(bothDoorsClosedSubState)
			{
				case ST_LoweringWater : Exit_LoweringWaterState(); break;
				case ST_RaisingWater  : Exit_RaisingWaterState();  break;
				default : throw std::logic_error("WaterLock::HandleNormalOperationState(): bothDoorsClosedSubState == unsuported state"); break;
			}

			Exit_NormalOperationState();
		}

		Exit_NormalOperationState();
		state = ST_Emergency;
		Entry_EmergencyState();
		return;
	}

	switch(normalOperationSubState)
	{
		case ST_OneDoorOpen     : HandleOneDoorOpenStates(ev);    break;
		case ST_BothDoorsClosed : HandleBothDoorsClosedState(ev); break;
		default : throw std::logic_error("WaterLock::HandleNormalOperationState(): normalOperationSubState == unsuported state"); break;
	}
}

void WaterLock::HandleEmergencyState(EEvents ev)
{
	if(ev == EV_ResumeNormalOperationsButtonPressed)
	{
		Exit_EmergencyState();
		state = ST_NormalOperation;
		Entry_NormalOperationState();
	}
}

void WaterLock::HandleOneDoorOpenStates(EEvents ev)
{
	switch(oneDoorOpenSubState)
	{
		case ST_Opening         : HandleOpeningState(ev);         break;
		case ST_Closing         : HandleClosingState(ev);         break;
		case ST_Open            : HandleOpenState(ev);            break;
		case ST_OutsideGreenLit : HandleOutsideGreenLitState(ev); break;
		case ST_InsideGreenLit  : HandleInsideGreenLitState(ev);  break;
		default : throw std::logic_error("WaterLock::HandleOneDoorOpenStates(): oneDoorOpenSubState == unsuported state"); break;
	}
}

void WaterLock::HandleBothDoorsClosedState(EEvents ev)
{
	switch(ev)
	{
		case ST_RaisingWater  : HandleRaisingWaterState(ev);  break;
		case ST_LoweringWater : HandleLoweringWaterState(ev); break;
		default : throw std::logic_error("WaterLock::HandleBothDoorsClosedState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleOpeningState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_OpeningState();
			oneDoorOpenSubState = ST_Closing;
			Entry_ClosingState();
			break;
		case EV_DoorStateChanged :
			if(openDoor->GetState() == DoorOpen)
			{
				Exit_OpeningState();
				openDoor->KillPollThread();
				oneDoorOpenSubState = ST_Open;
				Entry_OpeningState();
			}
			break;
		default : throw std::logic_error("WaterLock::HandleOpeningState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleClosingState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_ClosingState();
			oneDoorOpenSubState = ST_Closing;
			Entry_OpeningState();
			break;
		case EV_DoorStateChanged :
			if(openDoor->GetState() == DoorClosed)
			{
				Exit_ClosingState();
				openDoor->KillPollThread();
				Exit_OneDoorOpenState();
				normalOperationSubState = ST_BothDoorsClosed;
				Entry_BothDoorsClosedState();
			}
			break;
		default : throw std::logic_error("WaterLock::HandleClosingState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleOpenState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_OpenState();
			oneDoorOpenSubState = ST_Closing;
			Entry_ClosingState();
			break;
		case EV_ReleaseOutsideButtonPressed :
			Exit_OpenState();
			oneDoorOpenSubState = ST_OutsideGreenLit;
			Entry_OutsideGreenLitState();
			break;
		case EV_ReleaseInsideButtonPressed :
			Exit_OpenState();
			oneDoorOpenSubState = ST_InsideGreenLit;
			Entry_InsideGreenLitState();
			break;
		default : throw std::logic_error("WaterLock::HandleOpenState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleOutsideGreenLitState(EEvents ev)
{
	switch(ev)
	{
		case EV_ReleaseOutsideButtonPressed :
			Exit_OutsideGreenLitState();
			oneDoorOpenSubState = ST_Open;
			Entry_OpenState();
			break;
		case EV_ReleaseInsideButtonPressed :
			Exit_OutsideGreenLitState();
			oneDoorOpenSubState = ST_InsideGreenLit;
			Entry_InsideGreenLitState();
			break;
		default : throw std::logic_error("WaterLock::HandleOutsideGreenLitState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleInsideGreenLitState(EEvents ev)
{
	switch(ev)
	{
		case EV_ReleaseOutsideButtonPressed :
			Exit_InsideGreenLitState();
			oneDoorOpenSubState = ST_InsideGreenLit;
			Entry_OutsideGreenLitState();
			break;
		case EV_ReleaseInsideButtonPressed :
			Exit_InsideGreenLitState();
			oneDoorOpenSubState = ST_Open;
			Entry_OpenState();
			break;
		default : throw std::logic_error("WaterLock::HandleInsideGreenLitState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleRaisingWaterState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_RaisingWaterState();
			bothDoorsClosedSubState = ST_LoweringWater;
			Entry_LoweringWaterState();
			break;
		case EV_WaterLevelChanged :
			Exit_RaisingWaterState();

			if(waterSensor->GetWaterLevel() == High)
			{
				CloseHighWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = highWaterDoor;
				openDoor->StartPollingDoorSateOnPollThread();
				Entry_OneDoorOpenState();
			}
			else
			{
				Entry_RaisingWaterState();
			}
			break;
		default : throw std::logic_error("WaterLock::HandleRaisingWaterState(): ev == unsuported event"); break;
	}
}

void WaterLock::HandleLoweringWaterState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_LoweringWaterState();
			bothDoorsClosedSubState = ST_RaisingWater;
			Entry_RaisingWaterState();
			break;
		case EV_WaterLevelChanged :
			Exit_LoweringWaterState();

			if(waterSensor->GetWaterLevel() == Low)
			{
				CloseLowWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = lowWaterDoor;
				openDoor->StartPollingDoorSateOnPollThread();
				Entry_OneDoorOpenState();
			}
			else
			{
				Entry_LoweringWaterState();
			}
			break;
		default : throw std::logic_error("WaterLock::HandleLoweringWaterState(): ev == unsuported event"); break;
	}
}

void WaterLock::Entry_NormalOperationState()
{
	// TODO: return to deep history state

	if(openDoor == nullptr)
	{
		normalOperationSubState = ST_BothDoorsClosed;
		Entry_BothDoorsClosedState();
	}
	else
	{
		normalOperationSubState = ST_OneDoorOpen;
		Entry_OneDoorOpenState();
	}
}

void WaterLock::Exit_NormalOperationState()
{
	// TODO: save deep history
}

void WaterLock::Entry_EmergencyState()
{
	StopDoors();
	CloseAllValves();
	TrafficLightsRed();
}

void WaterLock::WaterLock::Exit_EmergencyState()
{
	// NOTE: no functionality
}

void WaterLock::Entry_OneDoorOpenState()
{
	oneDoorOpenSubState = ST_Opening;
	Entry_OpeningState();
}

void WaterLock::Exit_OneDoorOpenState()
{
	openDoor = nullptr;
}

void WaterLock::Entry_BothDoorsClosedState()
{
	waterSensor->StartPollingWaterLevelOnPollThread();

	if(waterSensor->GetWaterLevel() == High)
	{
		bothDoorsClosedSubState = ST_LoweringWater;
		Entry_LoweringWaterState();
	}
	else
	{
		bothDoorsClosedSubState = ST_RaisingWater;
		Entry_RaisingWaterState();
	}
}

void WaterLock::Exit_BothDoorsClosedState()
{
	waterSensor->KillPollThread();
}

void WaterLock::Entry_OpeningState()
{
	openDoor->Open();
}

void WaterLock::Exit_OpeningState()
{
	openDoor->Stop();
}

void WaterLock::Entry_ClosingState()
{
	openDoor->Close();
}

void WaterLock::Exit_ClosingState()
{
	openDoor->Stop();
}

void WaterLock::Entry_OpenState()
{
	// NOTE: no functionality
}

void WaterLock::Exit_OpenState()
{
	// NOTE: no functionality
}

void WaterLock::Entry_OutsideGreenLitState()
{
	openDoor->GetTrafficLight(Outside)->Green();
}

void WaterLock::Exit_OutsideGreenLitState()
{
	openDoor->GetTrafficLight(Outside)->Red();
}

void WaterLock::Entry_InsideGreenLitState()
{
	openDoor->GetTrafficLight(Inside)->Green();
}

void WaterLock::Exit_InsideGreenLitState()
{
	openDoor->GetTrafficLight(Inside)->Red();
}

void WaterLock::Entry_RaisingWaterState()
{
	RaiseWater(waterSensor->GetWaterLevel());
}

void WaterLock::Exit_RaisingWaterState()
{
	// NOTE: no functionality
}

void WaterLock::Entry_LoweringWaterState()
{
	LowerWater();
}

void WaterLock::Exit_LoweringWaterState()
{
	// NOTE: no functionality
}

void WaterLock::StopDoors()
{
	lowWaterDoor->Stop();
	highWaterDoor->Stop();
}

void WaterLock::CloseLowWaterDoorValves()
{
	lowWaterDoor->GetValve(Lower)->Close();
	lowWaterDoor->GetValve(Middle)->Close();
	lowWaterDoor->GetValve(Upper)->Close();
}

void WaterLock::CloseHighWaterDoorValves()
{
	highWaterDoor->GetValve(Lower)->Close();
	highWaterDoor->GetValve(Middle)->Close();
	highWaterDoor->GetValve(Upper)->Close();
}

void WaterLock::CloseAllValves()
{
	CloseLowWaterDoorValves();
  CloseHighWaterDoorValves();
}

void WaterLock::TrafficLightsRed()
{
	lowWaterDoor->GetTrafficLight(Inside)->Red();
	lowWaterDoor->GetTrafficLight(Outside)->Red();

	highWaterDoor->GetTrafficLight(Inside)->Red();
	highWaterDoor->GetTrafficLight(Outside)->Red();
}