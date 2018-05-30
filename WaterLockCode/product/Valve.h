#ifndef VALVE_H
#define VALVE_H

#include "interfaces/IValve.h"
#include "EWaterLockSides.h"
#include "EValves.h"


class Valve : public IValve
{
	private:
		EWaterLockSides side;
		EValves valve;

	public:
		Valve(EWaterLockSides side, EValves valve);

		void Open();

		void Close();

		// private copy constructor and assignment operator to prevent making copies
    Valve(const Valve&) { /* do nothing */ };
    Valve& operator= (const Valve&) { return *this; };
};

#endif
