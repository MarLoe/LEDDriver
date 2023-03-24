#ifndef __PWMDRIVER_H__
#define __PWMDRIVER_H__

#include <Arduino.h>
#include <vector>
#include <HALDriver.h>

class PWMDriver : public HALDriver
{
public:
    /// @brief Attach a pin to the given channel.
    /// @note Please take note of the channels used, so they do not interfear with the use of PWM channels for e.g. servo motors.
    /// @param pin The pin number to attach (e.g. LED_BUILTIN or GPIO_NUM_2)
    /// @param channel The channel to attach the pin to. Typically 16 (0-15) channels are available.
    virtual void attach(uint8_t pin, uint8_t channel);

    /// @brief Detach a pin from the channel it was attached to.
    /// @param pin The pin number to detach.
    virtual void detach(uint8_t pin);

protected:
    typedef struct str_channel
    {
        // TODO: Handling multiple pins on same channel
        uint8_t channel;
        uint8_t pin;
    } str_channel_t, *ptr_channel_t;

    std::vector<ptr_channel_t> _channels;
    std::vector<ptr_channel_t>::const_iterator findChannel(uint8_t channel);
    std::vector<ptr_channel_t>::const_iterator findChannelByPin(uint8_t pin);

protected:
    virtual std::vector<uint8_t> getChannels();
    virtual bool hasChannel(uint8_t channel);
    virtual uint8_t readChannel(uint8_t channel);
    virtual void writeChannel(uint8_t channel, uint8_t value);
};

#endif