#pragma once

#include <Arduino.h>

namespace pins {
    
    // SENSOR PINs
    constexpr uint8_t PIN_SOLAR = A0; // MEGA PINs: up to 53 (digital) and 54-69 (analog)
    constexpr uint8_t PIN_WATER = A1; // uint8_t: 256 possible values = 0-255, unsigned: no negative values

    // SERVO PIN
    constexpr uint8_t PIN_SERVO = 32;

    // BUZZER (driver IN1)
    constexpr uint8_t PIN_BUZZER = 36;

    // LED PINs
    constexpr uint8_t PIN_LED_R = 42;
    constexpr uint8_t PIN_LED_Y = 43;
    constexpr uint8_t PIN_LED_G = 44;
}