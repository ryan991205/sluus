#include "LockableDoor.h"

#include "DoorLock.h"

#include <iostream> // Note: debug


LockableDoor::LockableDoor(EWaterLockSides side,  IWaterLockEventGenerator* eventGenerator, Communicator* const TCP_Con) : Door(side, eventGenerator, TCP_Con)
{
	lock = new DoorLock(side,TCP_Con);
}

LockableDoor::~LockableDoor()
{
	delete lock;
}

const ILock* LockableDoor::GetLock() const
{
	return lock;
}

void LockableDoor::Open()
{
	lock->Unlock();

  if(communicator->Transmit("SetDoor" + SideAsString() + ":open;\n") != "ack;")
  {
		throw std::logic_error("LockableDoor::Open(): Open() recieved !ack");
  }
}

void LockableDoor::Stop()
{
  if(communicator->Transmit("SetDoor" + SideAsString() + ":stop;\n") != "ack;")
  {
    throw std::logic_error("LockableDoor::Stop(): Stop() recieved !ack");
  }

	if(GetState() == DoorClosed)
	{
		lock->Lock();
	}
}