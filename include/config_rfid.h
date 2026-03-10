#pragma once

#include <stdint.h>

namespace cfg {

    struct RfidConfig {
        uint8_t ss_pin; // RC522 SDA/SS -> gleiche Funktion
        uint8_t rst_pin; // RC522 RST
        uint16_t poll_interval_ms; // Polling Intervall
    };

    inline constexpr RfidConfig RFID_CONFIG {
        .ss_pin = 53,
        .rst_pin = 49,
        .poll_interval_ms = 80, // 80ms
    };
}