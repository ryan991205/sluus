#ifndef REPEATINGDOOR_H
#define REPEATINGDOOR_H

#include "Door.h"
#include "IRepeatable.h"


class RepeatingDoor : public Door, public IRepeatable
{
	public:
		RepeatingDoor(EWaterLockSides side, IWaterLockEventGenerator* eventGenerator, Communicator* const TCP_Con);
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
