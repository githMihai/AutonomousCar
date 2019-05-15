#ifndef PATHTRACKING_H
#define PATHTRACKING_H

#ifndef PATH_H
#include "path.h"
#endif // PATH_H

#ifndef GPSDATA_H
#include "gpsdata.h"
#endif // GPSDATA_H

#ifndef OBSERVER_H
#include "observer.h"
#endif // OBSERVER_H

// TODO: temporary
#ifndef GPSCONNECTION_H
#include "gpsconnection.h"
#endif // GPSCONNECTION_H

class PathTracking : public IObserver
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
     * \name PahtTracking
     * \brief Copy constructor
     * \param pathTracking 
     */
    PathTracking(const PathTracking& pathTracking);
    
    /*!
     * \name globalPosition
     * \brief Getter for globalPosition_
     * \return GPSData 
     */
    GPSData globalPosition();

    /*!
     * \name pathPosition
     * \brief Getter for pathPosition_
     * \return GPSData 
     */
    GPSData pathPosition();

    /*!
     * \name displacement
     * \brief Getter for displ_
     * \return double 
     */
    double displacement();

    std::complex<double> nextNode();

    int nextNodes(const int nodesNo, std::vector<std::complex<double> >& nodes); 

    void update(Subject* subject) override;

    void run();

    // TODO: temporary
    void update(double x, double y);

private:
    Path path;
    GPSData globalPosition_;
    GPSData pathPosition_;
    double displ_;
    bool running;
};

#endif // PATHTRACKING_H