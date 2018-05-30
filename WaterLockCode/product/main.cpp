#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "WaterLock.h"
#include "Door.h"
#include "LockableDoor.h"
#include "RepeatingDoor.h"
#include "EWaterLockSides.h"
#include "EWaterLockSides.h"
#include "Communicator.h"


bool continueLoop = true;

void InputHandler(int s)
{
    continueLoop = s; // Function does not work without s, so this is to keep werror from complaining.
    continueLoop = false;
    printf("\n Exiting main loop!\n");
}

void SetupInputHandeling(struct sigaction* sigIntHandler)
{
    sigIntHandler->sa_handler = InputHandler;
    sigemptyset(&(sigIntHandler->sa_mask));
    sigIntHandler->sa_flags = 0;

    sigaction(SIGINT, sigIntHandler, nullptr);

    printf("Press ctrl + c to quit\n\n");
    usleep(2000000);
}

void ShowMenu()
{
  //std::cout << "Press ctrl + c to quit\n" << std::endl;
}

int main()
{
  std::cout << "\nProgram Start\n" << std::endl;
  
  Communicator* simCom1_Normal        = new Communicator(5555);
  Communicator* simCom2_Normal        = new Communicator(5556);
  Communicator* simCom3_LockableDoor  = new Communicator(5557);
  Communicator* simCom4_RepeatingDoor = new Communicator(5558);

  WaterLock waterLock1_Normal(        new Door(Left,simCom1_Normal), 
                                      new Door(Right,simCom1_Normal));
  WaterLock waterLock2_Normal(        new Door(Left,simCom2_Normal), 
                                      new Door(Right,simCom2_Normal));
  WaterLock waterLock3_LockableDoor(  new LockableDoor(Left,simCom3_LockableDoor), 
                                      new LockableDoor(Right,simCom3_LockableDoor));
  WaterLock waterLock4_RepeatingDoor( new RepeatingDoor(Left,simCom4_RepeatingDoor), 
                                      new RepeatingDoor(Right,simCom4_RepeatingDoor));

  // Note: the following code sets up input handeling
  struct sigaction sigIntHandler;
  SetupInputHandeling(&sigIntHandler);

  continueLoop = true;
  while(continueLoop)
  {
    ShowMenu();
  }

  std::cout << "\nProgram End\n" << std::endl;
}