#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "WaterLock.h"
#include "Door.h"
#include "LockableDoor.h"
#include "RepeatingDoor.h"
#include "Communicator.h"
#include "EDoorTypes.h"
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
  //std::cout << "Press ctrl + c to quit\n" << std::endl;
}

int main()
{
  std::cout << "\nProgram Start\n" << std::endl;

  std::cout << "Connecting to hardware... " << std::flush;
  WaterLock waterLock1_Normal(Normal, Left, Normal, 5555);
  WaterLock waterLock2_Normal(Normal, Left, Normal, 5556);
  WaterLock waterLock3_LockableDoor(Lockable, Left, Lockable, 5557);
  WaterLock waterLock4_RepeatingDoor(Repeating, Left, Repeating, 5558);
  std::cout << "done!" << std::endl;

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