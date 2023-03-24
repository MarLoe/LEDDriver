#ifndef __UMS3DRIVER_H__
#define __UMS3DRIVER_H__

#include <Arduino.h>
#include <vector>
#include <HALDriver.h>

#if defined(ARDUINO_TINYS3) || defined(ARDUINO_PROS3) || defined(ARDUINO_FEATHERS3)
#define UMS3_PRESENT 1
#else
#define UMS3_PRESENT 0
#endif

#if UMS3_PRESENT
#include <UMS3.h>

#define UMS3_CHANNEL_RED 0
#define UMS3_CHANNEL_GREEN 1
#define UMS3_CHANNEL_BLUE 2

class UMS3Driver : public HALDriver
{
public:
    UMS3Driver(UMS3 &ums3) : UMS3Driver(&ums3){};
    UMS3Driver(UMS3 *ums3) : _ums3(ums3){};

protected:
    virtual std::vector<uint8_t> getChannels();
    virtual bool hasChannel(uint8_t channel);
    virtual uint8_t readChannel(uint8_t channel);
    virtual void writeChannel(uint8_t channel, uint8_t value);

private:
    UMS3 *_ums3;
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
};

#endif

#endif