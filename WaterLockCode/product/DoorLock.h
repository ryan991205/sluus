#ifndef DOORLOCK_H
#define DOORLOCK_H

#include <string>

#include "ILock.h"
#include "EWaterLockSides.h"
#include "ELockStates.h"
#include "Communicator.h"


class DoorLock : public ILock
{
	private:
		EWaterLockSides side;
		Communicator* communicator;

		std::string SideAsString();


	public:
		DoorLock(EWaterLockSides side, Communicator* const TCP_Con);

		void Lock();

		void Unlock();

		ELockStates GetState();

		// private copy constructor and assignment operator to prevent making copies
    DoorLock(const DoorLock&) { /* do nothing */ };
    DoorLock& operator= (const DoorLock&) { return *this; };
};

#endif
