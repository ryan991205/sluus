#include "Door.h"

#include "Valve.h"
#include "TrafficLight.h"


Door::Door(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
				 	 IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 	 ITrafficLight& _outsideLight)
	: communicator(TCP_Con)
	,	eventGenerator(_eventGenerator)
	, lowerValve(_lowerValve)
	, middleValve(_middleValve)
	, upperValve(_upperValve)
	, insideLight(_insideLight)
	, outsideLight(_outsideLight)
{
	this->side = side;

	continueDoorStatePolling = false;
	pollThread = nullptr;
}

Door::~Door()
{
	KillPollThread();
}

ITrafficLight* Door::GetTrafficLight(ETrafficLights trafficLight)
{
	switch(trafficLight)
	{
		case Inside  : return &insideLight;	break;
		case Outside : return &outsideLight; break;
		default      : break;
	}
	return nullptr;
}

IValve* Door::GetValve(EValves valve)
{
	switch(valve)
	{
		case Lower  : return &lowerValve;  break;
		case Middle : return &middleValve; break;
		case Upper  : return &upperValve;  break;
		default     : break;
	}
	return nullptr;
}

void Door::Open()
{
  if(communicator.Transmit("SetDoor" + SideAsString() + ":open;\n") != "ack;")
  {
		throw std::logic_error("Door::Open(): Open() recieved !ack");
  }
}

void Door::Close()
{
  if(communicator.Transmit("SetDoor" + SideAsString() + ":close;\n") != "ack;")
  {
    throw std::logic_error("Door::Close(): Close() recieved !ack");
  }
}

void Door::Stop()
{
  if(communicator.Transmit("SetDoor" + SideAsString() + ":stop;\n") != "ack;")
  {
    throw std::logic_error("Door::Stop(): Stop() recieved !ack");
  }
}

EDoorStates Door::GetState()
{
  std::string answer = communicator.Transmit("GetDoor" +  SideAsString() + ";\0");

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

void Door::StartPollingDoorSateOnPollThread()
{
	KillPollThread();

	pollThread = new std::thread(&Door::PollDoorState, this);
}

void Door::PollDoorState()
{
	EDoorStates currentState = GetState();
	EDoorStates newState = currentState;
	eventGenerator.DoorStateChanged(); // Note: to make sure we didn't miss a state change.

	continueDoorStatePolling = true;
	while(continueDoorStatePolling)
	{
		newState = GetState();
		if(currentState != newState)
		{
			currentState = newState;
			eventGenerator.DoorStateChanged();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void Door::KillPollThread()
{
	if(pollThread != nullptr)
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