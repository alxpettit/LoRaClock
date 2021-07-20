#include <Arduino.h>
#include "RotaryEncoder/RotaryEncoder.hpp"
#include <SPI.h>
#include <Wire.h>

class MyRotaryEncoder: RotaryEncoder {
    void onRotaryEncoderPress() override {
        Serial.println("Pressed :O");
    }

    void onRotaryEncoderRelease() override {
        Serial.println("Boop! :D");
    }

    void onRotaryEncoderSpin(volatile long long _counter) override {
        Serial.println(_counter);

    }
};

MyRotaryEncoder* rotaryEncoder;

void setup() {

    rotaryEncoder = new MyRotaryEncoder();
    // Set encoder pins and attach interrupts

    // Start the serial monitor to show output
    Serial.begin(115200); // Change to 9600 for Nano, 115200 for ESP32
    delay(500);           // Wait for serial to start
    Serial.println("Start");
}

void loop() {
}
