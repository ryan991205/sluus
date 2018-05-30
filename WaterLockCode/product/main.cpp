#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "WaterLock.h"
#include "Door.h"
#include "LockableDoor.h"
#include "RepeatingDoor.h"
#include "EWaterLockSides.h"
#include "EWaterLockSides.h"


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
  std::cout << "Press ctrl + c to quit\n" << std::endl;
}

int main()
{
  std::cout << "\nProgram Start\n" << std::endl;

  WaterLock* waterLock1_Normal = new WaterLock(new Door(Left), new Door(Right));
  WaterLock* waterLock2_Normal = new WaterLock(new Door(Left), new Door(Right));
  WaterLock* waterLock3_DoorLock = new WaterLock(new LockableDoor(Left), new LockableDoor(Right));
  WaterLock* waterLock4_RepeatingDoor = new WaterLock(new RepeatingDoor(Left), new RepeatingDoor(Right));

  // Note: the following code sets up input handeling
  struct sigaction sigIntHandler;
  SetupInputHandeling(&sigIntHandler);

  continueLoop = true;
  while(continueLoop)
  {
    ShowMenu();
  }

  delete waterLock1_Normal;
  delete waterLock2_Normal;
  delete waterLock3_DoorLock;
  delete waterLock4_RepeatingDoor;

  std::cout << "\nProgram End\n" << std::endl;
}