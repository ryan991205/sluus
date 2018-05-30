#include "LockableDoor.h"

LockableDoor::LockableDoor(EWaterLockSides side) : Door(side)
{
	lock = new DoorLock(side);
}

LockableDoor::~LockableDoor()
{
	delete lock;
}

const DoorLock* LockableDoor::GetLock() const
{
	return lock;
}
