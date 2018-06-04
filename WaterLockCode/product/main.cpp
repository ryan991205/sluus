#include <iostream>
#include <signal.h>
#include <exception>

#include <thread>

#include "WaterLock.h"
#include "EWaterLockSides.h"


void ShowMenu()
{
  std::cout << "\n  MENU:"                                                                                                                                        << std::endl;
  std::cout << "============================================================================================================================================"   << std::endl;
  std::cout << " | [s1StartBtn          / 1s]   Presses start button for WaterLock1.                                                                      | "   << std::endl;
  std::cout << " | [s1ReleaseInsideBtn  / 1i]   Presses the toggle button for the inside traffic light of WaterLock1.                                     | "   << std::endl;
  std::cout << " | [s1ReleaseOutsideBtn / 1o]   Presses the toggle button for the outside traffic light of WaterLock1.                                    | "   << std::endl;
  std::cout << " | [s1EmergencyBtn      / 1e]   Presses the emergency button for WaterLock1.                                                              | "   << std::endl;
  std::cout << " | [s1StopEmergencyBtn  / 1n]   Presses the button that returns the WaterLock to normal operations when in emergeny state for WaterLock1. | "   << std::endl;
  std::cout << " |                                                                                                                                        | "   << std::endl;
  std::cout << " | [s2StartBtn          / 2s]   Presses start button for WaterLock2.                                                                      | "   << std::endl;
  std::cout << " | [s2ReleaseInsideBtn  / 2i]   Presses the toggle button for the inside traffic light of WaterLock2.                                     | "   << std::endl;
  std::cout << " | [s2ReleaseOutsideBtn / 2o]   Presses the toggle button for the outside traffic light of WaterLock2.                                    | "   << std::endl;
  std::cout << " | [s2EmergencyBtn      / 2e]   Presses the emergency button for WaterLock2.                                                              | "   << std::endl;
  std::cout << " | [s2StopEmergencyBtn  / 2n]   Presses the button that returns the WaterLock to normal operations when in emergeny state for WaterLock2. | "   << std::endl;
  std::cout << " |                                                                                                                                        | "   << std::endl;
  std::cout << " | [s3StartBtn          / 3s]   Presses start button for WaterLock3.                                                                      | "   << std::endl;
  std::cout << " | [s3ReleaseInsideBtn  / 3i]   Presses the toggle button for the inside traffic light of WaterLock3.                                     | "   << std::endl;
  std::cout << " | [s3ReleaseOutsideBtn / 3o]   Presses the toggle button for the outside traffic light of WaterLock3.                                    | "   << std::endl;
  std::cout << " | [s3EmergencyBtn      / 3e]   Presses the emergency button for WaterLock3.                                                              | "   << std::endl;
  std::cout << " | [s3StopEmergencyBtn  / 3n]   Presses the button that returns the WaterLock to normal operations when in emergeny state for WaterLock3. | "   << std::endl;
  std::cout << " |                                                                                                                                        | "   << std::endl;
  std::cout << " | [s4StartBtn          / 4s]   Presses start button for WaterLock4.                                                                      | "   << std::endl;
  std::cout << " | [s4ReleaseInsideBtn  / 4i]   Presses the toggle button for the inside traffic light of WaterLock4.                                     | "   << std::endl;
  std::cout << " | [s4ReleaseOutsideBtn / 4o]   Presses the toggle button for the outside traffic light of WaterLock4.                                    | "   << std::endl;
  std::cout << " | [s4EmergencyBtn      / 4e]   Presses the emergency button for WaterLock4.                                                              | "   << std::endl;
  std::cout << " | [s4StopEmergencyBtn  / 4n]   Presses the button that returns the WaterLock to normal operations when in emergeny state for WaterLock4. | "   << std::endl;
  std::cout << " |                                                                                                                                        | "   << std::endl;
  std::cout << " | [quit                / q ]   Quit the program.                                                                                         | "   << std::endl;
  std::cout << "============================================================================================================================================\n" << std::endl;
}

