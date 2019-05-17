#ifndef BEHAVIOUR_TREE
#define BEHAVIOUR_TREE

#include <functional>

#include "observer.h"
#include "state.h"
#include "idlestate.h"
#include "parkingstate.h"
#include "stopsignstate.h"
#include "lanefollow.h"
#include "overtakestate.h"
#include "pathplanningstate.h"
#include "crossroadstate.h"

#include "timer.h"

#ifndef FN_PTR
// typedef void* (*FnPtr)(void*);
typedef std::function<void(void*)> FnPtr;
#endif // FNPTR

enum states 
{
    IDLE=0,
    PATH_PLANNING,
    CROSSROAD,
    STOP,
    LANE_FOLLOW,
    OVERTAKE,
    PARKING
};

class Machine : public Subject
{
public:
    Machine();
    void runTask(FnPtr routine);
    void abortTask();
    void* run(void*);

private:
    FnPtr runningTask;

    State* currentState;

    states state;

    State* allStates[7];

    Timer t;

    bool idle;
    bool cross;
    bool stopSign;
    bool exitStop;
    bool obstacle;
    bool exitObstacle;
    bool laneFollow;
    bool parkingSign;
    bool exitParking;
};

#endif // BEHAVIOUR_TREE