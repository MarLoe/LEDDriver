//*******************************************************************************
//  Description:  LEDDriver example that demonstrates multi blinking led.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN

#define BLINK_COUNT 3
#define BLINK_ON 250     // led will be on for this amount of milliseconds
#define BLINK_OFF 500    // led will be off for this amount of milliseconds
#define BLINK_PAUSE 5000 // The amount of milliseconds before the blinking starts

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-blink-multi' running");
    Serial.println("LEDDriver will make led blink in the background.");

    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();

    // Blink led attached to channel 0 @ 0.5Hz
    // LED.blink(0, 250, 250);
    LED.multiBlink(0, BLINK_COUNT, BLINK_ON, BLINK_OFF, BLINK_PAUSE);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}