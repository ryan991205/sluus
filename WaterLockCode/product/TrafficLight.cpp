#include "TrafficLight.h"

TrafficLight::TrafficLight(EWaterLockSides side, ETrafficLights light, Communicator* const TCP_Con)
{
	communicator = TCP_Con;
	this->side = side;
	this->light = light;
}

void TrafficLight::Green()
{
	std::string str = "GetTrafficLight" + GetLightNumber() + "Green;\n";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void TrafficLight::Red()
{
std::string str = "GetTrafficLight" + GetLightNumber() + "Red;\n";
     if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

std::string TrafficLight::GetLightNumber()
{
	if(side == Left && light == Inside)          return "1";
    else if(side == Left && light == Outside)    return "2";
    else if(side == Right && light == Inside)    return "3";
    else if(side == Right && light == Outside)   return "4";
}