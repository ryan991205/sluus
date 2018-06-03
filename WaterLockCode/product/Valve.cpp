#include "Valve.h"

#include <iostream>

Valve::Valve(EWaterLockSides side, EValves valve, Communicator* communicator)
{
	if(communicator== nullptr)
	{
		throw std::logic_error("Valve::Vale(): communicator == nullptr");
	}

	this->side = side;
	this->valve = valve;
	this->communicator = communicator;
}

void Valve::Open()
{
	if(communicator->Transmit("SetDoor" + SideAsString() + "Valve" + ValveAsString() + ":open;\n") != "ack;")
  {
		throw std::logic_error("Valve::Open(): Open() recieved !ack");
  }
}

void Valve::Close()
{
	if(communicator->Transmit("SetDoor" + SideAsString() + "Valve" + ValveAsString() + ":close;\n") != "ack;")
  {
		std::cout << "!ack" << std::endl;
		throw std::logic_error("Valve::Close(): Open() recieved !ack");
  }
}

std::string Valve::SideAsString()
{
	std::string sideStr;

	switch(side)
	{
		case Left  : sideStr = "Left";  break;
		case Right : sideStr = "Right"; break;
		default : throw std::logic_error("Valve::SideAsString(): door side == unsuported side"); break;
	}

	return sideStr;
}

std::string Valve::ValveAsString()
{
	std::string valveNr;

	switch(valve)
	{
		case Lower  : valveNr = "1"; break;
		case Middle : valveNr = "2"; break;
		case Upper  : valveNr = "3"; break;
		default : throw std::logic_error("Valve::ValveAsString(): valve == unsuported valve"); break;
	}

	return valveNr;
}
