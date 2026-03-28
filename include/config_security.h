#pragma once

#include <stdint.h>

namespace cfg {

    struct SecurityConfig {
        uint32_t code_timeout_ms;
        uint32_t interkey_timeout_ms;
        uint32_t lockout_ms;
        uint8_t max_attempts;
        uint8_t code_len_max;
        const char* pin_code; // pointer to a character, can change pointer but not data pointed to
    };

    // rfid policy
    inline constexpr bool RFID_ALLOW_ANY = true; // inline: define in header file in comliance with One Definition Rule

    struct UidEntry {
        uint8_t len;
        uint8_t bytes[10];
    };

    inline constexpr SecurityConfig SECURITY_CONFIG {
        .code_timeout_ms = 15000,
        .interkey_timeout_ms = 8000,
        .lockout_ms = 30000,
        .max_attempts = 3,
        .code_len_max = 8,
        .pin_code = "1234",
    };
}