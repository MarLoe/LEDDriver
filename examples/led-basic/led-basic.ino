//*******************************************************************************
//  Description:  LEDDriver example that demonstrates basic use.
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
    Serial.println("Example 'led-basic' running");
    Serial.println("LEDDriver will set the level of the led.");

    PWMDriver *hal = new PWMDriver();

    // Attach led pin to channel 0
    hal->attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin(*hal);
}

void loop()
{
    LED.set(0, 20);
    delay(1000);

    LED.set(0, 100);
    delay(1000);

    LED.set(0, 255);
    delay(1000);
}