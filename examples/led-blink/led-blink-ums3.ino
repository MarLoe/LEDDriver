//*******************************************************************************
//  Description:  LEDDriver example that demonstrates blinking led.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"
#include <UMS3Driver.h>

LEDDriver LED;
UMS3 ums3;

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-blink' running");
    Serial.println("LEDDriver will make led blink in the background.");

    UMS3Driver *hal = new UMS3Driver(ums3);

    ums3.begin();

    // Initialize and start the LEDDriver
    LED.begin(*hal);

    // Blink led attached to channel 0 @ 0.5Hz
    // LED.blink(UMS3_CHANNEL_RED, 0.5f);
    // LED.blink(UMS3_CHANNEL_GREEN, 1.0f);
    // LED.blink(UMS3_CHANNEL_BLUE, 1.5f);

    LED.blink(UMS3_CHANNEL_GREEN, 4);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}