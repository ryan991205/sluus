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
    if(communicator->Transmit("SetTrafficLight" + GetLightNumber() + "Red:off;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Green(): Green() recieved !ack");
    }

    if(communicator->Transmit("SetTrafficLight" + GetLightNumber() + "Green:on;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Green(): Green() recieved !ack");
    }
}

void TrafficLight::Red()
{
    if(communicator->Transmit("SetTrafficLight" + GetLightNumber() + "Green:off;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Green(): Green() recieved !ack");
    }

    if(communicator->Transmit("SetTrafficLight" + GetLightNumber() + "Red:on;\n") != "ack;")
    {
        throw std::logic_error("TrafficLight::Red(): Red() recieved !ack");
    }
}

std::string TrafficLight::GetLightNumber()
{
    std::string TLightNr;

	     if((side == Left ) && (light == Inside )) TLightNr = "2";
    else if((side == Left ) && (light == Outside)) TLightNr = "1";
    else if((side == Right) && (light == Inside )) TLightNr = "3";
    else if((side == Right) && (light == Outside)) TLightNr = "4";
    else throw std::logic_error("TrafficLight::GetLightNumber(): trafficLight not suported");

    return TLightNr;
}

//ETrafficLightColors TrafficLight::GetColor()
//{
//    std::string red = communicator->Transmit("GetTrafficLight" + GetLightNumber + "Red;\n")
//    std::string green = communicator->Transmit("GetTrafficLight" + GetLightNumber + "Red;\n")
//    ETrafficLightColors lightColor;
//
//    if((red != "on;") && (red != "off;"))
//    {
//        throw std::logic_error("TrafficLight::GetColor(): GetTrafficLightRed did not return a proper response");
//    }
//
//    if((green != "on;") && (green != "off;"))
//    {
//        throw std::logic_error("TrafficLight::GetColor(): GetTrafficLightGreen did not return a proper response");
//    }
//
//    if((red == "on;") && (green == "on;"))
//    {
//        throw std::logic_error("TrafficLight::GetColor(): both red and green are on");
//    }
//
//    if((red == "off;") || (green == "off;"))
//    {
//        throw std::logic_error("TrafficLight::GetColor(): both red and green are off");
//    }
//
//    if(red == "on;")
//    {
//        lightColor = Red;
//    }
//    else
//    {
//        lightColor = Green;
//    }
//
//    return lightColor;
//}