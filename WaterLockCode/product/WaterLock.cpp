#include "WaterLock.h"

#include <stdexcept>

#include "EEvents.h"


WaterLock::WaterLock(Door& _lowWaterDoor, Door& _highWaterDoor, IWaterSensor& _waterSensor, IWaterLockEventGenerator& _eventGenerator)
	: lowWaterDoor(_lowWaterDoor)
	, highWaterDoor(_highWaterDoor)
	, waterSensor(_waterSensor)
	, eventGenerator(_eventGenerator)
{
	SetOpenDoor();

	continueEventPolling = false;
	eventPollThread = nullptr;

	continueWaterLevelPolling = false;
	waterLevelPollThread = nullptr;

	CloseAllValves();
	TrafficLightsRed();

	state = ST_NormalOperation;
	Entry_NormalOperationState();

	StartPollingEventsOnEventPollThread();
}

WaterLock::~WaterLock()
{
	KillEventPollThread();
	KillWaterLevelPollThread();
}

void WaterLock::RaiseWater(EWaterLevels waterLevel)
{
	switch(waterLevel)
	{
		case Low              :
			highWaterDoor.GetValve(Lower)->Open();
			break;
		case BelowMiddleValve :
			highWaterDoor.GetValve(Lower)->Open();
			break;
		case AboveMiddleValve :
			highWaterDoor.GetValve(Lower)->Open();
			highWaterDoor.GetValve(Middle)->Open();
			break;
		case AboveUpperValve  :
			highWaterDoor.GetValve(Lower)->Open();
			highWaterDoor.GetValve(Middle)->Open();
			highWaterDoor.GetValve(Upper)->Open();
			break;
		default : throw std::logic_error("WaterLock::RaiseWater(): waterLevel == unsuported water level"); break;
	}
}

void WaterLock::LowerWater()
{
	lowWaterDoor.GetValve(Lower)->Open();
}

void WaterLock::StartPollingEventsOnEventPollThread()
{
	KillEventPollThread();

	eventPollThread = new std::thread(&WaterLock::PollEvents, this);
}

