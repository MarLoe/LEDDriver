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

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-flicker' running");
    Serial.println("LEDDriver will make led flicker in the background.");
    
    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();                
    
    // Flicker led attached to channel 0 @ 2Hz
    LED.flicker(0, 2);
}

void loop()
{
    // Nothing to do here - LEDDriver have created a task that runs in the background
}