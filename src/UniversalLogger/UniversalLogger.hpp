//
// Created by ada on 2021-06-06.
//

#ifndef LORACLOCK_UNIVERSALLOGGER_HPP
#define LORACLOCK_UNIVERSALLOGGER_HPP

#include <U8x8lib.h>

#define OLED U8X8_SSD1306_128X64_NONAME_SW_I2C

class UniversalLogger {
    OLED* _oled;
    HardwareSerial* _hwserial;
    int log_conf;
public:
    explicit UniversalLogger(OLED* oled = nullptr, HardwareSerial* hwserial = nullptr);
    void log(const char* input) const;
    void error(const char* input) const;
    void info(const char* input) const;
    void warning(const char* input) const;
    void report();
};


#endif //LORACLOCK_UNIVERSALLOGGER_HPP
