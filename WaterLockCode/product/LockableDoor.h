#ifndef LOCKABLEDOOR_H
#define LOCKABLEDOOR_H

#include "Door.h"
#include "ILock.h"
#include "Communicator.h"


class LockableDoor : public Door
{
	public:
		LockableDoor(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& eventGenerator,
				 				 IValve& lowerValve, IValve& middleValve, IValve& upperValve, ITrafficLight& insideLight,
				 				 ITrafficLight& outsideLight, ILock& lock);

		void Open() override;
		void Stop() override;

	private:
		ILock& lock;
};

#endif