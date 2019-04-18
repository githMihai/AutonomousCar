#ifndef PATHTRACKING_H
#define PATHTRACKING_H

#ifndef PATH_H
#include "path.h"
#endif // PATH_H

#ifndef GPSDATA_H
#include "gpsdata.h"
#endif // GPSDATA_H

// TODO: temporary
#ifndef GPSCONNECTION_H
#include "gpsconnection.h"
#endif // GPSCONNECTION_H

class PathTracking
{
public:
    /*!
     * \name PathTracking
     * \brief Construct a new Path Tracking object.
     */
    PathTracking(std::complex<double> startingNode, std::complex<double> destinationNode);

    /*!
     * \name PathTracking
     * \brief Construct a new Path Tracking object.
     */
    PathTracking(const std::string startingNode, const std::string destinationNode);

    PathTracking(const PathTracking& pathTracking)
    {
        this->g_ = pathTracking.g_;
        this->globalPosition_ = pathTracking.globalPosition_;
        this->running = pathTracking.running;
        this->displ = pathTracking.displ;
        this->path = pathTracking.path;
    }
    
    /*!
     * \name globalPosition
     * \brief Getter for globalPosition_.
     * \return const GPSData& 
     */
    const GPSData& globalPosition() const;

    /*!
     * \name pathPosition
     * \brief Getter fot pathPosition_.
     * \return const GPSData& 
     */
    const GPSData& pathPosition() const;


    /*!
     * \name update
     * \brief Updates position.
     */
    void update_thread();


    void run();

    // PathTracking(const PathTracking& pathFollower);
    // PathTracking& operator=(const PathTracking& pathFollower);

    Path* path;
    GPSData* globalPosition_;
    GPSData pathPosition_;
    std::shared_ptr<GPSConnection> g_;
    double displ;
    
    bool running;

private:
    // PathTracking(const PathTracking& pathFollower);
    // PathTracking& operator=(const PathTracking& pathFollower);

    // GPSData globalPosition_;
    // GPSData pathPosition_;
    // std::shared_ptr<GPSConnection> g_;
    // double displ;
    
    // bool running;
};

#endif // PATHTRACKING_H