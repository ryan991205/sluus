#ifndef DOORLOCK_H
#define DOORLOCK_H

#include <string>

#include "ILock.h"
#include "Communicator.h"
#include "EWaterLockSides.h"
#include "ELockStates.h"


class DoorLock : public ILock
{
	public:
		DoorLock(EWaterLockSides side, Communicator& TCP_Con);

		void Lock();
		void Unlock();

		ELockStates GetState();

	private:
		EWaterLockSides side;
		Communicator& communicator;

		std::string SideAsString();

		// private copy constructor and assignment operator to prevent making copies
    DoorLock(const DoorLock& arg) : communicator(arg.communicator) { /* do nothing */ };
    DoorLock& operator= (const DoorLock&) { return *this; };
};

#endif
