#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include <thread>

#include "IWaterSensor.h"
#include "IWaterLockEventGenerator.h"
#include "Communicator.h"
#include "EWaterLevels.h"


class WaterSensor : public IWaterSensor
{
	public:
		WaterSensor(IWaterLockEventGenerator* eventGenerator, Communicator* const TCP_Con);
		~WaterSensor();

		EWaterLevels GetWaterLevel();

		void StartPollingWaterLevelOnPollThread();
		void KillPollThread();

	private:
		IWaterLockEventGenerator* eventGenerator;
		Communicator* communicator;
		std::thread* pollThread;
		bool continueWaterLevelPolling;

		void PollWaterLevel();

		// private copy constructor and assignment operator to prevent making copies
  	WaterSensor(const WaterSensor&) { /* do nothing */ };
    WaterSensor& operator= (const WaterSensor&) { return *this; };
};

#endif
