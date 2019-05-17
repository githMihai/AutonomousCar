#include "statemachine.h"

Machine::Machine() :    idle(true), cross(false), stopSign(false), 
                        exitStop(false), obstacle(false), exitObstacle(false), 
                        laneFollow(false), parkingSign(false), exitParking(false)
{
    // std::shared_ptr<State> idleState = std::shared_ptr<IdleState>();
    // std::shared_ptr<State> pathPlanning = std::shared_ptr<PathPlanningState>();
    // std::shared_ptr<State> laneFollow = std::shared_ptr<LaneFollowState>();
    // std::shared_ptr<State> overtake = std::shared_ptr<OvertakeState>();
    // std::shared_ptr<State> parking = std::shared_ptr<ParkingState>();
    // std::shared_ptr<State> stop = std::shared_ptr<StopSignState>();
    // std::shared_ptr<State> crossroad = std::shared_ptr<CrossroadState>();

    State* idleState = new IdleState();
    State* pathPlanning = new PathPlanningState();
    // State* laneFollow = std::shared_ptr<LaneFollowState>();
    // std::shared_ptr<State> overtake = std::shared_ptr<OvertakeState>();
    // std::shared_ptr<State> parking = std::shared_ptr<ParkingState>();
    // std::shared_ptr<State> stop = std::shared_ptr<StopSignState>();
    // std::shared_ptr<State> crossroad = std::shared_ptr<CrossroadState>();

    this->allStates[IDLE] = idleState;
    this->allStates[PATH_PLANNING] = pathPlanning;
    // this->allStates[CROSSROAD] = crossroad;
    // this->allStates[STOP] = stop;
    // this->allStates[LANE_FOLLOW] = laneFollow;
    // this->allStates[OVERTAKE] = overtake;
    // this->allStates[PARKING] = parking;

    state = IDLE;
}

void* Machine::run(void*)
{
    switch (state)
    {
        case IDLE:
        {
            currentState = this->allStates[IDLE];
            state = PATH_PLANNING;
            break;
        }
        case PATH_PLANNING:
        {
            currentState = this->allStates[PATH_PLANNING];
            state = CROSSROAD;
            break;
        }
        case CROSSROAD:
        {
            currentState = this->allStates[CROSSROAD];
            state = STOP;

            break;
        }
        case STOP:
        {
            currentState = this->allStates[STOP];
            state = LANE_FOLLOW;

            break;
        }
        case LANE_FOLLOW:
        {
            currentState = this->allStates[LANE_FOLLOW];
            state = OVERTAKE;

            break;
        }
        case OVERTAKE:
        {
            currentState = this->allStates[OVERTAKE];
            state = PARKING;

            break;
        }
        case PARKING:
        {
            currentState = this->allStates[PARKING];
            state = IDLE;

            break;
        }
    }
    std::cout << state << std::endl;
    currentState->run();
}





