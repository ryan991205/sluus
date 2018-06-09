#include <iostream>
#include <signal.h>
#include <exception>

#include <thread>

#include "Valve.h"
#include "TrafficLight.h"
#include "Door.h"
#include "RepeatingDoor.h"
#include "LockableDoor.h"
#include "WaterSensor.h"
#include "EventGenerator.h"
#include "Communicator.h"
#include "DoorLock.h"

#include "WaterLock.h"
#include "EWaterLockSides.h"


void ShowMenu()
{
  std::cout << "\n  MENU:"                                                                                                                                      << std::endl;
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
  Communicator w1_TCP_Con(5555);
  Communicator w2_TCP_Con(5556);
  Communicator w3_TCP_Con(5557);
  Communicator w4_TCP_Con(5558);
  std::cout << "done!\n" << std::endl;

  // WaterLock 1, normal
  EventGenerator w1_EventGenerator;
  WaterSensor w1_WaterSensor(w1_EventGenerator, w1_TCP_Con);

  Valve w1_Low_LowerValve(Left, Lower, w1_TCP_Con);
  Valve w1_Low_MiddleValve(Left, Middle, w1_TCP_Con);
  Valve w1_Low_UpperValve(Left, Upper, w1_TCP_Con);
  TrafficLight w1_Low_InsideLight(Left, Inside, w1_TCP_Con);
  TrafficLight w1_Low_OutsideLight(Left, Outside, w1_TCP_Con);
  Door w1_LowWaterDoor(Left, w1_TCP_Con, w1_EventGenerator, w1_Low_LowerValve, w1_Low_MiddleValve, w1_Low_UpperValve, w1_Low_InsideLight, w1_Low_OutsideLight);

  Valve w1_High_LowerValve(Right, Lower, w1_TCP_Con);
  Valve w1_High_MiddleValve(Right, Middle, w1_TCP_Con);
  Valve w1_High_UpperValve(Right, Upper, w1_TCP_Con);
  TrafficLight w1_High_InsideLight(Right, Inside, w1_TCP_Con);
  TrafficLight w1_High_OutsideLight(Right, Outside, w1_TCP_Con);
  Door w1_HighWaterDoor(Right, w1_TCP_Con, w1_EventGenerator, w1_High_LowerValve, w1_High_MiddleValve, w1_High_UpperValve, w1_High_InsideLight, w1_High_OutsideLight);

  WaterLock waterLock1_Normal(w1_LowWaterDoor, w1_HighWaterDoor, w1_WaterSensor, w1_EventGenerator);

  //*// WaterLock 2, normal
  EventGenerator w2_EventGenerator;
  WaterSensor w2_WaterSensor(w2_EventGenerator, w2_TCP_Con);

  Valve w2_Low_LowerValve(Left, Lower, w2_TCP_Con);
  Valve w2_Low_MiddleValve(Left, Middle, w2_TCP_Con);
  Valve w2_Low_UpperValve(Left, Upper, w2_TCP_Con);
  TrafficLight w2_Low_InsideLight(Left, Inside, w2_TCP_Con);
  TrafficLight w2_Low_OutsideLight(Left, Outside, w2_TCP_Con);
  Door w2_LowWaterDoor(Left, w2_TCP_Con, w2_EventGenerator, w2_Low_LowerValve, w2_Low_MiddleValve, w2_Low_UpperValve, w2_Low_InsideLight, w2_Low_OutsideLight);

  Valve w2_High_LowerValve(Right, Lower, w2_TCP_Con);
  Valve w2_High_MiddleValve(Right, Middle, w2_TCP_Con);
  Valve w2_High_UpperValve(Right, Upper, w2_TCP_Con);
  TrafficLight w2_High_InsideLight(Right, Inside, w2_TCP_Con);
  TrafficLight w2_High_OutsideLight(Right, Outside, w2_TCP_Con);
  Door w2_HighWaterDoor(Right, w2_TCP_Con, w2_EventGenerator, w2_High_LowerValve, w2_High_MiddleValve, w2_High_UpperValve, w2_High_InsideLight, w2_High_OutsideLight);

  WaterLock waterLock2_Normal(w2_LowWaterDoor, w2_HighWaterDoor, w2_WaterSensor, w2_EventGenerator);
  //*/

  //*// WaterLock 3, lockable
  EventGenerator w3_EventGenerator;
  WaterSensor w3_WaterSensor(w3_EventGenerator, w3_TCP_Con);

  Valve w3_Low_LowerValve(Left, Lower, w3_TCP_Con);
  Valve w3_Low_MiddleValve(Left, Middle, w3_TCP_Con);
  Valve w3_Low_UpperValve(Left, Upper, w3_TCP_Con);
  TrafficLight w3_Low_InsideLight(Left, Inside, w3_TCP_Con);
  TrafficLight w3_Low_OutsideLight(Left, Outside, w3_TCP_Con);
  DoorLock w3_Low_Lock(Left, w3_TCP_Con);
  LockableDoor w3_LowWaterDoor(Left, w3_TCP_Con, w3_EventGenerator, w3_Low_LowerValve, w3_Low_MiddleValve, w3_Low_UpperValve, w3_Low_InsideLight, w3_Low_OutsideLight, w3_Low_Lock);

  Valve w3_High_LowerValve(Right, Lower, w3_TCP_Con);
  Valve w3_High_MiddleValve(Right, Middle, w3_TCP_Con);
  Valve w3_High_UpperValve(Right, Upper, w3_TCP_Con);
  TrafficLight w3_High_InsideLight(Right, Inside, w3_TCP_Con);
  TrafficLight w3_High_OutsideLight(Right, Outside, w3_TCP_Con);
  DoorLock w3_High_Lock(Right, w3_TCP_Con);
  LockableDoor w3_HighWaterDoor(Right, w3_TCP_Con, w3_EventGenerator, w3_High_LowerValve, w3_High_MiddleValve, w3_High_UpperValve, w3_High_InsideLight, w3_High_OutsideLight, w3_High_Lock);

  WaterLock waterLock3_Normal(w3_LowWaterDoor, w3_HighWaterDoor, w3_WaterSensor, w3_EventGenerator);
  //*/

  //*// WaterLock 4, repeatable
  EventGenerator w4_EventGenerator;
  WaterSensor w4_WaterSensor(w4_EventGenerator, w4_TCP_Con);

  Valve w4_Low_LowerValve(Left, Lower, w4_TCP_Con);
  Valve w4_Low_MiddleValve(Left, Middle, w4_TCP_Con);
  Valve w4_Low_UpperValve(Left, Upper, w4_TCP_Con);
  TrafficLight w4_Low_InsideLight(Left, Inside, w4_TCP_Con);
  TrafficLight w4_Low_OutsideLight(Left, Outside, w4_TCP_Con);
  RepeatingDoor w4_LowWaterDoor(Left, w4_TCP_Con, w4_EventGenerator, w4_Low_LowerValve, w4_Low_MiddleValve, w4_Low_UpperValve, w4_Low_InsideLight, w4_Low_OutsideLight);

  Valve w4_High_LowerValve(Right, Lower, w4_TCP_Con);
  Valve w4_High_MiddleValve(Right, Middle, w4_TCP_Con);
  Valve w4_High_UpperValve(Right, Upper, w4_TCP_Con);
  TrafficLight w4_High_InsideLight(Right, Inside, w4_TCP_Con);
  TrafficLight w4_High_OutsideLight(Right, Outside, w4_TCP_Con);
  RepeatingDoor w4_HighWaterDoor(Right, w4_TCP_Con, w4_EventGenerator, w4_High_LowerValve, w4_High_MiddleValve, w4_High_UpperValve, w4_High_InsideLight, w4_High_OutsideLight);

  WaterLock waterLock4_Normal(w4_LowWaterDoor, w4_HighWaterDoor, w4_WaterSensor, w4_EventGenerator);
  //*/

  std::string ans = "";
  while((ans != "quit") && (ans != "q"))
  {
    ShowMenu();
    std::cin >> ans;

    //*//
         if((ans == "s1StartBtn"         ) || (ans == "1s"))  w1_EventGenerator.StartButtonPressed();
    else if((ans == "s1ReleaseInsideBtn" ) || (ans == "1i"))  w1_EventGenerator.ReleaseInsideButtonPressed();
    else if((ans == "s1ReleaseOutsideBtn") || (ans == "1o"))  w1_EventGenerator.ReleaseOutsideButtonPressed();
    else if((ans == "s1EmergencyBtn"     ) || (ans == "1e"))  w1_EventGenerator.EmergencyButtonPressed();
    else if((ans == "s1StopEmergencyBtn" ) || (ans == "1n"))  w1_EventGenerator.ResumeNormalOperationsButtonPressed();
    //*/

    //*//
    else if((ans == "s2StartBtn"         ) || (ans == "2s"))  w2_EventGenerator.StartButtonPressed();
    else if((ans == "s2ReleaseInsideBtn" ) || (ans == "2i"))  w2_EventGenerator.ReleaseInsideButtonPressed();
    else if((ans == "s2ReleaseOutsideBtn") || (ans == "2o"))  w2_EventGenerator.ReleaseOutsideButtonPressed();
    else if((ans == "s2EmergencyBtn"     ) || (ans == "2e"))  w2_EventGenerator.EmergencyButtonPressed();
    else if((ans == "s2StopEmergencyBtn" ) || (ans == "2n"))  w2_EventGenerator.ResumeNormalOperationsButtonPressed();
    //*/

    //*//
    else if((ans == "s3StartBtn"         ) || (ans == "3s"))  w3_EventGenerator.StartButtonPressed();
    else if((ans == "s3ReleaseInsideBtn" ) || (ans == "3i"))  w3_EventGenerator.ReleaseInsideButtonPressed();
    else if((ans == "s3ReleaseOutsideBtn") || (ans == "3o"))  w3_EventGenerator.ReleaseOutsideButtonPressed();
    else if((ans == "s3EmergencyBtn"     ) || (ans == "3e"))  w3_EventGenerator.EmergencyButtonPressed();
    else if((ans == "s3StopEmergencyBtn" ) || (ans == "3n"))  w3_EventGenerator.ResumeNormalOperationsButtonPressed();
    //*/

    //*//
    else if((ans == "s4StartBtn"         ) || (ans == "4s"))  w4_EventGenerator.StartButtonPressed();
    else if((ans == "s4ReleaseInsideBtn" ) || (ans == "4i"))  w4_EventGenerator.ReleaseInsideButtonPressed();
    else if((ans == "s4ReleaseOutsideBtn") || (ans == "4o"))  w4_EventGenerator.ReleaseOutsideButtonPressed();
    else if((ans == "s4EmergencyBtn"     ) || (ans == "4e"))  w4_EventGenerator.EmergencyButtonPressed();
    else if((ans == "s4StopEmergencyBtn" ) || (ans == "4n"))  w4_EventGenerator.ResumeNormalOperationsButtonPressed();
    //*/

    else if((ans == "quit") || (ans == "q")) std::cout << "Exiting program." << std::endl;
    else std::cout << "Input was: [" << ans << "]. Input not recognized, please try again." << std::endl;

    std::cout << std::endl;
  }

  std::cout << "\nProgram End\n" << std::endl;
  return 0;
}