#ifndef LORACLOCK_CLOCKCORE_H
#define LORACLOCK_CLOCKCORE_H

// I learned from reading the ezTime.h code that you can actually do computations during compile time
// and make macro wrappers that act syntactically like functions.
// This is, I presume, much better than using wrapper functions for trivial & repetitive computations
#define millisToTicks(ms) (ms/configTICK_RATE_HZ)
#define vTaskDelayMillis(ms) vTaskDelay(millisToTicks(ms))
// I learned about configTICK_RATE_HZ from: https://www.freertos.org/a00110.html

/*
 * ClockCore
 * coordinates basic functions of embedded device
 * For example, setLocation() sets the location of timezone -- only functions after connecting to wifi
 */
class ClockCore: public LoggingObject {
    const char* location;
    bool location_is_set = false;
    int wifi_conn_attempts;

public:
    explicit ClockCore(const char *location, int wifi_conn_attempts);

    void update(Timezone *tz);

    // See: https://circuitjournal.com/arduino-force-inline
    // for info on forcing inlines and the importance for highly optimized C++ programming
    inline bool isLocationSet() const  __attribute__((always_inline));

    __attribute__((unused)) void connect_to_wifi();

};

String& wl_status2str(wl_status_t status, String& conn_status_str);

#endif //LORACLOCK_CLOCKCORE_H
