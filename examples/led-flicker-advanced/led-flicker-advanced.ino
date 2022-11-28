//*******************************************************************************
//  Description:  LEDDriver example that demonstrates flickering led.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN
#define FLICKER_ON 50          // led will be on for this amount of milliseconds
#define FLICKER_OFF 10         // led will be off for this amount of milliseconds
#define FLICKER_DELAY 5000     // The amount of milliseconds before the flickering starts
#define FLICKER_DURATION 10000 // The amount of milliseconds the flickering will last

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-flicker-advanced' running");
    Serial.println("LEDDriver will make led flicker in the background.");
    Serial.println("");
    Serial.print("The flickering will start in ");
    Serial.print(FLICKER_DELAY);
    Serial.println(" milliseconds.");
    Serial.print("And continue for ");
    Serial.print(FLICKER_DURATION);
    Serial.println(" milliseconds.");

    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();

    // Flicker led attached to channel 0
    // The led will be on for FLICKER_ON ms and then off for FLICKER_OFF ms.
    // The flickering is delay (will start in) FLICKER_DELAY ms and last for FLICKER_DURATION ms.
    LED.flicker(0, FLICKER_ON, FLICKER_OFF, FLICKER_DELAY, FLICKER_DURATION);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}