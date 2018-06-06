#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <vector>
#include <mutex>

#include "IUserInputEventGenerator.h"
#include "IWaterLockEventGenerator.h"
#include "EEvents.h"


class EventGenerator : public IUserInputEventGenerator, public IWaterLockEventGenerator
{
	public:
		std::vector<EEvents> events;

		EventGenerator();

		EEvents GetEvent();

		void StartButtonPressed();

		void ReleaseInsideButtonPressed();

		void ReleaseOutsideButtonPressed();

		void EmergencyButtonPressed();

		void ResumeNormalOperationsButtonPressed();

		void WaterLevelChanged();

		void DoorStateChanged();

		void Emergency();

	private:
		std::mutex mutex;

		// private copy constructor and assignment operator to prevent making copies
    EventGenerator(const EventGenerator&) { /* do nothing */ };
    EventGenerator& operator= (const EventGenerator&) { return *this; };
};

#endif
