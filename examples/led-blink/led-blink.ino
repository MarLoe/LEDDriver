//*******************************************************************************
//  Description:  LEDDriver example that demonstrates blinking led.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN

LEDDriver LED;

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-blink' running");
    Serial.println("LEDDriver will make led blink in the background.");

    PWMDriver *hal = new PWMDriver();

    // Attach led pin to channel 0
    hal->attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin(*hal);

    // Blink led attached to channel 0 @ 0.5Hz
    LED.blink(0, 0.5f);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}