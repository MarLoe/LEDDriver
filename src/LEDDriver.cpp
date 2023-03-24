#include "LEDDriver.h"
#include <mutex>

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
#define LED_DRIVER_TASK_SIZE 2048
#else
#define LED_DRIVER_TASK_SIZE 768
#endif

LEDDriver::LEDDriver()
{
    _taskHandle = NULL;
}

LEDDriver::~LEDDriver()
{
    end();
}

void LEDDriver::begin(HALDriver &halDriver, BaseType_t coreId)
{
    _halDriver = &halDriver;
    _queueHandle = xQueueCreate(10, sizeof(ptr_command_t));
    xTaskCreatePinnedToCore(LEDDriver::task, "LEDDriver::task", LED_DRIVER_TASK_SIZE, this, 2, &_taskHandle, coreId);
}

void LEDDriver::end()
{
    if (_taskHandle != NULL)
    {
        vTaskDelete(_taskHandle);
        _taskHandle = NULL;
    }
    if (_queueHandle != NULL)
    {
        vQueueDelete(_queueHandle);
        _queueHandle = NULL;
    }
}

uint8_t LEDDriver::get(uint8_t channel)
{
    if (_halDriver->hasChannel(channel))
    {
        return _halDriver->readChannel(channel);
    }
    return 0;
}

bool LEDDriver::set(uint8_t channel, uint8_t value)
{
    return sendCommand(command_type::cmd_set, channel, 0, 0, 0, value, 0, 0);
}

bool LEDDriver::fade(uint8_t channel, uint8_t value, unsigned long duration)
{
    uint8_t from = get(channel);
    return fade(channel, from, value, duration);
}

bool LEDDriver::fade(uint8_t channel, uint8_t from, uint8_t value, unsigned long duration)
{
    return sendCommand(command_type::cmd_fade, channel, 0, 0, from, value, 0, duration);
}

bool LEDDriver::blink(uint8_t channel, float freq)
{
    uint32_t duration = 500 / freq; // half on / half off
    return blink(channel, duration, duration);
}

bool LEDDriver::blink(uint8_t channel, unsigned long on, unsigned long off, unsigned long delay, unsigned long timeout)
{
    return sendCommand(command_type::cmd_blink, channel, on, off, 0, 255, delay, timeout);
}

bool LEDDriver::multiBlink(uint8_t channel, uint8_t count, unsigned long on, unsigned long off, unsigned long pause, unsigned long delay, unsigned long timeout)
{
    if (count == 0)
    {
        log_w("Cannot multi blink 0 times");
        return false;
    }

    bool result = true;
    unsigned long cycle = (on + off);
    unsigned long cyclePause = ((count - 1) * cycle) + pause;
    for (int i = 0; i < count; i++)
    {
        unsigned long cycleDelay = i * cycle;
        result &= blink(channel, on, off + cyclePause, delay + cycleDelay, timeout == 0 ? 0 : timeout + cycleDelay);
    }
    return result;
}

bool LEDDriver::flicker(uint8_t channel, float freq)
{
    return flicker(channel, roundf(1000 / freq), 0);
}

bool LEDDriver::flicker(uint8_t channel, unsigned long on, unsigned long off, unsigned long delay, unsigned long timeout)
{
    return sendCommand(command_type::cmd_flicker, channel, on, off, 1, 255, delay, timeout);
}

bool LEDDriver::off(uint8_t channel)
{
    return sendCommand(command_type::cmd_off, channel, 0, 0, 0, 0, 0, 0);
}

bool LEDDriver::stop()
{
    bool result = true;
    for (auto it : _halDriver->getChannels())
    {
        result &= off(it);
    }
    return result;
}

