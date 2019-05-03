#ifndef PATHTRACKING_H
#include "pathtracking.h"
#endif // PATHTRACKING_H

#ifndef MAP_H
#include "map.h"
#endif // MAP_H

#include <time.h>
#include <thread>
#include <unistd.h>

#define CAR_ID 4

PathTracking::PathTracking(std::complex<double> startingNode, std::complex<double> destinationNode) :
    path(Path(Map::getInstance()[startingNode][0], Map::getInstance()[destinationNode][0])),
    globalPosition_(GPSData(CAR_ID, std::complex<double>(0,0), std::complex<double>(0,0))),
    pathPosition_(GPSData(CAR_ID, std::complex<double>(0,0), std::complex<double>(0,0))),
    running(true),
    displ_(0.0)  {}

PathTracking::PathTracking(const std::string startingNode, const std::string destinationNode) :
    path(Path(Map::getInstance().nodePointer(startingNode), Map::getInstance().nodePointer(destinationNode))),
    globalPosition_(GPSData(CAR_ID, std::complex<double>(0,0), std::complex<double>(0,0))),
    pathPosition_(GPSData(CAR_ID, std::complex<double>(0,0), std::complex<double>(0,0))),
    running(true),
    displ_(0.0)  {}

// PathTracking::PathTracking(const PathTracking& pathTracking)
// {
//     this->globalPosition_ = pathTracking.globalPosition_;
//     this->pathPosition_ = pathTracking.pathPosition_;
//     this->running = pathTracking.running;
//     this->displ_ = pathTracking.displ_;
//     this->path = pathTracking.path;
// }

void PathTracking::run()
{

}

void PathTracking::update(Subject* gps)
{
    this->globalPosition_ = ((GPSConnection*)gps)->position;
    this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    this->displ_ = this->path.displacement(this->globalPosition_.getPosition());
    std::cout << this->pathPosition_ << " displ: " << this->displ_ << std::endl;
} 

GPSData PathTracking::globalPosition()      { return this->globalPosition_; }
GPSData PathTracking::pathPosition()        { return this->pathPosition_; }
double PathTracking::displacement()         { return this->displ_; }