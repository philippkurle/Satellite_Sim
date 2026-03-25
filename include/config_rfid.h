#pragma once

#include <stdint.h>

namespace cfg {

    // define type RfidConfig, reusable configuration blueprint
    struct RfidConfig {
        uint8_t ss_pin; // RC522 SDA/SS -> same function
        uint8_t rst_pin; // RC522 RST
        uint16_t poll_interval_ms; // how often rfid checks for card
    };

    // create constant object of type RfidConfig with assigned values
    inline constexpr RfidConfig RFID_CONFIG {
        .ss_pin = 53,
        .rst_pin = 49,
        .poll_interval_ms = 80,
    };
}