#ifndef __LEDDRIVER_H__
#define __LEDDRIVER_H__

#include <Arduino.h>
#include <vector>

class LEDDriver
{
public:
    LEDDriver();
    ~LEDDriver();

    /// @brief Will initialse the LEDDriver and create a task for running and controlling LEDs in the background.
    /// @param coreId The core on wich the task shall run.
    void begin(BaseType_t coreId = tskNO_AFFINITY);

    /// @brief Deinitialize the LEDDriver and stop the task controlling the LEDs.
    void end();

    /// @brief Attach a pin to the given channel.
    /// @note Please take note of the channels used, so they do not interfear with the use of PWM channels for e.g. servo motors.
    /// @param pin The pin number to attach (e.g. LED_BUILTIN or GPIO_NUM_2)
    /// @param channel The channel to attach the pin to. Typically 16 (0-15) channels are available.
    void attach(uint8_t pin, uint8_t channel);

    /// @brief Detach a pin from the channel it was attached to.
    /// @param pin The pin number to detach.
    void detach(uint8_t pin);


    /// @brief Get the current level of the channel. Can be used to read the level of the LED attache to the channel.
    /// @param channel The channel to get the level from.
    /// @return The current level of the channel.
    /// @see set
    uint8_t get(uint8_t channel);

    /// @brief Set the level for the given channel.
    /// @param channel The channel to set the level for.
    /// @param value The level between 0-255, where 0 (zero) is off and 255 is full on.
    /// @return If success true, else false
    bool set(uint8_t channel, uint8_t value);


    /// @brief Fade the LED for the given channel. The LED will fade from the current level to value. I can fade both up and down.
    /// @param channel The channel to fade.
    /// @param value The level to fade to.
    /// @param duration The number of milliseconds the fade should take (e.g. 500 for half a second).
    /// @return If success true, else false
    bool fade(uint8_t channel, uint8_t value, unsigned long duration);

    /// @brief Fade the LED for the given channel. The LED will fade between the levels provided. I can fade both up and down.
    /// @param channel The channel to fade.
    /// @param from The level to fade from.
    /// @param value The level to fade to.
    /// @param duration The number of milliseconds the fade should take (e.g. 500 for half a second).
    /// @return If success true, else false
    bool fade(uint8_t channel, uint8_t from, uint8_t value, unsigned long duration);


    /// @brief Blink the LED on (level 255) and off (level 0) for the given channel. The LED will blink until off() or stop() is called.
    /// @param channel The channel to blink.
    /// @param freq The frequency given in Hz (e.g. 5 to blink 5 times per second or 0.25 to blink every forth second)
    /// @return If success true, else false
    bool blink(uint8_t channel, float freq);
    
    /// @brief Blink the LED on (level 255) and off (level 0) for the given channel.
    /// @param channel The channel to blink.
    /// @param on The number of milliseconds the led is turned on (level 255).
    /// @param off  The number of milliseconds the led is turned off (level 0).
    /// @param delay The number of milliseconds to delay before the blinking start. If 0 (zero) the blinking will start immediately.
    /// @param timeout The number of milliseconds the blinking should last. If 0 (zero) the blinking will not stop until off() or stop() is called.
    /// @return If success true, else false
    bool blink(uint8_t channel, unsigned long on, unsigned long off, unsigned long delay = 0, unsigned long timeout = 0);

    /// @brief Will turn of the LED attached to the given channel and thus stop any background blinking or fading.
    /// @param channel 
    /// @return If success true, else false
    bool off(uint8_t channel);

    /// @brief Will turn off all channels with attached pins
    /// @return If all channels succesfull turned off then true, else false
    /// @see off
    bool stop();

protected:
    TaskHandle_t _taskHandle;
    QueueHandle_t _queueHandle;

    typedef struct str_channel
    {
        // TODO: Handling multiple pins on same channel
        uint8_t channel;
        uint8_t pin;
    } str_channel_t, *ptr_channel_t;

    typedef enum command_type : uint8_t
    {
        cmd_off,
        cmd_set,
        cmd_fade,
        cmd_blink,
    } command_type_t;

    typedef struct __attribute__((packed, aligned(1))) str_command
    {
        command_type_t type;
        uint8_t channel;
        uint8_t min;
        uint8_t max;
        uint32_t on;
        uint32_t off;
        unsigned long start;
        unsigned long end;
        uint32_t state;
        float fraction;
    } str_command_t, *ptr_command_t;

    void taskLoop();

    bool sendCommand(command_type_t type, uint8_t channel, uint32_t on, uint32_t off, uint8_t min, uint8_t max, unsigned long start, unsigned long end);
    void receiveCommands(std::vector<ptr_command_t> *commands, long timeout);
    std::vector<ptr_command_t>::const_iterator eraseCommand(std::vector<ptr_command_t> *commands, std::vector<ptr_command_t>::const_iterator command);

    std::vector<ptr_channel_t> _channels;
    std::vector<ptr_channel_t>::const_iterator findChannel(uint8_t channel);
    std::vector<ptr_channel_t>::const_iterator findChannelByPin(uint8_t pin);

    uint8_t readChannel(uint8_t channel);
    void writeChannel(uint8_t channel, uint8_t value);

protected:
    static void task(void *param);
};

extern LEDDriver LED;

#endif