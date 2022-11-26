//*******************************************************************************
//  Description:  LEDDriver example that demonstrates advanced blinking led.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN

#define BLINK_ON 500         // led will be on for this amount of milliseconds
#define BLINK_OFF 100        // led will be off for this amount of milliseconds
#define BLINK_DELAY 5000     // The amount of milliseconds before the blinking starts
#define BLINK_DURATION 10000 // The amount of milliseconds the blinking will last

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-blink-advanced' running");
    Serial.println("LEDDriver will make led blink in the background.");
    Serial.println("");
    Serial.print("The blinking will start in ");
    Serial.print(BLINK_DELAY);
    Serial.println(" milliseconds.");

    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();

    // Blink led attached to channel 0
    // The led will be on for BLINK_ON ms and then off for BLINK_ON ms.
    // The blinking is delay (will start in) 5000ms and last for 10000ms.
    LED.blink(0, BLINK_ON, BLINK_OFF, BLINK_DELAY, BLINK_DURATION);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}