#pragma once

#include <stdint.h>

namespace thresholds {

    // Parameter für Hysterese
    
    // Leak Water
    constexpr uint16_t WATER_SUSPECT_ON  = 220;
    constexpr uint16_t WATER_SUSPECT_OFF = 180;

    constexpr uint16_t WATER_CONFIRM_ON  = 320;
    constexpr uint16_t WATER_CONFIRM_OFF = 260;

    // PV-Werte
    constexpr uint16_t PV_WARN_ON = 370;
    constexpr uint16_t PV_WARN_OFF = 400;

    constexpr uint16_t PV_SAFE_ON = 300;
    constexpr uint16_t PV_SAFE_OFF= 330;
}