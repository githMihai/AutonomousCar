#ifndef CARCONTROL_H
#include "carcontrol.h"
#include "connexc.h"
#endif // CARCONTROL_H

void* ackFunction(void*) {}

CarControl::CarControl() : serialHandler()
{
    this->flags = DISABLE_ALL;
}

// void p(std::string c)
void* p(void* c)
{
    std::cout << "merge " << std::string((char*)c) << std::endl;
    // std::cout << "merge " << c << std::endl;
}

void* ack(void*)
{
    std::cout << "ack" << std::endl;
}

void CarControl::enable(const uint8_t flag) throw()
{
    bool sent = false;
    bool enableEncoderPublisher         = (flag & ENCODER_PUBLISHER) ? true : false;
    bool enablePID                      = (flag & PID_ACTIVATION) ? true : false; 
    bool enableSafetyStop               = (flag & DISTANCE_SENSORS_PUBLISHER) ? true : false;
    bool enableDistanceSensorsPubliser  = (flag & SAFETTY_STOP) ? true : false;

    {
        // Encoder Publisher
        {
            this->serialHandler.addWaiter("ENPB", ackFunction);
            sent = this->serialHandler.sendEncoderActivation(enableEncoderPublisher);
            if (sent)
            {
                if(this->serialHandler.waitWaiter("ENPB", 3.0))
                {
                    std::cout << "Encoder publisher " << enableEncoderPublisher << std::endl;
                }
                else
                {
                    throw ConnectionException("Response was not received!", -1, 0);
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("ENPB", ackFunction);
        }

        // PID
        {
            this->serialHandler.addWaiter("PIDA", ackFunction);
            sent = this->serialHandler.sendPidActivation(enablePID);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("PIDA", 3.0))
                {
                    std::cout << "PID " << enablePID << std::endl;
                }
                else
                {
                    throw ConnectionException("Response was not received!", -1, 0);
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("PIDA", ackFunction);
        }

        // Safety Stop
        {
            this->serialHandler.addWaiter("SFBR", ackFunction);
            sent = this->serialHandler.sendSafetyStopActivation(enableSafetyStop);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("SFBR", 3.0))
                {
                    std::cout << "Safety stop " << enableSafetyStop << std::endl;
                }
                else
                {
                    throw ConnectionException("Response was not received!", -1, 0);
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("SFBR", ackFunction);
        }

        // Distance Sensors Publisher
        {
            this->serialHandler.addWaiter("DSPB", ackFunction);
            sent = this->serialHandler.sendDistanceSensorPublisherActivation(enableDistanceSensorsPubliser);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("DSPB", 3.0))
                {
                    std::cout << "Distance Sensors publisher " << enableDistanceSensorsPubliser << std::endl;
                }
                else
                {
                    throw ConnectionException("Response was not received!", -1, 0);
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("DSPB", ackFunction);
        }
    }
    this->flags = flag;
}

void CarControl::enableEncoder() throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", ackFunction);
    sent = this->serialHandler.sendEncoderActivation(true);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", 3.0))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", ackFunction);
    this->flags = this->flags | ENCODER_PUBLISHER;
}

void CarControl::enableEncoder(FnPtr callbackFunc) throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", ackFunction);
    sent = this->serialHandler.sendEncoderActivation(true);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", 3.0))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", ackFunction);
    this->serialHandler.addWaiter("ENPB", callbackFunc);
    this->flags = this->flags | ENCODER_PUBLISHER;
}

void CarControl::disableEncoder() throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", ackFunction);
    sent = this->serialHandler.sendEncoderActivation(false);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", 3.0))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", ackFunction);
    this->flags = this->flags & (~ENCODER_PUBLISHER);
}

void CarControl::enablePID() throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("PIDA", ackFunction);
    sent = this->serialHandler.sendPidActivation(true);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("PIDA", 3.0))
        {
            std::cout << "PID activated" << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("PIDA", ackFunction);
    this->flags = this->flags | PID_ACTIVATION;
}

void CarControl::disablePID() throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("PIDA", ackFunction);
    sent = this->serialHandler.sendPidActivation(false);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("PIDA", 3.0))
        {
            std::cout << "PID activated" << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("PIDA", ackFunction);
    this->flags = this->flags & (~PID_ACTIVATION);
}

void CarControl::move(const double speed, const double angle) throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("MCTL", ackFunction);
    sent = this->serialHandler.sendMove(speed, angle);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("MCTL", 3.0))
        {
            std::cout << "MCTL received" << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("MCTL", ackFunction);
}

void CarControl::brake(double angle) throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("BRAK", ackFunction);
    sent = this->serialHandler.sendBrake(angle);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("BRAK", 3.0))
        {
            std::cout << "BRAK received" << std::endl;
        }
        else
        {
            throw ConnectionException("Response was not received!", -1, 0);
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("BRAK", ackFunction);
}

void CarControl::close()
{
    this->serialHandler.close();
}