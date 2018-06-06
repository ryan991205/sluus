#include "EventGenerator.h"


EventGenerator::EventGenerator()
{

}

EEvents EventGenerator::GetEvent()
{
	std::lock_guard<std::mutex> lock(mutex);

	if(events.size() == 0)
	{
		return EV_NoEvent;
	}

	EEvents event = events.at(0);
	events.erase(events.begin());
	return event;
}

void EventGenerator::StartButtonPressed()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_StartButtonPressed);
}

void EventGenerator::ReleaseInsideButtonPressed()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_ReleaseInsideButtonPressed);
}

void EventGenerator::ReleaseOutsideButtonPressed()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_ReleaseOutsideButtonPressed);
}

void EventGenerator::ResumeNormalOperationsButtonPressed()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_ResumeNormalOperationsButtonPressed);
}

void EventGenerator::EmergencyButtonPressed()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_EmergencyButtonPressed);
}

void EventGenerator::WaterLevelChanged()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_WaterLevelChanged);
}

void EventGenerator::DoorStateChanged()
{
	events.push_back(EV_DoorStateChanged);
}

void EventGenerator::Emergency()
{
	std::lock_guard<std::mutex> lock(mutex);

	events.push_back(EV_Emergency);
}