//
// Created by ada on 2021-05-24.
//

#ifndef LORACLOCK_ROTARYENCODER_H
#define LORACLOCK_ROTARYENCODER_H

#include <Arduino.h>
#include <LoggingObject/LoggingObject.h>

class RotaryEncoder: public LoggingObject {
protected:
    // Some bits heavily "inspired" by
    // https://garrysblog.com/2021/03/20/reliably-debouncing-rotary-encoders-with-arduino-and-esp32/

    // 'volatile' keyword is one I just learned about!
    // Tells compiler that this value can change at any time,
    // which reduces caching (and thus, malignant optimization)
    volatile long long counter = 0;
    volatile double accel_amount = 1;
    double default_accel_amount = 1.5;
    double max_accel_amount = 10;
    double accel_accel_inc = 0.1;
    volatile double accel_accel_amount = default_accel_amount;
    volatile unsigned long millis_at_increment = 0;
    volatile unsigned long millis_at_click = 0;
    int max_counter = 256;
public:
    int getMaxCounter() const;

    int getMinCounter() const;

    void setMinCounter(int minCounter);

    void setMaxCounter(int maxCounter);

private:
    int min_counter = 0;
//    void (*onRotaryEncoderSpin)(int);
//    void (*onRotaryEncoderPress)();
//    void (*onRotaryEncoderRelease)();

    const uint8_t pin_sw, pin_dt, pin_clk; // pins to access
    bool counter_rollover;
public:


    explicit RotaryEncoder(
            uint8_t _pin_sw = 17, uint8_t _pin_dt = 22, uint8_t _pin_clk = 23, bool _counter_rollover = true);
    static void static_read_switch(RotaryEncoder &re);

    static void static_read_encoder(RotaryEncoder &re);
private:

    void read_switch();

    // virtual means child class redefines
    virtual void onRotaryEncoderPress();

    virtual void onRotaryEncoderRelease();

    virtual void onRotaryEncoderSpin(volatile long long _counter);

    void update_accel();

    void read_encoder();
};


#endif //LORACLOCK_ROTARYENCODER_H
