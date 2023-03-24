#include "PWMDriver.h"
#include <mutex>

void PWMDriver::attach(uint8_t pin, uint8_t channel)
{
    auto it = findChannel(channel);
    if (it == _channels.end())
    {
        auto ch = new str_channel_t{
            .channel = channel,
            .pin = pin,
        };
        _channels.push_back(ch);

        // Initialize channels
        // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
        // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
        ledcSetup(channel, 12000, 8); // 12 kHz PWM, 8-bit resolution
    }

    // Assign led pin to channel
    ledcAttachPin(pin, channel);

    // Turn off led
    writeChannel(channel, 0);
}

void PWMDriver::detach(uint8_t pin)
{
    ledcDetachPin(pin);
    auto it = findChannelByPin(pin);
    if (it == _channels.end())
    {
        log_w("Pin %i not attached to any channel", pin);
        return;
    }

    // Turn off led
    writeChannel((*it)->channel, 0);
    _channels.erase(it);
    delete *it;
}

std::vector<PWMDriver::ptr_channel_t>::const_iterator PWMDriver::findChannel(uint8_t channel)
{
    auto isChannel = [&channel](ptr_channel_t i)
    {
        return i->channel == channel;
    };
    return std::find_if(_channels.begin(), _channels.end(), isChannel);
}

std::vector<PWMDriver::ptr_channel_t>::const_iterator PWMDriver::findChannelByPin(uint8_t pin)
{
    auto isChannel = [&pin](ptr_channel_t i)
    {
        return i->pin == pin;
    };
    return std::find_if(_channels.begin(), _channels.end(), isChannel);
}

std::vector<uint8_t> PWMDriver::getChannels()
{
    std::vector<uint8_t> result;
    for (auto it : _channels)
    {
        result.push_back(it->channel);
    }
    return result;
}

bool PWMDriver::hasChannel(uint8_t channel)
{
    auto it = findChannel(channel);
    if (it == _channels.end())
    {
        log_e("Channel %i not attached to any pin", channel);
        return false;
    }
    return true;
}

uint8_t PWMDriver::readChannel(uint8_t channel)
{
    uint32_t value = ledcRead(channel);
    if (value > 0xFF)
    {
        return 0xFF;
    }
    return value;
}

void inline PWMDriver::writeChannel(uint8_t channel, uint8_t value)
{
    // log_d("Write: %i", value);
    ledcWrite(channel, value);
}
