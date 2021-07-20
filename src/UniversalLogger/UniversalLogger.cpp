//
// Created by ada on 2021-06-06.
//

#include "UniversalLogger.hpp"

#define LOG_TO_SERIAL 1
#define LOG_TO_SCREEN 2
#define LOG_TO_LORA 4
#define LOG_DEBUG 8
#define LOG_ERROR 16
#define LOG_WARNING 32
#define LOG_INFO 64
#define LOG_DEFAULT LOG_ERROR & LOG_WARNING & LOG_TO_SERIAL
#define LOG_ALL_LEVELS LOG_DEBUG & LOG_ERROR & LOG_WARNING & LOG_INFO

UniversalLogger::UniversalLogger(OLED* oled, HardwareSerial* hwserial): _oled{oled}, _hwserial{hwserial} {

}

void UniversalLogger::log(const char *input) const {
    if (log_conf | LOG_TO_LORA) {
    }
    if (log_conf | LOG_TO_SCREEN) {
        _oled->println(input);
    }
    if (log_conf | LOG_TO_SERIAL) {
        _hwserial->println(input);
    }
}

void UniversalLogger::error(const char *input) const {
    if (log_conf | LOG_ERROR) {
        log(input);
    }
}

void UniversalLogger::info(const char *input) const {
    if (log_conf | LOG_INFO) {
        log(input);
    }
}

void UniversalLogger::warning(const char *input) const {
    if (log_conf | LOG_WARNING) {
        log(input);
    }
}

void UniversalLogger::report() {

}

