#pragma once

#include <stdint.h>

struct ServoConfig {
    uint8_t center_deg; // Neutralstellung
    uint8_t amplitude_deg; // Ausschlag links/rechts
    uint16_t step_ms; // Zeit pro Schritt
    uint8_t repeats; // wie oft links-rechts drehen
};

inline constexpr ServoConfig SERVO_CONFIG { // constexpr -> kann zur Compile-Zeit ausgewertet werden; {} -> Initialisierung mit default Werten
    .center_deg = 90,
    .amplitude_deg = 25,
    .step_ms = 180,
    .repeats = 2
};
