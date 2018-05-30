#include "DoorLock.h"

DoorLock::DoorLock(EWaterLockSides side, Communicator* const TCP_Con)
{
	communicator = TCP_Con;
	this->side = side;
}

void DoorLock::Lock()
{
	std::string str = "SetDoorLock" + sideAsString(side) + ":on;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

void DoorLock::Unlock()
{
	std::string str = "SetDoorLock" + sideAsString(side) + ":off;";
    if(communicator->Transmit(str) != "ack;")
    {
         // error
    }
}

ELockStates DoorLock::GetState()
{
	std::string str = "GetDoorLockState" +  sideAsString(side) +  ";\0";
    str = communicator->Transmit(str);

    if(str.compare("lockWorking;")== 0)             return LockWorking;
    else if(str.compare("lockDamaged;") == 0)       return LockDamaged;
    else
    {
        // error
    }
    return LockDamaged; //FIXME
}

std::string DoorLock::sideAsString(EWaterLockSides waterLockSide)
{
    if(waterLockSide == Left)   return "Left";
    else                        return "Right";
}
