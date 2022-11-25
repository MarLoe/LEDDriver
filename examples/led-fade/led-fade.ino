//*******************************************************************************
//  Description:  LEDDriver example that demonstrates use of automated fading.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"

// It is assumed that your board has a builtin LED connected to pin LED_BUILTIN.
// If not, replace LED_BUILTIN with the correct pin number and/or connect an external LED.
#define LED_PIN LED_BUILTIN
#define LED_MIN 16
#define LED_MAX 192

void setup()
{
    Serial.begin(115200);
    Serial.println("\nExample 'led-fade' running");
    Serial.println("\nLEDDriver is able to fade the led from 0 (off) to 255 (full)");

    // Attach led pin to channel 0
    LED.attach(LED_PIN, 0);

    // Initialize and start the LEDDriver
    LED.begin();
}

void loop()
{
    Serial.println("Fading up");

    // Fade led up from LED_MIN to LED_MAX in the background (duration 2000ms)
    LED.fade(0, LED_MIN, LED_MAX, 2000);

    Serial.println("Waiting 2000ms");
    delay(2000);

    Serial.println("Fading down");
    // Fade led down to LED_MIN from current value (duration 500ms)
    LED.fade(0, LED_MIN, 500);

    Serial.println("Waiting 2000ms");
    delay(2000);
}