#pragma once

#include <stdint.h>

namespace modes {

    // update interval depending on mode
    constexpr uint16_t MODECTRL_PERIOD_MS = 100;

    constexpr uint16_t DISPLAY_PERIOD_NOMINAL_MS = 200;
    constexpr uint16_t DISPLAY_PERIOD_WARN_MS = 500;
    constexpr uint16_t DISPLAY_PERIOD_SAFE_MS = 1200;

    constexpr uint16_t TELEMETRY_PERIOD_NOMINAL_MS = 200;
    constexpr uint16_t TELEMETRY_PERIOD_WARN_MS = 800;
    constexpr uint16_t TELEMETRY_PERIOD_SAFE_MS = 1500;

    // leak sensors
    constexpr uint16_t LEAK_PERIOD_SAFE_MS = 5000; //  5s
    constexpr uint16_t LEAK_DUTY_WARN_ON_MS = 400; // 400ms on
    constexpr uint16_t LEAK_DUTY_WARN_OFF_MS = 1600; // and 1600ms off
}