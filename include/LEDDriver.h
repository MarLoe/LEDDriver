#ifndef __LEDDRIVER_H__
#define __LEDDRIVER_H__

#include <Arduino.h>
#include <vector>

class LEDDriver
{
public:
    LEDDriver();
    ~LEDDriver();

    void begin(BaseType_t coreId = tskNO_AFFINITY);
    void end();

    void attach(uint8_t pin, uint8_t channel);
    void detach(uint8_t pin);

    uint8_t get(uint8_t channel);
    bool set(uint8_t channel, uint8_t value);

    bool fade(uint8_t channel, uint8_t value, uint32_t duration);
    bool fade(uint8_t channel, uint8_t from, uint8_t value, uint32_t duration);

    bool blink(uint8_t channel, float freq);
    bool blink(uint8_t channel, uint32_t on, uint32_t off, unsigned long delay = 0, unsigned long timeout = 0);

    bool multiBlink(uint8_t channel, uint8_t count, uint32_t on, uint32_t pause, uint32_t duration, unsigned long delay = 0, unsigned long timeout = 0);

    bool off(uint8_t channel);
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