//
// Created by ada on 2021-07-18.
//

#ifndef LORACLOCK_TIMEDISPLAY_H
#define LORACLOCK_TIMEDISPLAY_H


#include <U8x8lib.h>
#include <ezTime.h>

class TimeDisplay {
    char date_buffer_str[16] = "";
    char time_buffer_str[16] = "";
    char footer_buffer_str[16] = "";
    char day_short_str[5] = "";
    char month_short_str[5] = "";
    volatile time_t unix_time{};
    volatile time_t old_unix_time{};
    U8X8* u8x8;

public:
    Timezone* tz = nullptr;
    TimeDisplay(U8X8 *u8x8, Timezone *tz);
    // this version of update was specifically for TimeLib.h -- now using EZtime, so we will be rewriting this!
//    void update() {
//        // TODO: consider replacing function calls with a single breakTime() call
//        strncpy(day_short_str, dayShortStr(day()), 5);
//        strncpy(month_short_str, monthShortStr(month()), 5);
//        // TODO: skip sprintf & display if value has not changed!
//        snprintf(date_buffer_str, 16, "%02d-%02d-%02d", year(), month(), day());
//        u8x8.setFont(u8x8_font_chroma48medium8_r);
//        u8x8.drawString(0, 0, date_buffer_str);
//        snprintf(footer_buffer_str, 16, "%s, %s", month_short_str, dayShortStr(day()));
//        u8x8.drawString(0, 6, footer_buffer_str);
//        snprintf(time_buffer_str, 16, "%02d:%02d:%02d", hour(), minute(), second());
//        u8x8.setFont(u8x8_font_courB18_2x3_f);
//        u8x8.drawString(0, 2, time_buffer_str);
//    }

    void update() const;

    void loop_time();
};



#endif //LORACLOCK_TIMEDISPLAY_H
