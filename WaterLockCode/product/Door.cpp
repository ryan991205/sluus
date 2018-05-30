#include "Door.h"


Door::Door(EWaterLockSides side, EventGenerator* eventGenerator, Communicator* const TCP_Con)
{
	if(eventGenerator == nullptr)
	{
		throw std::logic_error("eventGenerator == nullptr");
	}

	if(TCP_Con == nullptr)
	{
		throw std::logic_error("TCP_Con == nullptr");
	}

	this->eventGenerator = eventGenerator;
	this->side = side;
	communicator = TCP_Con;

	lowerValve = new Valve(side, Lower);
	middleValve = new Valve(side, Middle);
	upperValve = new Valve(side, Upper);
	insideLight = new TrafficLight(side, Inside, TCP_Con);
	outsideLight = new TrafficLight(side, Outside, TCP_Con);

	continueDoorStatePolling = false;
	pollThread = nullptr;
}

Door::~Door()
{
	KillPollThread();
	delete lowerValve;
	delete middleValve;
	delete upperValve;
	delete insideLight;
	delete outsideLight;
}

TrafficLight* Door::GetTrafficLight(ETrafficLights trafficLight)
{
	switch(trafficLight)
	{
		case Inside  : return insideLight;	break;
		case Outside : return outsideLight; break;
		default      : break; // Note: do nothing
	}
	return nullptr;
}

Valve* Door::GetValve(EValves valve)
{
	switch(valve)
	{
		case Lower  : return lowerValve;  break;
		case Middle : return middleValve; break;
		case Upper  : return upperValve;  break;
		default     : break; // Note: do nothing
	}
	return nullptr;
}

void Door::Open()
{
	std::string str = "SetDoor" + sideAsString(side) + ":open;";

    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void Door::Close()
{
	std::string str = "SetDoor" + sideAsString(side) + ":close;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void Door::Stop()
{
	std::string str = "SetDoor" + sideAsString(side) + ":stop;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }

}

void Door::StartPollingDoorSateOnPollThread()
{
	if(pollThread != nullptr)
	{
		KillPollThread();
	}

	pollThread = new std::thread(&Door::PollDoorState, this);
}

void Door::PollDoorState()
{
	EDoorStates currentState = GetState();
	EDoorStates newState = currentState;
	eventGenerator->DoorStateChanged(); // Note: to make sure we didn't miss a state change.

	continueDoorStatePolling = true;
	while(continueDoorStatePolling)
	{
		newState = GetState();
		if(currentState != newState)
		{
			currentState = newState;
			eventGenerator->DoorStateChanged();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void Door::KillPollThread()
{
	if(pollThread != NULL)
	{
		if(pollThread->joinable())
		{
			continueDoorStatePolling = false;
			pollThread->join();
		}

		delete pollThread;
		pollThread = nullptr;
	}
}

EDoorStates Door::GetState()
{
	std::string str = "GetDoor" +  sideAsString(side) + ";\0";

    str = communicator->Transmit(str);

    	   if(str.compare("low;")== 0)                return DoorLocked;
    else if(str.compare("belowValve2;") == 0)       return DoorClosed;
    else if(str.compare("aboveValve2;") == 0)       return DoorOpen;
    else if(str.compare("aboveValve3;") == 0)       return DoorClosing;
    else if(str.compare("high;") ==0)               return DoorOpening;
    else if(str.compare("high;") ==0)               return DoorStopped;
    else if(str.compare("high;") ==0)               return MotorDamage;
    else
    {
        // error
    }
	return  MotorDamage; // FIXME
}

std::string Door::sideAsString(EWaterLockSides waterLockSide)
{
    if(waterLockSide == Left)   return "Left";
    else                        return "Right";
}
