#include "TimeDisplay/TimeDisplay.h"
#include "/home/ada/.auth/wifi.h"
#include "RotaryEncoder/RotaryEncoder.hpp"
#include <U8x8lib.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <ezTime.h>
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include "ClockCore.h"
#include <UniversalLogger/UniversalLogger.hpp>
#include "/home/ada/.auth/wifi.h"

void ClockCore::update(Timezone *tz) {
    if (not location_is_set) {
        while (not tz->setLocation(location)) {
            // I spent ages looking on sites like StackOverflow for how to do this
            // I almost started using TaskScheduler library or writing my own version of it for very basic scheduling,
            // But, low and behold, I peaked at the FreeRTOS page on PIO and got my answer :)
            vTaskDelayMillis(1000);
        }

        location_is_set = true;
    }
}

ClockCore::ClockCore(const char *location, int wifi_conn_attempts)
        : location(location), wifi_conn_attempts(5) {}

bool ClockCore::isLocationSet() const {
    return location_is_set;
}

__attribute__((unused)) void ClockCore::connect_to_wifi() {
    String conn_status_str;
    for (int attempts = 0; attempts <= wifi_conn_attempts; ++attempts) {
        WIFI_BEGIN();
        WiFi.waitForConnectResult();
        wl_status_t status = WiFiClass::status();
        wl_status2str(status, conn_status_str);
        //omniPrint->send(String("WiFi status: ") + conn_status_str);
        println("WiFi status:");
        println(conn_status_str);
        if (status == WL_CONNECTED) {
            println(WiFi.localIP().toString());
            // Wait a second so the user has time to see our output...
            //delay(1000);
            break;
        } else {
            println("Failure!");
            println("Retrying conn!");
            delay(5000);
            continue;
        }
    }
}

String &wl_status2str(wl_status_t status, String &conn_status_str) {
    switch (status) {
        case WL_CONNECTED:
            conn_status_str = "connected";
            break;
        case WL_CONNECTION_LOST:
            conn_status_str = "connection lost";
            break;
        case WL_CONNECT_FAILED:
            conn_status_str = "connection failed";
            break;
        case WL_DISCONNECTED:
            conn_status_str = "disconnected";
            break;
        case WL_IDLE_STATUS:
            conn_status_str = "idle";
            break;
        case WL_NO_SSID_AVAIL:
            conn_status_str = "no SSID available";
            break;
        case WL_NO_SHIELD:
            conn_status_str = "no shield available";
            break;
        case WL_SCAN_COMPLETED:
            conn_status_str = "scan completed";
            break;
        default:
            conn_status_str = "unknown: " + String(status);
    };
    return conn_status_str;
}

