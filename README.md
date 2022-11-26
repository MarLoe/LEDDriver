# LEDDriver  

[![GitHub release](https://img.shields.io/github/release/marloe/LEDDriver.svg)](https://github.com/marloe/LEDDriver/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/marloe/LEDDriver/v0.1.0.svg)](https://github.com/marloe/LEDDriver/compare/v0.1.0...main)

Arduino library for controlling standard LEDs using pwm for duty cycling.

LEDDriver provides an easy approach to controlling LEDs using built in PWM functionality. LEDDriver is used much like other libraries for Arduino.


### Example

```cpp
// Attach led on pin LED_BUILTIN to channel
LED.attach(LED_BUILTIN, 0);
// Start and initialize the LED libray
LED.begin();
// Set the led attached to channel 0 to full duty cycle (on)
LED.set(0, 255);
// Set the led attached to channel 0 to half duty cycle (faded)
LED.set(0, 128);
```   


### Included examples

- [`led-basic.ino`](examples/led-basic/led-basic.ino) - Demonstrates basic use
- [`led-blink.ino`](examples/led-blink/led-blink.ino) - Demonstrates using blink (executed in the background)
- [`led-fade.ino`](examples/led-fade/led-fade.ino) - Demonstrates how to fade the led to a specific level
- [`led-fade-manual.ino`](examples/led-fade-manual/led-fade-manual.ino) - Demonstrates how to manualy control the fade level of a led


