#include "TrafficLight.h"


TrafficLight::TrafficLight(EWaterLockSides side, ETrafficLights light, Communicator* const TCP_Con)
{
    if(TCP_Con == nullptr)
    {
        throw std::logic_error("TrafficLight::TrafficLight(): TCP_Con == nullptr");
    }

	communicator = TCP_Con;
	this->side = side;
	this->light = light;
}

void TrafficLight::Green()
{
    if(communicator->Transmit("GetTrafficLight" + GetLightNumber() + "Green;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Green(): Green() recieved !ack");
    }
}

void TrafficLight::Red()
{
    if(communicator->Transmit("GetTrafficLight" + GetLightNumber() + "Red;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Red(): Red() recieved !ack");
    }
}

std::string TrafficLight::GetLightNumber()
{
    std::string TLightNr;

	     if((side == Left ) && (light == Inside )) TLightNr = "1";
    else if((side == Left ) && (light == Outside)) TLightNr = "2";
    else if((side == Right) && (light == Inside )) TLightNr = "3";
    else if((side == Right) && (light == Outside)) TLightNr = "4";
    else throw std::logic_error("TrafficLight::GetLightNumber(): trafficLight not suported");

    return TLightNr;
}