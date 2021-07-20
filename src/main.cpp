// Stage 1:
// TODO: finish UniversalLogger -- must still add support for changing settings in flag byte, and logging to lora.
// TODO: remove the #define USE_* stuff -- makes things too verbose.
// TODO: put logging everywhere!!! so we can debug our crashing issue.
// TODO: make it easy to disable each subclass during runtime -- has advantages for debugging from compile time too,
// TODO: and can later be used by user for e.g. disabling WiFi connecting mid-run.
// Stage 2:
// TODO: rework to use eztime for formatting time string.
// TODO: rework clock display -- externalize clock display class into separate directory with .cpp & .hpp.
// TODO: get time code to be robust -- long-term testing.
// Stage 3:
// TODO: Coordination will likely be done by central computer, e.g. raspberry pi, running Python -- with lower level behavior
// TODO: controlled by LoRa boards themselves -- e.g., flashing calendar display, rendering, etc.
// TODO: volatile storage limits feasibility of smarter firmware. May want to go with events being tracked in LoRa core,
// TODO: and then being broadcast by core to all nodes when relevant -- the nodes are then directed via metadata to do specific things, right as the event is happening.
// Will send messages both through LoRa and WiFi, so that all nodes will get it as long as at least one comm method is available.

#define WIFI
#define EZTIME_MAX_DEBUGLEVEL_INFO
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <ezTime.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <U8x8lib.h>
#include "RotaryEncoder/RotaryEncoder.hpp"
#include "/home/ada/.auth/wifi.h"
#include "TimeDisplay/TimeDisplay.h"
#include "ClockCore/ClockCore.h"


// Declarations go here...
class MyRotaryEncoder;
class TimeDisplay;

void println(const String& output);

// Here's where we put our class pointers...

MyRotaryEncoder* rotaryEncoder = nullptr;
U8X8_SSD1306_128X64_NONAME_SW_I2C* u8x8 = nullptr;
Timezone* tz = nullptr;
TimeDisplay* timeDisplay = nullptr;
ClockCore* clockCore = nullptr;
//NTPClient *timeClient;


TaskHandle_t taskHandleDoWiFiConnect;

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

// TODO:
// split up task into wifi connect task, and sync/setLocation tasks
// that way we can resync in the event our connection lasts a long time :)
[[noreturn]] void taskDoWiFiConnect(void* param) {
    for(;;) {
        if (not WiFi.isConnected()) {
            clockCore->connect_to_wifi();
            waitForSync();
            clockCore->update(tz);
            vTaskSuspend(taskHandleDoWiFiConnect);
            // TODO: replace waitForSync() with something that doesn't hog FreeRTOS cycles
        }
        vTaskDelayMillis(1000);
    }
}


void onWiFiEvent(WiFiEvent_t event) {
//    switch (event) {
//        case SYSTEM_EVENT_STA_DISCONNECTED:
//            vTaskResume(taskHandleDoWiFiConnect);
//            break;
//    }
}

// not actually unused, but clion won't understand...
//__attribute__((unused))
void setup() {
    // Initialize basic I/O systems
    Serial.begin(115200);
    u8x8 = new U8X8_SSD1306_128X64_NONAME_SW_I2C(15, 4, 16);
    u8x8->begin();
    u8x8->setFont(u8x8_font_chroma48medium8_r);

    //WiFi.onEvent(onWiFiEvent, SYSTEM_EVENT_AP_STADISCONNECTED);
    xTaskCreatePinnedToCore(taskDoWiFiConnect, "doWiFiConnect", 10000, NULL,
                            1, &taskHandleDoWiFiConnect, 1);
    rotaryEncoder = new MyRotaryEncoder;
    tz = new Timezone();
    timeDisplay = new TimeDisplay(u8x8, tz);
    clockCore = new ClockCore("America/Los_Angeles", 5);
    // Don't use display -- will create gibberish
    //clockCore->setDisplay(u8x8);
    clockCore->setSerial(&Serial);
    //const long utcOffsetSec = -25200; // 7 hour offset
    setInterval(240);
    WiFi.onEvent(onWiFiEvent);


//    WiFiUDP ntpUDP;
//    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", utcOffsetSec);
//    timeClient->begin();
//    timeClient->update();
//    // Set TimeLib global time value to that retrieved by NTPClient
//    setTime((time_t)timeClient->getEpochTime());
    u8x8->clearDisplay();
}

// not actually unused, but clion won't understand...
//__attribute__((unused))
void loop() {
    timeDisplay->loop_time();
    // loop_rotary_encoder();
    // loop_oled();
    // loop_wifi();
    // eztime handles this for us
//    // 100 seconds after every hour, attempt to update time over NTP
//    if (unix_time % 3600 == 100) {
//        timeClient->update();
//        setTime((time_t) timeClient->getEpochTime());
//    }
}