//
// Created by ada on 2021-05-24.
//

#include <Arduino.h>
#include "RotaryEncoder.hpp"


RotaryEncoder::RotaryEncoder(uint8_t _pin_sw, uint8_t _pin_dt, uint8_t _pin_clk, bool _counter_rollover) :
    pin_sw{_pin_sw}, pin_dt{_pin_dt}, pin_clk{_pin_clk}, counter_rollover{_counter_rollover}
{
    pinMode(pin_sw, INPUT_PULLUP);
    pinMode(pin_dt, INPUT_PULLUP);
    pinMode(pin_clk, INPUT_PULLUP);
    attachInterruptArg(digitalPinToInterrupt(pin_dt),
            // calls read_enocder with an argument pointing to us
            // must be static to pass as callback
                       reinterpret_cast<void (*)(void *)>(RotaryEncoder::static_read_encoder), this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(pin_clk),
                       reinterpret_cast<void (*)(void *)>(RotaryEncoder::static_read_encoder), this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(pin_sw),
                       reinterpret_cast<void (*)(void *)>(RotaryEncoder::static_read_switch), this, CHANGE);
}

void RotaryEncoder::static_read_switch(RotaryEncoder &re) {
    re.read_switch();
}

void RotaryEncoder::static_read_encoder(RotaryEncoder &re) {
    re.read_encoder();
}

void RotaryEncoder::read_switch() {
    unsigned long millis_since_click = (millis() - millis_at_click);
    if (millis_since_click > 100) {
        if (digitalRead(pin_sw)) {
            millis_at_click = millis();
            onRotaryEncoderRelease();

        } else {
            onRotaryEncoderPress();
        }
    }
}

// virtual means child class redefines
void RotaryEncoder::onRotaryEncoderPress() {

}

void RotaryEncoder::onRotaryEncoderRelease() {

}

void RotaryEncoder::onRotaryEncoderSpin(volatile long long _counter) {

}

void RotaryEncoder::update_accel() {
    // 200 millis since
    unsigned long millis_value = millis();
    if ((millis_value - millis_at_increment) < 100) {
        accel_accel_amount += accel_accel_inc;
        accel_amount *= accel_accel_amount;
        if (accel_amount > max_accel_amount) {
            accel_amount = max_accel_amount;
        }
    } else {
        accel_amount = 1;
        accel_accel_amount = default_accel_amount;
    }
}

void RotaryEncoder::read_encoder() {
    // Encoder interrupt routine for both pins. Updates counter
    // if they are valid and have rotated a full indent


    static uint8_t old_AB = 3;  // Lookup table index
    static int8_t encval = 0;   // Encoder value

    // Lookup table for valid states
    // 1 = valid, 0 = no change, -1 = invalid state
    constexpr static const int8_t enc_states[]  = {0, -1 ,1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    old_AB <<= 2;  // Remember previous state

    if (digitalRead(pin_dt)) old_AB |= 0x02; // Add current state of pin A
    if (digitalRead(pin_clk)) old_AB |= 0x01; // Add current state of pin B

    encval += enc_states[( old_AB & 0x0f )];

    // Update counter if encoder has rotated a full indent, that is at least 4 steps
    if (encval > 3) {        // Four steps forward
        update_accel();
        millis_at_increment = millis();
        counter += accel_amount;              // Increase counter
        if (counter > max_counter) {
            if (counter_rollover) {
                counter = min_counter;
            } else {
                counter = max_counter;
            }
        }
        onRotaryEncoderSpin(counter);
        encval = 0;
    }
    else if (encval < -3) {  // Four steps backwards
        update_accel();
        millis_at_increment = millis();
        counter -= accel_amount;               // Decrease counter
        onRotaryEncoderSpin(counter);
        encval = 0;
        if (counter < min_counter) {
            if (counter_rollover) {
                counter = max_counter;
            } else {
                counter = min_counter;
            }
        }
    }
}

int RotaryEncoder::getMaxCounter() const {
    return max_counter;
}

void RotaryEncoder::setMaxCounter(int maxCounter) {
    max_counter = maxCounter;
    if (counter > max_counter) {
        if (counter_rollover) {
            counter = min_counter;
        } else {
            counter = max_counter;
        }
    }
}

int RotaryEncoder::getMinCounter() const {
    return min_counter;
}

void RotaryEncoder::setMinCounter(int minCounter) {
    min_counter = minCounter;
    if (counter < min_counter) {
        if (counter_rollover) {
            counter = max_counter;
        } else {
            counter = min_counter;
        }
    }
}
