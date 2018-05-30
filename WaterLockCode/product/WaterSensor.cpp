#include <string>

#include "WaterSensor.h"

WaterSensor::WaterSensor(Communicator* const TCP_Con)
{
	communicator = TCP_Con;

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
}
