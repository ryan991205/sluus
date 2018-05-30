#include "LockableDoor.h"

LockableDoor::LockableDoor(EWaterLockSides side, Communicator* const TCP_Con) : Door(side, TCP_Con)
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
