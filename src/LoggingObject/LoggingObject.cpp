//
// Created by ada on 2021-07-15.
//

#include "LoggingObject.h"


LoggingObject::LoggingObject(U8X8 *display, HardwareSerial *serial) : display(nullptr), serial(nullptr) {


}

void LoggingObject::println(const String &output) {
    if (serial != nullptr) {
        serial->println(output);
    }
    if (display != nullptr) {
        display->println(output);
    }
}

void LoggingObject::setDisplay(U8X8 *new_display) {
    display = new_display;
}

void LoggingObject::setSerial(HardwareSerial *new_serial) {
    serial = new_serial;
}
