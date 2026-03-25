#pragma once

#include <stdint.h>

struct ServoConfig {
    uint8_t center_deg; // center position
    uint8_t amplitude_deg; // turning left/right
    uint16_t step_ms; // time per step
    uint8_t repeats; // how often turn left/right
};

// inline: define in header file in comliance with One Definition Rule
// constexpr: value/result computed during compilation, not runtime
inline constexpr ServoConfig SERVO_CONFIG {
    .center_deg = 90,
    .amplitude_deg = 25,
    .step_ms = 180,
    .repeats = 2
};
