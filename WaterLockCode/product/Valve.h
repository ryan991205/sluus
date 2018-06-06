#ifndef VALVE_H
#define VALVE_H

#include "IValve.h"
#include "Communicator.h"
#include "EWaterLockSides.h"
#include "EValves.h"


class Valve : public IValve
{
	public:
		Valve(EWaterLockSides side, EValves valve, Communicator& TCP_Con);

		void Open();
		void Close();

	private:
		EWaterLockSides side;
		EValves valve;
		Communicator& communicator;

		std::string SideAsString();
		std::string ValveAsString();

		// private copy constructor and assignment operator to prevent making copies
    Valve(const Valve& arg) : communicator(arg.communicator) { /* do nothing */ };
    Valve& operator= (const Valve&) { return *this; };
};

#endif
