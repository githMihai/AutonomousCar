#ifndef PATHTRACKING_H
#define PATHTRACKING_H

#ifndef PATH_H
#include "path.h"
#endif // PATH_H

#ifndef GPSDATA_H
#include "gpsdata.h"
#endif // GPSDATA_H

class PathTracking : public Path
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
    void update();

private:
    PathTracking(const PathTracking& pathFollower);
    PathTracking& operator=(const PathTracking& pathFollower);

    GPSData globalPosition_;
    GPSData pathPosition_;
};

#endif // PATHTRACKING_H