#include "WaterLock.h"

#include <stdexcept>

#include "EValves.h"

#include <iostream> // Note: debug


WaterLock::WaterLock(Door& _lowWaterDoor, Door& _highWaterDoor, WaterSensor& _waterSensor, Communicator& TCP_Con, EventGenerator& _eventGenerator)
	: lowWaterDoor(_lowWaterDoor), highWaterDoor(_highWaterDoor), waterSensor(_waterSensor),
	  communicator(TCP_Con), eventGenerator(_eventGenerator)
{
	if(lowWaterDoor.side == highWaterDoor.side)
	{
		throw std::logic_error("lowWaterDoor.side == highWaterDoor.side");
	}

	SetOpenDoor();

	state = ST_NormalOperation;
	Entry_NormalOperationState();

	pollThread = nullptr;
	StartPollingEventsOnPollThread();
}

WaterLock::~WaterLock()
{
	KillPollThread();
}

IUserInputEventGenerator* WaterLock::GetEventGenerator()
{
	return &eventGenerator;
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
		//std::cout << "WaterLock::PollEvents(): loop!" << std::endl; // Note: debug

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

	std::cout << "new ev: ";
	switch(ev)
	{
		case EV_StartButtonPressed 									: std::cout << "EV_StartButtonPressed," << std::endl; 								 break;
    case EV_ReleaseInsideButtonPressed 					: std::cout << "EV_ReleaseInsideButtonPressed," << std::endl; 				 break;
    case EV_ReleaseOutsideButtonPressed 				: std::cout << "EV_ReleaseOutsideButtonPressed," << std::endl; 				 break;
    case EV_EmergencyButtonPressed 							: std::cout << "EV_EmergencyButtonPressed," << std::endl; 						 break;
    case EV_ResumeNormalOperationsButtonPressed : std::cout << "EV_ResumeNormalOperationsButtonPressed," << std::endl; break;
    case EV_WaterLevelChanged 									: std::cout << "EV_WaterLevelChanged," << std::endl; 									 break;
    case EV_DoorStateChanged 										: std::cout << "EV_DoorStateChanged," << std::endl; 									 break;
    case EV_Emergency 													: std::cout << "EV_Emergency," << std::endl; 													 break;
    case EV_NoEvent 														: std::cout << "EV_NoEvent" << std::endl; 														 break;
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
	std::cout << "WaterLock::HandleNormalOperationState()" << std::endl;

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
	std::cout << "WaterLock::HandleEmergencyState()" << std::endl;

	if(ev == EV_ResumeNormalOperationsButtonPressed)
	{
		Exit_EmergencyState();
		state = ST_NormalOperation;
		Entry_NormalOperationState();
	}
}

void WaterLock::HandleOneDoorOpenStates(EEvents ev)
{
	std::cout << "WaterLock::HandleOneDoorOpenStates()" << std::endl;

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
	std::cout << "WaterLock::HandleBothDoorsClosedState()" << std::endl;

	switch(bothDoorsClosedSubState)
	{
		case ST_RaisingWater  : HandleRaisingWaterState(ev);  break;
		case ST_LoweringWater : HandleLoweringWaterState(ev); break;
		default : throw std::logic_error("WaterLock::HandleBothDoorsClosedState(): bothDoorsClosedSubState == unsuported state"); break;
	}
}

void WaterLock::HandleOpeningState(EEvents ev)
{
	std::cout << "WaterLock::HandleOpeningState()" << std::endl;

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
	std::cout << "WaterLock::HandleClosingState()" << std::endl;

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
	std::cout << "WaterLock::HandleOpenState()" << std::endl;

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
	std::cout << "WaterLock::HandleOutsideGreenLitState()" << std::endl;

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
	std::cout << "WaterLock::HandleInsideGreenLitState()" << std::endl;

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
	std::cout << "WaterLock::HandleRaisingWaterState()" << std::endl;

	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_RaisingWaterState();
			CloseHighWaterDoorValves();
			bothDoorsClosedSubState = ST_LoweringWater;
			Entry_LoweringWaterState();
			break;
		case EV_WaterLevelChanged :
			std::cout << "WaterLevelChanged" << std::endl;
			Exit_RaisingWaterState();

			if(waterSensor.GetWaterLevel() == High)
			{
				CloseHighWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = highWaterDoor;
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
	std::cout << "WaterLock::HandleLoweringWaterState()" << std::endl;

	switch(ev)
	{
		case EV_StartButtonPressed :
			Exit_LoweringWaterState();
			CloseLowWaterDoorValves();
			bothDoorsClosedSubState = ST_RaisingWater;
			Entry_RaisingWaterState();
			break;
		case EV_WaterLevelChanged :
			if(waterSensor.dynamicCastGetWaterLevel() == Low)
			{
				Exit_LoweringWaterState();
				CloseLowWaterDoorValves();
				Exit_BothDoorsClosedState();
				normalOperationSubState = ST_OneDoorOpen;
				openDoor = lowWaterDoor;
				Entry_OneDoorOpenState();
			}
			break;
		default : break;
	}
}

void WaterLock::Entry_NormalOperationState()
{
	std::cout << "WaterLock::Entry_NormalOperationState()" << std::endl;

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
	std::cout << "WaterLock::Exit_NormalOperationState()" << std::endl;
}

void WaterLock::Entry_EmergencyState()
{
	std::cout << "WaterLock::Entry_EmergencyState()" << std::endl;

	StopDoors();
	CloseAllValves();
	TrafficLightsRed();
}

void WaterLock::Exit_EmergencyState()
{
	std::cout << "WaterLock::Exit_EmergencyState()" << std::endl;

	SetOpenDoor();
}

void WaterLock::Entry_OneDoorOpenState()
{
	std::cout << "WaterLock::Entry_OneDoorOpenState()" << std::endl;

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
	std::cout << "WaterLock::Exit_OneDoorOpenState()" << std::endl;

	openDoor = nullptr;
}

void WaterLock::Entry_BothDoorsClosedState()
{
	std::cout << "WaterLock::Entry_BothDoorsClosedState()" << std::endl;

	waterSensor.StartPollingWaterLevelOnPollThread();

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
	std::cout << "WaterLock::Exit_BothDoorsClosedState()" << std::endl;

	waterSensor.KillPollThread();
}

void WaterLock::Entry_OpeningState()
{
	std::cout << "WaterLock::Entry_OpeningState()" << std::endl;

	openDoor->Open();
	openDoor->StartPollingDoorSateOnPollThread();
}

void WaterLock::Exit_OpeningState()
{
	std::cout << "WaterLock::Exit_OpeningState()" << std::endl;

  openDoor->KillPollThread();
	openDoor->Stop();
}

void WaterLock::Entry_ClosingState()
{
	std::cout << "WaterLock::Entry_ClosingState()" << std::endl;

	openDoor->Close();
	openDoor->StartPollingDoorSateOnPollThread();
}

void WaterLock::Exit_ClosingState()
{
	std::cout << "WaterLock::Exit_ClosingState()" << std::endl;

	openDoor->KillPollThread();
	openDoor->Stop();
}

void WaterLock::Entry_OpenState()
{
	std::cout << "WaterLock::Entry_OpenState()" << std::endl;

	// NOTE: no functionality
}

void WaterLock::Exit_OpenState()
{
	std::cout << "WaterLock::Exit_OpenState()" << std::endl;

	// NOTE: no functionality
}

void WaterLock::Entry_OutsideGreenLitState()
{
	std::cout << "WaterLock::Entry_OutsideGreenLitState()" << std::endl;

	openDoor->GetTrafficLight(Outside)->Green();
}

void WaterLock::Exit_OutsideGreenLitState()
{
	std::cout << "WaterLock::Exit_OutsideGreenLitState()" << std::endl;

	openDoor->GetTrafficLight(Outside)->Red();
}

void WaterLock::Entry_InsideGreenLitState()
{
	std::cout << "WaterLock::Entry_InsideGreenLitState()" << std::endl;

	openDoor->GetTrafficLight(Inside)->Green();
}

void WaterLock::Exit_InsideGreenLitState()
{
	std::cout << "WaterLock::Exit_InsideGreenLitState()" << std::endl;

	openDoor->GetTrafficLight(Inside)->Red();
}

void WaterLock::Entry_RaisingWaterState()
{
	std::cout << "WaterLock::Entry_RaisingWaterState()" << std::endl;

	RaiseWater(waterSensor.GetWaterLevel());
}

void WaterLock::Exit_RaisingWaterState()
{
	std::cout << "WaterLock::Exit_RaisingWaterState()" << std::endl;

	// NOTE: no functionality
}

void WaterLock::Entry_LoweringWaterState()
{
	std::cout << "WaterLock::Entry_LoweringWaterState()" << std::endl;

	LowerWater();
}

void WaterLock::Exit_LoweringWaterState()
{
	std::cout << "WaterLock::Exit_LoweringWaterState()" << std::endl;

	// NOTE: no functionality
}

void WaterLock::StopDoors()
{
	std::cout << "WaterLock::StopDoors()" << std::endl;

	lowWaterDoor.Stop();
	highWaterDoor.Stop();
}

void WaterLock::CloseLowWaterDoorValves()
{
	std::cout << "WaterLock::CloseLowWaterDoorValves()" << std::endl;

	lowWaterDoor.GetValve(Lower)->Close();
	lowWaterDoor.GetValve(Middle)->Close();
	lowWaterDoor.GetValve(Upper)->Close();
}

void WaterLock::CloseHighWaterDoorValves()
{
	std::cout << "WaterLock::CloseHighWaterDoorValves()" << std::endl;

	highWaterDoor.GetValve(Lower)->Close();
	highWaterDoor.GetValve(Middle)->Close();
	highWaterDoor.GetValve(Upper)->Close();
}

void WaterLock::CloseAllValves()
{
	std::cout << "WaterLock::CloseAllValves()" << std::endl;

	CloseLowWaterDoorValves();
  CloseHighWaterDoorValves();
}

void WaterLock::TrafficLightsRed()
{
	std::cout << "WaterLock::TrafficLightsRed()" << std::endl;

	lowWaterDoor.GetTrafficLight(Inside)->Red();
	lowWaterDoor.GetTrafficLight(Outside)->Red();

	highWaterDoor.GetTrafficLight(Inside)->Red();
	highWaterDoor.GetTrafficLight(Outside)->Red();
}

void WaterLock::SetOpenDoor()
{
	std::cout << "WaterLock::SetOpenDoor()" << std::endl;

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