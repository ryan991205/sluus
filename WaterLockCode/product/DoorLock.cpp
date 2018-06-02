#include "DoorLock.h"

DoorLock::DoorLock(EWaterLockSides side, Communicator* const TCP_Con)
{
    if(TCP_Con == nullptr)
    {
        throw std::logic_error("DoorLock::DoorLock(): TCP_Con == nullptr");
    }

	communicator = TCP_Con;
	this->side = side;
}

void DoorLock::Lock()
{
    if(communicator->Transmit("SetDoorLock" + SideAsString() + ":on;") != "ack;")
    {
        throw std::logic_error("DoorLock::Lock(): Lock() recieved !ack");
    }
}

void DoorLock::Unlock()
{
    if(communicator->Transmit("SetDoorLock" + SideAsString() + ":off;") != "ack;")
    {
        throw std::logic_error("DoorLock::Unlock(): Unlock() recieved !ack");
    }
}

ELockStates DoorLock::GetState()
{
    std::string answer = communicator->Transmit("GetDoorLockState" +  SideAsString() +  ";\0");

    ELockStates lockState;

         if(answer.compare("lockWorking;") == 0) lockState = LockWorking;
    else if(answer.compare("lockDamaged;") == 0) lockState = LockDamaged;
    else throw std::logic_error("DoorLock::GetState(): lock state == unsuported state");

    return lockState;
}

std::string DoorLock::SideAsString()
{
    std::string sideStr;

    switch(side)
    {
        case Left  : sideStr = "Left";  break;
        case Right : sideStr = "Right"; break;
        default : throw std::logic_error("DoorLock::SideAsString(): waterLockSide == unsuported side"); break;
    }

    return sideStr;
}