void inline LEDDriver::taskLoop()
{
    std::vector<ptr_command_t> commands;
    while (1)
    {
        // Unload command queue
        TickType_t waitTime = commands.size() > 0 ? 10 : 5000;
        receiveCommands(&commands, waitTime);

        long t = millis();

        // Process commands
        for (auto it = commands.cbegin(); it != commands.cend();)
        {
            ptr_command_t cmd = *it;

            bool ended = false;
            if (t >= cmd->start)
            {
                switch (cmd->type)
                {
                case command_type::cmd_off:
                    ended = true;
                    _halDriver->writeChannel(cmd->channel, 0);
                    break;

                case command_type::cmd_set:
                    cmd->start = cmd->end;
                    ended = t > cmd->end;
                    _halDriver->writeChannel(cmd->channel, ended ? cmd->max : cmd->min);
                    break;

                case command_type::cmd_fade:
                    ended = cmd->end > 0 && t > cmd->end;
                    if (ended)
                    {
                        _halDriver->writeChannel(cmd->channel, cmd->max);
                    }
                    else
                    {
                        if (cmd->state == 0)
                        {
                            cmd->state = 1;
                            cmd->fraction = (cmd->max - cmd->min) / (float)(cmd->end - cmd->start);
                        }
                        int progress = t - cmd->start;
                        uint8_t value = cmd->min;
                        if (progress > 0)
                        {
                            value += roundf(progress * cmd->fraction);
                        }
                        _halDriver->writeChannel(cmd->channel, value);
                    }
                    break;

                case command_type::cmd_blink:
                    ended = cmd->end > 0 && t > cmd->end;
                    if (!ended && cmd->state == 0)
                    {
                        cmd->state = 1;
                        cmd->start += cmd->on;
                        _halDriver->writeChannel(cmd->channel, cmd->max);
                    }
                    else if (ended || cmd->state == 1)
                    {
                        cmd->state = 0;
                        cmd->start += cmd->off;
                        _halDriver->writeChannel(cmd->channel, cmd->min);
                    }
                    break;

                case command_type::cmd_flicker:
                    ended = cmd->end > 0 && t > cmd->end;
                    if (!ended && (cmd->state == 0 || cmd->off == 0))
                    {
                        cmd->state = 1;
                        cmd->start += cmd->on;
                        _halDriver->writeChannel(cmd->channel, random(cmd->min, cmd->max));
                    }
                    else if (ended || cmd->state == 1)
                    {
                        cmd->state = 0;
                        cmd->start += cmd->off;
                        _halDriver->writeChannel(cmd->channel, 0);
                    }
                    break;
                }
            }

            it = ended ? eraseCommand(&commands, it) : it += 1;
        }
    }
}

bool LEDDriver::sendCommand(command_type_t type, uint8_t channel, unsigned long on, unsigned long off, uint8_t min, uint8_t max, unsigned long start, unsigned long end)
{
    if (!_halDriver->hasChannel(channel))
    {
        return false;
    }

    auto cmd = new str_command_t{
        .type = type,
        .channel = channel,
        .min = min,
        .max = max,
        .on = on,
        .off = off,
        .start = millis() + start,
        .end = end == 0 ? 0 : millis() + start + end,
        .state = 0,
        .fraction = 0,
    };

    return xQueueSendToBack(_queueHandle, &cmd, 50 * portTICK_PERIOD_MS) == pdTRUE;
}

void LEDDriver::receiveCommands(std::vector<ptr_command_t> *commands, long timeout)
{
    ptr_command_t cmd;
    while (_queueHandle != NULL && xQueueReceive(_queueHandle, &cmd, timeout * portTICK_PERIOD_MS) == pdPASS)
    {
        // Do not wait for any subsequend commands from the queue
        timeout = 0;

        if (cmd->type == command_type::cmd_off)
        {
            // Remove all pending command for this channel
            for (auto it = commands->cbegin(); it != commands->cend();)
            {
                if (cmd->channel == (*it)->channel)
                {
                    // This command addresses a channel that has started and has no end
                    // We will remove it and thus let this command override
                    it = eraseCommand(commands, it);
                }
                else
                {
                    ++it;
                }
            }
        }
        commands->push_back(cmd);
    }
}

std::vector<LEDDriver::ptr_command_t>::const_iterator LEDDriver::eraseCommand(std::vector<ptr_command_t> *commands, std::vector<ptr_command_t>::const_iterator command)
{
    auto eraseCommand = *command;
    auto it = commands->erase(command);
    delete eraseCommand;
    return it;
}

void inline LEDDriver::task(void *param)
{
    LEDDriver *_this = (LEDDriver *)param;
    _this->taskLoop();
}
