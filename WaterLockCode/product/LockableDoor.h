#ifndef LOCKABLEDOOR_H
#define LOCKABLEDOOR_H

#include "Door.h"
#include "ILockable.h"
#include "ILock.h"
#include "Communicator.h"


class LockableDoor : public Door, public ILockable
{
	public:
		LockableDoor(EWaterLockSides side,  IWaterLockEventGenerator* eventGenerator, Communicator* const TCP_Con);
		~LockableDoor();

		void Open() override;
		void Stop() override;

	private:
		ILock* lock;
};

#endif