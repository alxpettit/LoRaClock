#include <Arduino.h>
#define PIN_SW 17
#define PIN_DT 22
#define PIN_CLK 23
TaskHandle_t rotary_encoder;


class RotaryEncoder {
    const byte pin_sw, pin_dt, pin_clk;
    uint8_t table_index, store;
    // proudly stolen from: https://www.best-microcontroller-projects.com/rotary-encoder.html :P
    constexpr static const int8_t rot_enc_table[] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};
    explicit RotaryEncoder(byte _pin_sw = 17, byte _pin_dt = 22, byte _pin_clk = 23)
        : pin_sw{_pin_sw}, pin_dt{_pin_dt}, pin_clk{_pin_clk} {
        pinMode(PIN_CLK, INPUT);
        pinMode(PIN_CLK, INPUT_PULLUP);
        pinMode(PIN_DT, INPUT);
        pinMode(PIN_DT, INPUT_PULLUP);
        pinMode(PIN_SW, INPUT);
        pinMode(PIN_SW, INPUT_PULLDOWN);
    };

    int8_t read_rotary() {
        table_index <<= 2;
        if (digitalRead(PIN_DT))
    }

};

static uint16_t state = 0, counter = 0;
[[noreturn]] void PollRotaryEncoder(void *pvParameters) {
    const TickType_t xDelay = 1 / portTICK_PERIOD_MS;
    for (;;) {
        vTaskDelay(xDelay);
        state = (state << 1) | digitalRead(PIN_CLK) | 0xe000;
        if (state == 0xf000) {
            state = 0x0000;
            if (digitalRead(PIN_DT)) {
                ++counter;
            } else {
                --counter;
            }
            Serial.println(counter);
        }
    }
}

#define PIN_SW 17
#define PIN_DT 22
#define PIN_CLK 23

void setup() {
    Serial.begin(9600);

}

void loop() {
}


