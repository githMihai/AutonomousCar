#ifndef GPSCONNECTION_H
#define GPSCONNECTION_H

#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include <complex>
#include <cmath>
#include <vector>
#include <netdb.h>

#include "socket.h"
#include "gpsdata.h"

/*!
 * \brief The GPSConnection class, contains functions for receiving position from GPS server
 */
class GPSConnection
{
public:
    /*!
     * \name GPSConnection
     * \brief Constructor method.
     * \param carID             car ID
     * \param negotiationPort   negotiation port
     */
    GPSConnection(int carID, uint16_t negotiationPort);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   GPSConnection object to be copied.
     * \return copy of obj
     */
    GPSConnection& operator=(const GPSConnection &obj);

    /*!
     * \name getServer
     * \brief Method used for listening the server IP. The Server can change.
     */
    void getServer();

    /*!
     * \name sendIDToServer
     * \brief Send the car id to server for identification
     * \param newServerIP   the IP of the new server
     */
    void sendIDToServer(std::string newServerIP);

    /*!
     * \name getPositionData
     * \brief Utility function for receiving the car position and orientation from the server.
     * \param gpsData   GPSData to be updated
     */
    void getPositionData(GPSData *gpsData);

    /*!
     * \name stop
     * \brief not implemented
     */
    void stop();

    /*!
     * \name msg2data
     * \brief This function converts a string message into parameters for GPSData::update
     *        and runs update function
     * \param msg       String message read from GPS server
     * \param gpsData   GPSData to be updated
     */
    void msg2data(std::string msg, GPSData* gpsData);

private:
    // Car related data
    int thisCarId;
    std::complex<double> carOrientation;
    std::complex<double> carPos;

    // Communication parameters
    std::string serverIP;
    int negotiationPort;
    int carSubscriptionPort;
    int carCommunicationPort;
    bool newServerIP;
    int carId;
    int maxWaitTimeForServer;

    std::string hostName;
    std::string hostIP;
    std::vector<std::string> gateway;
    Socket socket;
    sockaddr_in hostAddress;

    // Flags
    bool idSentFlag;
    bool startUp;
    Socket GSocketPoz;
    bool runCarClient;

};

#endif // GPSCONNECTION_H
