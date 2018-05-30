#ifndef DOORLOCK_H
#define DOORLOCK_H

#include "ILock.h"
#include "EWaterLockSides.h"
#include "ELockStates.h"


class DoorLock : public ILock
{
	private:
		EWaterLockSides side;

	public:
		DoorLock(EWaterLockSides side);

		void Lock();

		void Unlock();

		ELockStates GetState();

		// private copy constructor and assignment operator to prevent making copies
    DoorLock(const DoorLock&) { /* do nothing */ };
    DoorLock& operator= (const DoorLock&) { return *this; };
};

#endif
