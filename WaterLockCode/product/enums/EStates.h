#ifndef ESTATES_H
#define ESTATES_H


enum EStates
{
    ST_NormalOperation,
    ST_Emergency
};

enum ENormalOperationSubStates
{
    ST_OneDoorOpen,
    ST_BothDoorsClosed
};

enum EOneDoorOpenSubStates
{
    ST_Opening,
    ST_Closing,
    ST_Open,
    ST_OutsideGreenLit,
    ST_InsideGreenLit
};

enum EBothDoorsClosedSubStates
{
    ST_RaisingWater,
    ST_LoweringWater
};

#endif
