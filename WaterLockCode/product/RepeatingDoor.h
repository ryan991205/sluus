#ifndef REPEATINGDOOR_H
#define REPEATINGDOOR_H

#include "Door.h"
#include "IRepeatable.h"


class RepeatingDoor : public Door, public IRepeatable
{
	public:
		RepeatingDoor(EWaterLockSides side, Communicator& TCP_Con, IWaterLockEventGenerator& _eventGenerator,
				 				  IValve& _lowerValve, IValve& _middleValve, IValve& _upperValve, ITrafficLight& _insideLight,
				 				  ITrafficLight& _outsideLight);
		~RepeatingDoor();

		void Open() override;
		void Close() override;
		void Stop() override;

	private:
		bool continuePushingDoorCommand;
		std::thread* pushThread;

		void StartRepeatingDoorOpenCommandWithPushThread();
		void StartRepeatingDoorCloseCommandWithPushThread();
		void RepeatDoorOpenCommand();
		void RepeatDoorCloseCommand();
		void KillPushThread();
};

#endif
