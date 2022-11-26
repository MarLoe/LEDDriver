//*******************************************************************************
//  Description:  LEDDriver example that demonstrates use of manual fading.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN
#define LED_MIN 0
#define LED_MAX 255

void ledFadeUp()
{
    for (int i = LED_MIN; i <= LED_MAX; i = i + 1)
    {
        LED.set(0, i); // Set the level for the led attached to channel 0
        delay(2);
    }
}

void ledFadeDown()
{
    for (int i = LED_MAX; i >= LED_MIN; i = i - 1)
    {
        LED.set(0, i); // Set the level for the led attached to channel 0
        delay(2);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-fade-manual' running");
    Serial.println("LEDDriver is able to fade the led from 0 (off) to 255 (full)");

    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();
}

void loop()
{
    Serial.println("Fading up");
    ledFadeUp();

    Serial.println("Fading down");
    ledFadeDown();
}