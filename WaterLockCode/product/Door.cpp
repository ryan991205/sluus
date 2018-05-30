#include "Door.h"


Door::Door(EWaterLockSides side, Communicator* const TCP_Con)
{
	this->side = side;

	lowerValve = new Valve(side, Lower);
	middleValve = new Valve(side, Middle);
	upperValve = new Valve(side, Upper);
	insideLight = new TrafficLight(side, Inside);
	outsideLight = new TrafficLight(side, Outside);
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
		case Lower  : return lowerValve;	break;
		case Middle : return middleValve; break;
		case Upper  : return upperValve;  break;
		default     : break; // Note: do nothing
	}

	return nullptr;
}

void Door::Open()
{
	// TODO - implement Door::Open
}

void Door::Close()
{
	// TODO - implement Door::Close
}

void Door::Stop()
{
	// TODO - implement Door::Stop
}

EDoorStates Door::GetState()
{
	std::string str = "GetDoor" +  sideAsString(side) + ";\0";

    str = lockCommunicator->Transmit(str);

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
}

std::string sideAsString(EWaterLockSides waterLockSide)
{
    if(waterLockSide == Left)   return "Left";
    else                        return "Right";
}
