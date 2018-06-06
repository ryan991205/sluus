#include "LockableDoor.h"

#include "DoorLock.h"

#include <iostream> // Note: debug


LockableDoor::LockableDoor(EWaterLockSides side, IWaterLockEventGenerator& _eventGenerator, Communicator& TCP_Con,
				 	 								 IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 	 								 ITrafficLight& _outsideLight)
	: Door(side, eventGenerator, TCP_Con, lowerValve, middleValve, upperValve, insideLight, outsideLight)
{
	lock = new DoorLock(side, *TCP_Con);
}

LockableDoor::~LockableDoor()
{
	delete lock;
}

void LockableDoor::Open()
{
	lock->Unlock();

  if(communicator.Transmit("SetDoor" + SideAsString() + ":open;\n") != "ack;")
  {
		throw std::logic_error("LockableDoor::Open(): Open() recieved !ack");
  }
}

void LockableDoor::Stop()
{
  if(communicator.Transmit("SetDoor" + SideAsString() + ":stop;\n") != "ack;")
  {
    throw std::logic_error("LockableDoor::Stop(): Stop() recieved !ack");
  }

	if(GetState() == DoorClosed)
	{
		lock->Lock();
	}
}