#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include <thread>

#include "IWaterSensor.h"
#include "EventGenerator.h"
#include "Communicator.h"
#include "EWaterLevels.h"


class WaterSensor : public IWaterSensor
{
	public:
		WaterSensor(EventGenerator* eventGenerator, Communicator* const TCP_Con);
		~WaterSensor();

		EWaterLevels GetWaterLevel();

		void StartPollingWaterLevelOnPollThread();
		void KillPollThread();

	private:
		EventGenerator* eventGenerator;
		Communicator* communicator;
		std::thread* pollThread;
		bool continueWaterLevelPolling;

		void PollWaterLevel();

		// private copy constructor and assignment operator to prevent making copies
  	WaterSensor(const WaterSensor&) { /* do nothing */ };
    WaterSensor& operator= (const WaterSensor&) { return *this; };
};

#endif
