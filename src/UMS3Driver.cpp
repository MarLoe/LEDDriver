#include "PWMDriver.h"
#include <mutex>
#include "UMS3Driver.h"

#if UMS3_PRESENT

std::vector<uint8_t> UMS3Driver::getChannels()
{
    std::vector<uint8_t> result = {0, 1, 2};
    return result;
}

bool UMS3Driver::hasChannel(uint8_t channel)
{
    auto channels = getChannels();
    return std::find(channels.begin(), channels.end(), channel) != channels.end();
}

uint8_t UMS3Driver::readChannel(uint8_t channel)
{
    switch (channel)
    {
    case UMS3_CHANNEL_RED:
        return _red;
    case UMS3_CHANNEL_GREEN:
        return _green;
    case UMS3_CHANNEL_BLUE:
        return _blue;
    }
    return 0;
}

void inline UMS3Driver::writeChannel(uint8_t channel, uint8_t value)
{
    switch (channel)
    {
    case UMS3_CHANNEL_RED:
        _red = value;
        break;
    case UMS3_CHANNEL_GREEN:
        _green = value;
        break;
    case UMS3_CHANNEL_BLUE:
        _blue = value;
        break;
    default:
        return;
    }
    _ums3->setPixelColor(_red, _green, _blue);
    _ums3->setPixelPower(_red || _green || _blue);
}

#endif