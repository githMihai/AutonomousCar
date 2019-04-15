#ifndef PATHTRACKING_H
#include "pathtracking.h"
#endif // PATHTRACKING_H

#ifndef MAP_H
#include "map.h"
#endif // MAP_H

// TODO: temporary
#ifndef GPSCONNECTION_H
#include "gpsconnection.h"
#endif // GPSCONNECTION_H

#include <thread>

PathTracking::PathTracking(std::complex<double> startingNode, std::complex<double> destinationNode)
    : Path(Map::getInstance()[startingNode][0], Map::getInstance()[destinationNode][0])
{
    // // TODO: hardcoded
    // GPSConnection *g = new GPSConnection(4, 12346);
    // this->globalPosition_ = GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // std::thread run (&GPSConnection::getServer, g);
    // std::thread getPoz (&GPSConnection::getPositionData, g, &this->globalPosition_);
}

PathTracking::PathTracking(const std::string startingNode, const std::string destinationNode)
    : Path(Map::getInstance().nodePointer(startingNode), Map::getInstance().nodePointer(destinationNode))
{
    // // TODO: hardcoded
    // GPSConnection *g = new GPSConnection(4, 12346);
    // this->globalPosition_ = GPSData(4, std::complex<double>(0,0), std::complex<double>(0,0));
    // std::thread run (&GPSConnection::getServer, g);
    // std::thread getPoz (&GPSConnection::getPositionData, g, &this->globalPosition_);
}

const GPSData& PathTracking::globalPosition() const     { return this->globalPosition_; }
const GPSData& PathTracking::pathPosition() const       { return this->pathPosition_; }

void PathTracking::update()
{
    // this->pathPosition_ = this->closest
}