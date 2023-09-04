//*******************************************************************************
//  Description:  LEDDriver example that demonstrates use of automated fading.
//  Author:       Martin Lobger
//  Copyright:    Copyright (c) 2022 Martin Lobger
//  License:      MIT License. See included LICENSE file.
//*******************************************************************************
#include "LEDDriver.h"
#include <UMS3Driver.h>

#define LED_MIN 0
#define LED_MAX 192

LEDDriver LED;
UMS3 ums3;

void wait()
{
    Serial.println("Waiting 2000ms");
    delay(2000);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Example 'led-fade' running");
    Serial.println("LEDDriver is able to fade the led from 0 (off) to 255 (full)");

    UMS3Driver *hal = new UMS3Driver(ums3);

    ums3.begin();

    // Initialize and start the LEDDriver
    LED.begin(*hal);
}

void loop()
{
    Serial.println("Fading to red");
    LED.fade(UMS3_CHANNEL_RED, LED_MIN, LED_MAX, 2000);
    wait();

    Serial.println("Fading to blue");
    LED.fade(UMS3_CHANNEL_RED, LED_MIN, 2000);
    LED.fade(UMS3_CHANNEL_BLUE, LED_MIN, LED_MAX, 2000);
    wait();

    Serial.println("Fading to green");
    LED.fade(UMS3_CHANNEL_BLUE, LED_MIN, 2000);
    LED.fade(UMS3_CHANNEL_GREEN, LED_MIN, LED_MAX, 2000);
    wait();

    LED.fade(UMS3_CHANNEL_GREEN, LED_MIN, 2000);
}