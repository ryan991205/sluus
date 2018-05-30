#include "DoorLock.h"

DoorLock::DoorLock(EWaterLockSides side)
{
	this->side = side;
}

void DoorLock::Lock()
{
	// TODO - implement DoorLock::Lock
}

void DoorLock::Unlock()
{
	// TODO - implement DoorLock::Unlock
}

ELockStates DoorLock::GetState()
{
	// TODO - implement DoorLock::GetState
	return LockDamaged;
}
