#include "WaterSensor.h"

#include <string>


WaterSensor::WaterSensor(Communicator& TCP_Con)
    : communicator(TCP_Con)
{

}

EWaterLevels WaterSensor::GetWaterLevel()
{
	std::string answer = communicator.Transmit("GetWaterLevel;\n");

    EWaterLevels waterLevel = Low;

         if(answer.compare("low;"        ) == 0) waterLevel = Low;
    else if(answer.compare("belowValve2;") == 0) waterLevel = BelowMiddleValve;
    else if(answer.compare("aboveValve2;") == 0) waterLevel = AboveMiddleValve;
    else if(answer.compare("aboveValve3;") == 0) waterLevel = AboveUpperValve;
    else if(answer.compare("high;"       ) == 0) waterLevel = High;
    else
    {
        throw std::logic_error("WaterSensor::GetWaterLevel(): waterLevel == unsuported waterLevel");
    }

    return waterLevel;
}