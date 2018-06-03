#include "Door.h"

#include <iostream> // Note: debug


Door::Door(EWaterLockSides side, EventGenerator* eventGenerator, Communicator* const TCP_Con)
{
	if(eventGenerator == nullptr)
	{
		throw std::logic_error("Door::Door(): eventGenerator == nullptr");
	}

	if(TCP_Con == nullptr)
	{
		throw std::logic_error("Door::Door(): TCP_Con == nullptr");
	}

	this->eventGenerator = eventGenerator;
	this->side = side;
	communicator = TCP_Con;

	lowerValve = new Valve(side, Lower, communicator);
	middleValve = new Valve(side, Middle, communicator);
	upperValve = new Valve(side, Upper, communicator);
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
  if(communicator->Transmit("SetDoor" + SideAsString() + ":open;") != "ack;")
  {
		throw std::logic_error("Door::Open(): Open() recieved !ack");
  }
}

void Door::Close()
{
  if(communicator->Transmit("SetDoor" + SideAsString() + ":close;") != "ack;")
  {
    throw std::logic_error("Door::Close(): Close() recieved !ack");
  }
}

void Door::Stop()
{
  if(communicator->Transmit("SetDoor" + SideAsString() + ":stop;") != "ack;")
  {
    throw std::logic_error("Door::Stop(): Stop() recieved !ack");
  }

}

void Door::StartPollingDoorSateOnPollThread()
{
	KillPollThread();

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
	if(pollThread != nullptr)
	{
		std::cout << "Door::KillPollThread(): Loop!" << std::endl; // Note: debug

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
  std::string answer = communicator->Transmit("GetDoor" +  SideAsString() + ";\0");

	EDoorStates doorState;

  	   if(answer.compare("doorLocked;" ) == 0) doorState = DoorLocked;
  else if(answer.compare("doorClosed;" ) == 0) doorState = DoorClosed;
  else if(answer.compare("doorOpen;"   ) == 0) doorState = DoorOpen;
  else if(answer.compare("doorClosing;") == 0) doorState = DoorClosing;
  else if(answer.compare("doorOpening;") == 0) doorState = DoorOpening;
  else if(answer.compare("doorStopped;") == 0) doorState = DoorStopped;
  else if(answer.compare("motorDamage;") == 0) doorState = MotorDamage;
  else throw std::logic_error("Door::GetState(): door state == unsuported state");

	return  doorState;
}

std::string Door::SideAsString()
{
	std::string sideStr;

	switch(side)
	{
		case Left  : sideStr = "Left";  break;
		case Right : sideStr = "Right"; break;
		default : throw std::logic_error("Door::SideAsString(): door side == unsuported side"); break;
	}

	return sideStr;
}
