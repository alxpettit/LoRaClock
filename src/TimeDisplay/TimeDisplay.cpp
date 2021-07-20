//
// Created by ada on 2021-07-18.
//

#include "TimeDisplay.h"

void TimeDisplay::update() const {
    if (tz) {
        // TODO: consider replacing function calls with a single breakTime() call
        // TODO: skip sprintf & display if value has not changed!
        u8x8->setFont(u8x8_font_chroma48medium8_r);
        u8x8->drawString(0, 0, tz->dateTime("Y-m-d").c_str());
        u8x8->drawString(0, 6, tz->dateTime("D, M").c_str());
        u8x8->setFont(u8x8_font_courB18_2x3_f);
        u8x8->drawString(0, 2, tz->dateTime("H:i:s").c_str());
    }
}

void TimeDisplay::loop_time() {
    unix_time = tz->now();
    if (unix_time != old_unix_time) {
        update();
        u8x8->display();
        Serial.println(tz->dateTime());

        old_unix_time = unix_time;
    }
}

TimeDisplay::TimeDisplay(U8X8 *u8x8, Timezone *tz) : u8x8(u8x8), tz(tz) {
    //Serial.println(tz->dateTime());
}
