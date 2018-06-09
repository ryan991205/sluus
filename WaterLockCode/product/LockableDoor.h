#ifndef LOCKABLEDOOR_H
#define LOCKABLEDOOR_H

#include "Door.h"
#include "ILockable.h"
#include "ILock.h"
#include "Communicator.h"


class LockableDoor : public Door, public ILockable
{
	public:
		LockableDoor(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
				 				 IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 				 ITrafficLight& _outsideLight, ILock& _lock);
		~LockableDoor();

		void Open() override;
		void Stop() override;

	private:
		ILock& lock;
};

#endif