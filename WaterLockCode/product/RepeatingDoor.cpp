#include "RepeatingDoor.h"

#include <iostream> // Note: debug

RepeatingDoor::RepeatingDoor(EWaterLockSides side, EventGenerator* eventGenerator, Communicator* const TCP_Con) : Door(side, eventGenerator, TCP_Con)
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
  if(communicator->Transmit("SetDoor" + SideAsString() + ":stop;\n") != "ack;")
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
		std::cout << "RepeatingDoor::RepeatDoorOpenCommand(): loop!" << std::endl; // Note: debug

		if(communicator->Transmit("SetDoor" + SideAsString() + ":open;\n") != "ack;")
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
		if(communicator->Transmit("SetDoor" + SideAsString() + ":close;\n") != "ack;")
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