void WaterLock::PollEvents()
{
	continueEventPolling = true;
	while(continueEventPolling)
	{
		HandleEvent(eventGenerator.GetEvent());
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void WaterLock::KillEventPollThread()
{
	if(eventPollThread != nullptr)
	{
		if(eventPollThread->joinable())
		{
			continueEventPolling = false;
			eventPollThread->join();
		}

		delete eventPollThread;
		eventPollThread = nullptr;
	}
}

void WaterLock::StartPollingWaterLevelOnWaterLevelPollThread()
{
    KillWaterLevelPollThread();
    waterLevelPollThread = new std::thread(&WaterLock::PollWaterLevel, this);
}

void WaterLock::PollWaterLevel()
{
    EWaterLevels currentWaterLevel = waterSensor.GetWaterLevel();
    EWaterLevels newWaterLevel = currentWaterLevel;
    eventGenerator.WaterLevelChanged(); // Note: to make sure we didn't miss a water level state change.

    continueWaterLevelPolling = true;
    while(continueWaterLevelPolling)
    {

        newWaterLevel = waterSensor.GetWaterLevel();
        if(currentWaterLevel != newWaterLevel)
        {
            currentWaterLevel = newWaterLevel;
            eventGenerator.WaterLevelChanged();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void WaterLock::KillWaterLevelPollThread()
{
    if(waterLevelPollThread != nullptr)
    {
        if(waterLevelPollThread->joinable())
        {
            continueWaterLevelPolling = false;
            waterLevelPollThread->join();
        }

        delete waterLevelPollThread;
        waterLevelPollThread = nullptr;
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

			Exit_BothDoorsClosedState();
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
	switch(bothDoorsClosedSubState)
	{
		case ST_RaisingWater  : HandleRaisingWaterState(ev);  break;
		case ST_LoweringWater : HandleLoweringWaterState(ev); break;
		default : throw std::logic_error("WaterLock::HandleBothDoorsClosedState(): bothDoorsClosedSubState == unsuported state"); break;
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
				oneDoorOpenSubState = ST_Open;
				Entry_OpenState();
			}
			break;
		default : break;
	}
}

void WaterLock::HandleClosingState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_ClosingState();
			oneDoorOpenSubState = ST_Opening;
			Entry_OpeningState();
			break;
		case EV_DoorStateChanged :
			if(openDoor->GetState() == DoorClosed)
			{
				Exit_ClosingState();
				Exit_OneDoorOpenState();
				normalOperationSubState = ST_BothDoorsClosed;
				Entry_BothDoorsClosedState();
			}
			break;
		default : break;
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
		default : break;
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
		default : break;
	}
}

void WaterLock::HandleInsideGreenLitState(EEvents ev)
{
	switch(ev)
	{
		case EV_ReleaseOutsideButtonPressed :
			Exit_InsideGreenLitState();
			oneDoorOpenSubState = ST_OutsideGreenLit;
			Entry_OutsideGreenLitState();
			break;
		case EV_ReleaseInsideButtonPressed :
			Exit_InsideGreenLitState();
			oneDoorOpenSubState = ST_Open;
			Entry_OpenState();
			break;
		default : break;
	}
}

void WaterLock::HandleRaisingWaterState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_RaisingWaterState();
			CloseHighWaterDoorValves();
			bothDoorsClosedSubState = ST_LoweringWater;
			Entry_LoweringWaterState();
			break;
		case EV_WaterLevelChanged :
			Exit_RaisingWaterState();
			if(waterSensor.GetWaterLevel() == High)
			{
				CloseHighWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = &highWaterDoor;
				Entry_OneDoorOpenState();
			}
			else
			{
				Entry_RaisingWaterState();
			}
			break;
		default : break;
	}
}

void WaterLock::HandleLoweringWaterState(EEvents ev)
{
	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_LoweringWaterState();
			CloseLowWaterDoorValves();
			bothDoorsClosedSubState = ST_RaisingWater;
			Entry_RaisingWaterState();
			break;
		case EV_WaterLevelChanged :
			if(waterSensor.GetWaterLevel() == Low)
			{
				Exit_LoweringWaterState();
				CloseLowWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = &lowWaterDoor;
				Entry_OneDoorOpenState();
			}
			break;
		default : break;
	}
}

void WaterLock::Entry_NormalOperationState()
{
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
	// Note: no functionality
}

void WaterLock::Entry_EmergencyState()
{
	StopDoors();
	CloseAllValves();
	TrafficLightsRed();
}

void WaterLock::Exit_EmergencyState()
{
	SetOpenDoor();
}

void WaterLock::Entry_OneDoorOpenState()
{
	openDoor->GetTrafficLight(Inside)->Red();
	openDoor->GetTrafficLight(Outside)->Red();

	switch(openDoor->GetState())
	{
		case DoorLocked  :
			oneDoorOpenSubState = ST_Opening;
			Entry_OpeningState();
			break;
    case DoorClosed  :
			oneDoorOpenSubState = ST_Opening;
			Entry_OpeningState();
			break;
    case DoorOpen    :
			oneDoorOpenSubState = ST_Open;
			Entry_OpenState();
			break;
    case DoorClosing :
			oneDoorOpenSubState = ST_Closing;
			Entry_ClosingState();
			break;
    case DoorOpening :
			oneDoorOpenSubState = ST_Opening;
			Entry_OpeningState();
			break;
    case DoorStopped :
			oneDoorOpenSubState = ST_Opening;
			Entry_OpeningState();
			break;
    case MotorDamage :
			throw std::logic_error("WaterLock::Exit_EmergencyState(): openDoorState == MotorDamage");
			break;
		default : throw std::logic_error("WaterLock::Exit_EmergencyState(): openDoorState == unsuported state");
	}
}

