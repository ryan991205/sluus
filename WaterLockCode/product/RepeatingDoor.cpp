#include "RepeatingDoor.h"


RepeatingDoor::RepeatingDoor(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
				 	 								   IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 	 								   ITrafficLight& _outsideLight)
	: Door(side, TCP_Con, _eventGenerator, _lowerValve, _middleValve, _upperValve, _insideLight, _outsideLight)
{
	continuePushingDoorCommand = false;
	pushThread = nullptr;
}

RepeatingDoor::~RepeatingDoor()
{
	KillPushThread();
}

void RepeatingDoor::Open()
{
	StartRepeatingDoorOpenCommandWithPushThread();
}

void RepeatingDoor::Close()
{
	StartRepeatingDoorCloseCommandWithPushThread();
}

void RepeatingDoor::Stop()
{
  if(communicator.Transmit("SetDoor" + SideAsString() + ":stop;\n") != "ack;")
  {
    throw std::logic_error("RepeatingDoor::Stop(): Stop() recieved !ack");
  }

	KillPushThread();
}

void RepeatingDoor::StartRepeatingDoorOpenCommandWithPushThread()
{
	KillPushThread();

	pushThread = new std::thread(&RepeatingDoor::RepeatDoorOpenCommand, this);
}

void RepeatingDoor::StartRepeatingDoorCloseCommandWithPushThread()
{
	KillPushThread();

	pushThread = new std::thread(&RepeatingDoor::RepeatDoorCloseCommand, this);
}

void RepeatingDoor::RepeatDoorOpenCommand()
{
	continuePushingDoorCommand = true;
	while(continuePushingDoorCommand)
	{
		if(communicator.Transmit("SetDoor" + SideAsString() + ":open;\n") != "ack;")
  	{
			throw std::logic_error("RepeatingDoor::RepeatDoorOpenCommand(): RepeatDoorOpenCommand() recieved !ack");
  	}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void RepeatingDoor::RepeatDoorCloseCommand()
{
	continuePushingDoorCommand = true;
	while(continuePushingDoorCommand)
	{
		if(communicator.Transmit("SetDoor" + SideAsString() + ":close;\n") != "ack;")
  	{
    	throw std::logic_error("RepeatingDoor::RepeatDoorCloseCommand(): RepeatDoorCloseCommand() recieved !ack");
  	}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void RepeatingDoor::KillPushThread()
{
	if(pushThread != nullptr)
	{
		if(pushThread->joinable())
		{
			continuePushingDoorCommand = false;
			pushThread->join();
		}

		delete pushThread;
		pushThread = nullptr;
	}
}