#ifndef SERIALHANDLER_H
#include "serialhandler.h"
#endif

#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstdlib>

SerialHandler::SerialHandler()
{

}

SerialHandler::SerialHandler(const char* device, const char* historyFileName) : serialPort(device, 460800, serial::Timeout::simpleTimeout(1000))
{
    this->running = false;
    this->th = std::thread(&SerialHandler::readThread, this);
    this->running = true;
}

void SerialHandler::send(const char* message)
{
    this->lock.lock();
    this->serialPort.write(std::string(message));
    this->lock.unlock();
}

bool SerialHandler::sendMove(double velocity, double angle)
{
    std::string message = this->messageConverter.mctl(velocity, angle);
    if (! message.empty())
    {
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialHandler::sendBrake(double angle)
{
    std::string message = this->messageConverter.brake(angle);
    if (! message.empty())
    {
        std::cout << message.c_str();
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

//TODO sendBezierCurve
bool SerialHandler::sendPidActivation(bool activate)
{
    std::string message = this->messageConverter.pida(activate);
    if (! message.empty())
    {
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialHandler::sendPidValue(double kp, double ki, double kd, double tf)
{
    std::string message = this->messageConverter.pids(kp, ki, kd, tf);
    if (! message.empty())
    {
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialHandler::sendSafetyStopActivation(bool activate)
{
    std::string message = this->messageConverter.sfbr(activate);
    if (! message.empty())
    {
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialHandler::sendDistanceSensorPublisherActivation(bool activate)
{
    std::string message = this->messageConverter.dspb(activate);
    if (! message.empty())
    {
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

bool SerialHandler::sendEncoderActivation(bool activate)
{
    std::string message = this->messageConverter.enpb(activate);
    if (! message.empty())
    {
        std::cout << message.c_str();
        this->send(message.c_str());
        return true;
    }
    else
    {
        return false;
    }
}

void SerialHandler::close()
{
    this->serialPort.close();
    // this->historyFile.close();
}

void SerialHandler::readThread()
{
    while(this->running)
    {
        if (this->serialPort.waitReadable())
        {
            this->buff = this->serialPort.readline();
            std::cout << "Response: " << this->buff << std::endl;
            this->checkWaiter(this->buff);
        }
    }
}

void SerialHandler::addWaiter(std::string key, FnPtr callbackFunction)
{
    this->waiters[key].push_back(callbackFunction);
}

void SerialHandler::checkWaiter(std::string response){
    std::string key = response.substr(1, 4);
    std::map<std::string, CallbackEvent >::iterator it;
    it = this->waiters.find(key);
    if (it != this->waiters.end())
    {
        CallbackEvent l_waiters = this->waiters[key];
        for(FnPtr func : l_waiters)
        {
            func(response.substr(6, response.length()-6-4));
        }
        std::cout << "resp: " << response.substr(6, response.length()-6-4) << std::endl;
        std::cout << "Waiter checked" << std::endl;
    }
}

void SerialHandler::stopThread()
{
    this->running = false;
    this->th.join();
    this->serialPort.close();
    this->historyFile.close();
}
