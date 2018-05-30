#include "EventGenerator.h"

EventGenerator::EventGenerator()
{

}

EEvents EventGenerator::GetEvent()
{
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
	events.push_back(EV_StartButtonPressed);
}

void EventGenerator::ReleaseInsideButtonPressed()
{
	events.push_back(EV_ReleaseInsideButtonPressed);
}

void EventGenerator::ReleaseOutsideButtonPressed()
{
	events.push_back(EV_ReleaseOutsideButtonPressed);
}

void EventGenerator::ResumeNormalOperationsButtonPressed()
{
	events.push_back(EV_ResumeNormalOperationsButtonPressed);
}

void EventGenerator::EmergencyButtonPressed()
{
	events.push_back(EV_EmergencyButtonPressed);
}

void EventGenerator::WaterLevelChanged()
{
	events.push_back(EV_WaterLevelChanged);
}

void EventGenerator::DoorStateChanged()
{
	events.push_back(EV_DoorStateChanged);
}

void EventGenerator::Emergency()
{
	events.push_back(EV_Emergency);
}
