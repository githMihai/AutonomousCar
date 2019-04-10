#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <serial/serial.h>
#include <condition_variable>
#include <map>
#include <functional>

#include "messageconverter.h"
#include "filehandler.h"

/*!
 * FnPtr defins a pointer to a function that has a string as parameter
 */
typedef void (*FnPtr)(std::string);
typedef std::vector<FnPtr> CallbackEvent;

/*!
 * \brief The SerialHandler class, it contains the functions for sending commands.
 */
class SerialHandler
{
public:
    /*!
     * \name SerialHandler
     * \brief SerialHandler, default constructor.
     */
    SerialHandler();

    /*!
     * \name SerialHandler
     * \brief Constructor method for the SerialHandler class.
     * \param [in] device           serial device file name
     * \param [in] historyFilename  name of the file containing command history
     */
    SerialHandler(const char* device, const char* historyFilename);

    /*!
     * \name send
     * \brief Function for sending a message(command).
     * \param message   message to be sent
     */
    void send(const char* message);

    /*!
     * \name sendMove
     * \brief Function for sending move command.
     * \param velocity  motor PWM signal, or, if PID activatet, the reference (in cm/s)
     * \param angle     steering servo angle
     * \return success status, True if no error
     */
    bool sendMove(double velocity, double angle);

    /*!
     * \name sendBrake
     * \brief Function for sending brake command.
     * \param angle     steering servo angle
     * \return success status, True if no error
     */
    bool sendBrake(double angle);

    /*!
     * \name sendPidActivation
     * \brief Function for sending PID activation command.
     * \param activate  boolean value for activating PID
     * \return success status, True if no error
     */
    bool sendPidActivation(bool activate);

    /*!
     * \name sendPidValue
     * \brief Function for sending PID parameter setting command.
     * \param kp    proportional factor
     * \param ki    integral factor
     * \param kd    derivative factor
     * \param tf    filter time constant
     * \return success status, True if no error
     */
    bool sendPidValue(double kp, double ki, double kd, double tf);

    /*!
     * \name sendSafetyStopActivation
     * \brief Function for sending safety brake activation command.
     * \param activate      boolean value for activating safety brake
     * \return success status, True if no error
     */
    bool sendSafetyStopActivation(bool activate);

    /*!
     * \name sendDistanceSensorPublisherActivation
     * \brief Function for sending distance sensor publisher activation command.
     * \param activate      boolean value for activating distance sensor publishar
     * \return success status, True if no error
     */
    bool sendDistanceSensorPublisherActivation(bool activate);

    /*!
     * \name sendEncoderActivation
     * \brief Function for sending encoder activation command.
     * \param activate      boolean value for activating encoder
     * \return success status, True if no error
     */
    bool sendEncoderActivation(bool activate);

    /*!
     * \name close
     * \brief Function for closing communication.
     */
    void close();

    /*!
     * \name read
     * \brief not implemented
     */
    void read();

    /*!
     * \name flush
     * \brief not implemented
     */
    void flush();

    /*!
     * \name readThread
     * \brief Function for reading from serial port.
     *        This function runs in an infinite loop and it is blocking
     *        (it waits for serail port to be available)
     */
    void readThread();

    /*!
     * \name addWaiter
     * \brief Method for adding a waiter function for the SerialHandelr class.
     * \param key               message key
     * \param callbackFunction  callback function
     */
    void addWaiter(std::string key, FnPtr callbackFunction);

    /*!
     * \name checkWaiter
     * \brief Method for checking the waiter functions set the SerialHandler class and for setting callback events.
     * \param response  response transmitted
     */
    void checkWaiter(std::string response);

    /*!
     * \name deleteWaiter
     * \brief Method for deleting a waiter function for the SerialHandler class.
     * \param key               message key
     * \param callbackFunction  callback function
     */
    void deleteWaiter(std::string key, FnPtr callbackFunction); //TODO not implemented

    /*!
     * \name stopThread
     * \brief Method for stopping a waiter function for the SerialHandler class.
     */
    void stopThread();

private:
    /*!
     * \brief lock to syncronize write on serial port
     */
    std::mutex lock;
    MessageConverter messageConverter;
    FileHandler historyFile;
    serial::Serial serialPort;

    int threadID;
    bool running;
    std::string buff;
    std::thread th;
    std::map<std::string, CallbackEvent > waiters;
};

#endif // SERIALHANDLER_H
