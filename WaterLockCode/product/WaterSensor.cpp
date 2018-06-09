#include "WaterSensor.h"

#include <string>

#include <iostream> // Note: debug


WaterSensor::WaterSensor(IWaterLockEventGenerator& _eventGenerator, Communicator& TCP_Con)
    : eventGenerator(_eventGenerator), communicator(TCP_Con)
{
    //continueWaterLevelPolling = false;
    //pollThread = nullptr;
}

WaterSensor::~WaterSensor()
{
    //KillPollThread();
}

EWaterLevels WaterSensor::GetWaterLevel()
{
	std::string answer = communicator.Transmit("GetWaterLevel;\n");
    //std::cout << "\nanswer: " << answer << std::endl;

    EWaterLevels waterLevel = Low;

         if(answer.compare("low;"        ) == 0) waterLevel = Low;
    else if(answer.compare("belowValve2;") == 0) waterLevel = BelowMiddleValve;
    else if(answer.compare("aboveValve2;") == 0) waterLevel = AboveMiddleValve;
    else if(answer.compare("aboveValve3;") == 0) waterLevel = AboveUpperValve;
    else if(answer.compare("high;"       ) == 0) waterLevel = High;
    else
    {
        //std::cout << "\nanswer: " << answer << std::endl;
        throw std::logic_error("WaterSensor::GetWaterLevel(): waterLevel == unsuported waterLevel");

    }
    return waterLevel;
}

/*
void WaterSensor::StartPollingWaterLevelOnPollThread()
{
    KillPollThread();

    pollThread = new std::thread(&WaterSensor::PollWaterLevel, this);
}

void WaterSensor::PollWaterLevel()
{
    EWaterLevels currentWaterLevel = GetWaterLevel();
    EWaterLevels newWaterLevel = currentWaterLevel;
    eventGenerator.WaterLevelChanged(); // Note: to make sure we didn't miss a water level state change.

    continueWaterLevelPolling = true;
    while(continueWaterLevelPolling)
    {
        //std::cout << "WaterSensor::PollWaterLevel(): loop!" << std::endl; // Note: debug

        newWaterLevel = GetWaterLevel();
        if(currentWaterLevel != newWaterLevel)
        {
            currentWaterLevel = newWaterLevel;
            eventGenerator.WaterLevelChanged();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
*/