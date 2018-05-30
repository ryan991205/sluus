#include "Door.h"


Door::Door(EWaterLockSides side, Communicator* const TCP_Con)
{
	this->side = side;
	communicator = TCP_Con;

	lowerValve = new Valve(side, Lower);
	middleValve = new Valve(side, Middle);
	upperValve = new Valve(side, Upper);
	insideLight = new TrafficLight(side, Inside, TCP_Con);
	outsideLight = new TrafficLight(side, Outside, TCP_Con);
}

Door::~Door()
{
	delete lowerValve;
	delete middleValve;
	delete upperValve;
	delete insideLight;
	delete outsideLight;
}

TrafficLight* Door::GetTrafficLight(ETrafficLights trafficLight)
{
	switch(trafficLight)
	{
		case Inside  : return insideLight;	break;
		case Outside : return outsideLight; break;
		default      : break; // Note: do nothing
	}
	return nullptr;
}

Valve* Door::GetValve(EValves valve)
{
	switch(valve)
	{
		case Lower  : return lowerValve;  break;
		case Middle : return middleValve; break;
		case Upper  : return upperValve;  break;
		default     : break; // Note: do nothing
	}
	return nullptr;
}

void Door::Open()
{
	 std::string str = "SetDoor" + sideAsString(side) + ":open;";

    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void Door::Close()
{
	std::string str = "SetDoor" + sideAsString(side) + ":close;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void Door::Stop()
{
	std::string str = "SetDoor" + sideAsString(side) + ":stop;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }

}

EDoorStates Door::GetState()
{
	std::string str = "GetDoor" +  sideAsString(side) + ";\0";

    str = communicator->Transmit(str);

    if(str.compare("low;")== 0)                     return DoorLocked;
    else if(str.compare("belowValve2;") == 0)       return DoorClosed;
    else if(str.compare("aboveValve2;") == 0)       return DoorOpen;
    else if(str.compare("aboveValve3;") == 0)       return DoorClosing;
    else if(str.compare("high;") ==0)               return DoorOpening;
    else if(str.compare("high;") ==0)               return DoorStopped;
    else if(str.compare("high;") ==0)               return MotorDamage;
    else
    {
        // error
    }
	return  MotorDamage; // FIXME
}

std::string Door::sideAsString(EWaterLockSides waterLockSide)
{
    if(waterLockSide == Left)   return "Left";
    else                        return "Right";
}
