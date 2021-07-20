//
// Created by ada on 2021-07-15.
//

#ifndef LORACLOCK_LOGGINGOBJECT_H
#define LORACLOCK_LOGGINGOBJECT_H

#include <Arduino.h>
#include <U8x8lib.h>

class LoggingObject {
private:
    U8X8* display;
    HardwareSerial* serial;
protected:
    void println(const String &output);

public:

    void setDisplay(U8X8* new_display);
    void setSerial(HardwareSerial* new_serial);
    explicit LoggingObject(U8X8* display = nullptr, HardwareSerial* serial = nullptr);
};


#endif //LORACLOCK_LOGGINGOBJECT_H