int main()
{
  std::cout << "\nProgram Start\n" << std::endl;

  std::cout << "Connecting to hardware... " << std::flush;
  WaterLock waterLock1_Normal(Normal, Left, Normal, 5555);
  //WaterLock waterLock2_Normal(Normal, Left, Normal, 5556);
  //WaterLock waterLock3_LockableDoor(Lockable, Left, Lockable, 5557);
  //WaterLock waterLock4_RepeatingDoor(Repeating, Left, Repeating, 5558);
  std::cout << "done!\n" << std::endl;

  std::string ans = "";
  while((ans != "quit") && (ans != "q"))
  {
    ShowMenu();
    std::cin >> ans;

         if((ans == "s1StartBtn"         ) || (ans == "1s"))  waterLock1_Normal.GetEventGenerator()->StartButtonPressed();
    else if((ans == "s1ReleaseInsideBtn" ) || (ans == "1i"))  waterLock1_Normal.GetEventGenerator()->ReleaseInsideButtonPressed();
    else if((ans == "s1ReleaseOutsideBtn") || (ans == "1o"))  waterLock1_Normal.GetEventGenerator()->ReleaseOutsideButtonPressed();
    else if((ans == "s1EmergencyBtn"     ) || (ans == "1e"))  waterLock1_Normal.GetEventGenerator()->EmergencyButtonPressed();
    else if((ans == "s1StopEmergencyBtn" ) || (ans == "1n"))  waterLock1_Normal.GetEventGenerator()->ResumeNormalOperationsButtonPressed();

    //else if((ans == "s2StartBtn"         ) || (ans == "2s"))  waterLock2_Normal.GetEventGenerator()->StartButtonPressed();
    //else if((ans == "s2ReleaseInsideBtn" ) || (ans == "2i"))  waterLock2_Normal.GetEventGenerator()->ReleaseInsideButtonPressed();
    //else if((ans == "s2ReleaseOutsideBtn") || (ans == "2o"))  waterLock2_Normal.GetEventGenerator()->ReleaseOutsideButtonPressed();
    //else if((ans == "s2EmergencyBtn"     ) || (ans == "2e"))  waterLock2_Normal.GetEventGenerator()->EmergencyButtonPressed();
    //else if((ans == "s2StopEmergencyBtn" ) || (ans == "2n"))  waterLock2_Normal.GetEventGenerator()->ResumeNormalOperationsButtonPressed();

    //else if((ans == "s3StartBtn"         ) || (ans == "3s"))  waterLock3_LockableDoor.GetEventGenerator()->StartButtonPressed();
    //else if((ans == "s3ReleaseInsideBtn" ) || (ans == "3i"))  waterLock3_LockableDoor.GetEventGenerator()->ReleaseInsideButtonPressed();
    //else if((ans == "s3ReleaseOutsideBtn") || (ans == "3o"))  waterLock3_LockableDoor.GetEventGenerator()->ReleaseOutsideButtonPressed();
    //else if((ans == "s3EmergencyBtn"     ) || (ans == "3e"))  waterLock3_LockableDoor.GetEventGenerator()->EmergencyButtonPressed();
    //else if((ans == "s3StopEmergencyBtn" ) || (ans == "3n"))  waterLock3_LockableDoor.GetEventGenerator()->ResumeNormalOperationsButtonPressed();

    //else if((ans == "s4StartBtn"         ) || (ans == "4s"))  waterLock4_RepeatingDoor.GetEventGenerator()->StartButtonPressed();
    //else if((ans == "s4ReleaseInsideBtn" ) || (ans == "4i"))  waterLock4_RepeatingDoor.GetEventGenerator()->ReleaseInsideButtonPressed();
    //else if((ans == "s4ReleaseOutsideBtn") || (ans == "4o"))  waterLock4_RepeatingDoor.GetEventGenerator()->ReleaseOutsideButtonPressed();
    //else if((ans == "s4EmergencyBtn"     ) || (ans == "4e"))  waterLock4_RepeatingDoor.GetEventGenerator()->EmergencyButtonPressed();
    //else if((ans == "s4StopEmergencyBtn" ) || (ans == "4n"))  waterLock4_RepeatingDoor.GetEventGenerator()->ResumeNormalOperationsButtonPressed();

    else if((ans == "quit") || (ans == "q")) std::cout << "Exiting program." << std::endl;
    else std::cout << "Input was: [" << ans << "]. Input not recognized, please try again." << std::endl;

    std::cout << std::endl;
  }

  std::cout << "\nProgram End\n" << std::endl;
  return 0;
}