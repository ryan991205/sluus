#ifndef LOCKABLEDOOR_H
#define LOCKABLEDOOR_H

#include <stdio.h> // for NULL

#include "Door.h"
#include "DoorLock.h"
#include "ILockable.h"
#include "ILock.h"


class LockableDoor : public Door, public ILockable
{
	public:
		LockableDoor(EWaterLockSides side);
		~LockableDoor();

		const DoorLock* GetLock() const;

	private:
		DoorLock* lock;
};

#endif