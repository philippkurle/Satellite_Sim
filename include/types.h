#pragma once

#include <stdint.h>

// Typ/Bauplan mit folgenden Werten in den Feldern
struct SensorData {
    uint16_t solar_data_raw = 0;
    bool pv_low_warn = false;
    bool pv_low_safe = false;

    uint16_t water_data_raw = 0;
    bool water_suspect = false;
    bool water_confirm = false;
};

enum class Mode : uint8_t {
    NOMINAL = 0,
    WARN = 1,
    SAFE = 2,
};

enum class SecurityUiState : uint8_t {
    WAIT_RFID,
    WAIT_CODE,
    LOCKOUT,
    AUTH_OK
};