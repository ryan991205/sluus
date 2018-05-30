#include <string>

#include "WaterSensor.h"

WaterSensor::WaterSensor(EventGenerator* eventGenerator, Communicator* const TCP_Con)
{
    if(eventGenerator == nullptr)
    {
        throw std::logic_error("eventGenerator == nullptr");
    }

    if(TCP_Con == nullptr)
    {
        throw std::logic_error("TCP_Con == nullptr");
    }

    this->eventGenerator = eventGenerator;
	communicator = TCP_Con;

    continueWaterLevelPolling = false;
    pollThread = nullptr;
}

WaterSensor::~WaterSensor()
{
    KillPollThread();
}

EWaterLevels WaterSensor::GetWaterLevel()
{
	std::string str = "GetWaterLevel;\n";
	str = communicator->Transmit(str);

    if(str.compare("low;")== 0)                     return Low;
    else if(str.compare("belowValve2;") == 0)       return BelowMiddleValve;
    else if(str.compare("aboveValve2;") == 0)       return AboveMiddleValve;
    else if(str.compare("aboveValve3;") == 0)       return AboveUpperValve;
    else if(str.compare("high;") ==0)               return High;
    else
    {
        // error
    }
    return Low; //FIXME
}

void WaterSensor::StartPollingWaterLevelOnPollThread()
{
    KillPollThread();

    pollThread = new std::thread(&WaterSensor::PollWaterLevel, this);
}

void WaterSensor::PollWaterLevel()
{
    EWaterLevels currentWaterLevel = GetWaterLevel();
    EWaterLevels newWaterLevel = currentWaterLevel;
    eventGenerator->WaterLevelChanged(); // Note: to make sure we didn't miss a water level state change.

    continueWaterLevelPolling = true;
    while(continueWaterLevelPolling)
    {
        newWaterLevel = GetWaterLevel();
        if(currentWaterLevel != newWaterLevel)
        {
            currentWaterLevel = newWaterLevel;
            eventGenerator->WaterLevelChanged();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void WaterSensor::KillPollThread()
{
    if(pollThread != nullptr)
    {
        if(pollThread->joinable())
        {
            continueWaterLevelPolling = false;
            pollThread->join();
        }

        delete pollThread;
        pollThread = nullptr;
    }
}