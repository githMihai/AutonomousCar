#ifndef PATHTRACKING_H
#include "pathtracking.h"
#endif // PATHTRACKING_H

#ifndef MAP_H
#include "map.h"
#endif // MAP_H

#include <time.h>
#include <thread>
#include <unistd.h>

PathTracking::PathTracking(std::complex<double> startingNode, std::complex<double> destinationNode)
    // : Path(Map::getInstance()[startingNode][0], Map::getInstance()[destinationNode][0])
{
    // TODO: hardcoded
    this->path = new Path(Map::getInstance()[startingNode][0], Map::getInstance()[destinationNode][0]);
    this->g_ = std::make_shared<GPSConnection>(4, 12346);
    this->globalPosition_ = new GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // this->pathPosition_ = new GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // std::thread run (&GPSConnection::getServer, this->g_);
    // std::thread getPoz (&GPSConnection::getPositionData, this->g_, &this->globalPosition_);
    this->running = true;
    this->displ = 0.0;
}

PathTracking::PathTracking(const std::string startingNode, const std::string destinationNode)
    // : Path(Map::getInstance().nodePointer(startingNode), Map::getInstance().nodePointer(destinationNode))
{
    // TODO: hardcoded
    this->path = new Path(Map::getInstance().nodePointer(startingNode), Map::getInstance().nodePointer(destinationNode));
    this->g_ = std::make_shared<GPSConnection>(4, 12346);
    this->globalPosition_ = new GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // this->pathPosition_ = new GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // std::thread run (&GPSConnection::getServer, this->g_);
    // std::thread getPoz (&GPSConnection::getPositionData, this->g_, &this->globalPosition_);
    this->running = true;
    this->displ = 0.0;
}

const GPSData& PathTracking::globalPosition() const     { return *this->globalPosition_; }
const GPSData& PathTracking::pathPosition() const       { return this->pathPosition_; }

void PathTracking::update_thread()
{
    while (this->running)
    {
        this->pathPosition_.update(this->path->pathPos(globalPosition_->getPosition()), this->globalPosition_->getOrientation());
        // this->displ = this->path->displacement(this->globalPosition_->getPosition());
        std::cout << "global: " << *this->globalPosition_ << std::endl;
        std::cout << "path: " << this->pathPosition_ << std::endl;
        std::cout << "displ: " << this->displ << std::endl;
        sleep(1);
    }
    // this->pathPosition_ = this->closest
}

void PathTracking::run()
{
    std::thread* run = new std::thread(&GPSConnection::getServer, this->g_);
    std::thread* getPoz = new std::thread(&GPSConnection::getPositionData, this->g_, this->globalPosition_);
}