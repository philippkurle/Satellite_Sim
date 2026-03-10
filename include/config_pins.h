#pragma once

#include <Arduino.h>

namespace pins {
    
    // SENSOR PINs
    constexpr uint8_t PIN_SOLAR = A0; // MEGA PINs bis zu 53 (digital) und 54-69 (analog) --> uint8_t mögliche Werte <= 256
    constexpr uint8_t PIN_WATER = A1;

    // SERVO PIN
    constexpr uint8_t PIN_SERVO = 32;

    // BUZZER (Treiber IN1)
    constexpr uint8_t PIN_BUZZER = 36;

    // LED PINs
    constexpr uint8_t PIN_LED_R = 42;
    constexpr uint8_t PIN_LED_Y = 43;
    constexpr uint8_t PIN_LED_G = 44;
}