void WaterLock::Exit_OneDoorOpenState()
{
	openDoor = nullptr;
}

void WaterLock::Entry_BothDoorsClosedState()
{
	StartPollingWaterLevelOnWaterLevelPollThread();

	if(waterSensor.GetWaterLevel() == High)
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
	KillWaterLevelPollThread();
}

void WaterLock::Entry_OpeningState()
{
	openDoor->Open();
	openDoor->StartPollingDoorSateOnPollThread();
}

void WaterLock::Exit_OpeningState()
{
  openDoor->KillPollThread();
	openDoor->Stop();
}

void WaterLock::Entry_ClosingState()
{
	openDoor->Close();
	openDoor->StartPollingDoorSateOnPollThread();
}

void WaterLock::Exit_ClosingState()
{
	openDoor->KillPollThread();
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
	RaiseWater(waterSensor.GetWaterLevel());
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
	lowWaterDoor.Stop();
	highWaterDoor.Stop();
}

void WaterLock::CloseLowWaterDoorValves()
{
	lowWaterDoor.GetValve(Lower)->Close();
	lowWaterDoor.GetValve(Middle)->Close();
	lowWaterDoor.GetValve(Upper)->Close();
}

void WaterLock::CloseHighWaterDoorValves()
{
	highWaterDoor.GetValve(Lower)->Close();
	highWaterDoor.GetValve(Middle)->Close();
	highWaterDoor.GetValve(Upper)->Close();
}

void WaterLock::CloseAllValves()
{
	CloseLowWaterDoorValves();
  CloseHighWaterDoorValves();
}

void WaterLock::TrafficLightsRed()
{
	lowWaterDoor.GetTrafficLight(Inside)->Red();
	lowWaterDoor.GetTrafficLight(Outside)->Red();

	highWaterDoor.GetTrafficLight(Inside)->Red();
	highWaterDoor.GetTrafficLight(Outside)->Red();
}

void WaterLock::SetOpenDoor()
{
	bool lowWaterDoorOpen = false;
	switch(lowWaterDoor.GetState())
	{
		case DoorLocked  : lowWaterDoorOpen = false; break;
    case DoorClosed  : lowWaterDoorOpen = false; break;
    case DoorOpen    : lowWaterDoorOpen = true;  break;
    case DoorClosing : lowWaterDoorOpen = true;  break;
    case DoorOpening : lowWaterDoorOpen = true;  break;
    case DoorStopped : lowWaterDoorOpen = true;  break;
    case MotorDamage : throw std::logic_error("WaterLock::SetOpenDoor(): lowWaterDoorState == MotorDamage"); break;
		default : throw std::logic_error("WaterLock::SetOpenDoor(): lowWaterDoorState == usuported state"); break;
	}

	bool highWaterDoorOpen = false;
	switch(highWaterDoor.GetState())
	{
		case DoorLocked  : highWaterDoorOpen = false; break;
    case DoorClosed  : highWaterDoorOpen = false; break;
    case DoorOpen    : highWaterDoorOpen = true;  break;
    case DoorClosing : highWaterDoorOpen = true;  break;
    case DoorOpening : highWaterDoorOpen = true;  break;
    case DoorStopped : highWaterDoorOpen = true;  break;
    case MotorDamage : throw std::logic_error("WaterLock::SetOpenDoor(): highWaterDoorState == MotorDamage"); break;
		default : throw std::logic_error("WaterLock::SetOpenDoor(): highWaterDoorState == usuported state"); break;
	}

	if(lowWaterDoorOpen && highWaterDoorOpen)
	{
		throw std::logic_error("WaterLock::WaterLock(): both doors are open");
	}
	else if(lowWaterDoorOpen)
	{
		openDoor = &lowWaterDoor;
	}
	else if(highWaterDoorOpen)
	{
		openDoor = &highWaterDoor;
	}
	else
	{
		openDoor = nullptr;
	}
}