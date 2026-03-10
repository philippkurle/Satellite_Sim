#pragma once

#include <stdint.h>

namespace modes {

    // Update Perioden je Mode
    constexpr uint16_t MODECTRL_PERIOD_MS = 100;

    constexpr uint16_t DISPLAY_PERIOD_NOMINAL_MS = 200;
    constexpr uint16_t DISPLAY_PERIOD_WARN_MS = 500;
    constexpr uint16_t DISPLAY_PERIOD_SAFE_MS = 1200;

    constexpr uint16_t TELEMETRY_PERIOD_NOMINAL_MS = 200;
    constexpr uint16_t TELEMETRY_PERIOD_WARN_MS = 800;
    constexpr uint16_t TELEMETRY_PERIOD_SAFE_MS = 1500;

    // Leak Sensors
    constexpr uint16_t LEAK_PERIOD_SAFE_MS = 5000; // nur alle 5s
    constexpr uint16_t LEAK_DUTY_WARN_ON_MS = 400; // 400ms an
    constexpr uint16_t LEAK_DUTY_WARN_OFF_MS = 1600; // und 1600ms aus
}