#include "LockableDoor.h"

#include "DoorLock.h"

#include <iostream> // Note: debug


LockableDoor::LockableDoor(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
				 	 								 IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 	 								 ITrafficLight& _outsideLight, ILock& _lock)
	: Door(side, TCP_Con, _eventGenerator, _lowerValve, _middleValve, _upperValve, _insideLight, _outsideLight), lock(_lock)
{

}

LockableDoor::~LockableDoor()
{

}

void LockableDoor::Open()
{
	lock.Unlock();

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
		lock.Lock();
	}
}