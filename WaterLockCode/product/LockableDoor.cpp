#include "LockableDoor.h"

LockableDoor::LockableDoor(EWaterLockSides side,  EventGenerator* eventGenerator, Communicator* const TCP_Con) : Door(side, eventGenerator, TCP_Con)
{
	lock = new DoorLock(side,TCP_Con);
}

LockableDoor::~LockableDoor()
{
	delete lock;
}

const DoorLock* LockableDoor::GetLock() const
{
	return lock;
}
