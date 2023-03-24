#ifndef __HALDRIVER_H__
#define __HALDRIVER_H__

#include <Arduino.h>
#include <vector>

class HALDriver
{
    friend class LEDDriver;

protected:
    virtual std::vector<uint8_t> getChannels() = 0;
    virtual bool hasChannel(uint8_t channel) = 0;
    virtual uint8_t readChannel(uint8_t channel) = 0;
    virtual void writeChannel(uint8_t channel, uint8_t value) = 0;
};

#endif