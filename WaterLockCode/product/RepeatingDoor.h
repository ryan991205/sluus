#ifndef REPEATINGDOOR_H
#define REPEATINGDOOR_H

#include "Door.h"
#include "IRepeatable.h"


class RepeatingDoor : public Door, public IRepeatable
{
	public:
		RepeatingDoor(EWaterLockSides side, Communicator* const TCP_Con);

	private:
		void Repeat();
};

#endif
