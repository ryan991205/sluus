#ifndef EEVENTS_H
#define EEVENTS_H


enum EEvents
{
    EV_StartButtonPressed,
    EV_ReleaseInsideButtonPressed,
    EV_ReleaseOutsideButtonPressed,
    EV_EmergencyButtonPressed,
    EV_ResumeNormalOperationsButtonPressed,
    EV_WaterLevelChanged,
    EV_DoorStateChanged,
    EV_Emergency,
    EV_NoEvent
};

#endif
