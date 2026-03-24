#pragma once

#include <stdint.h>
#include "config_keypad.h"

class KeypadMatrix {
    public:

        explicit KeypadMatrix(const cfg::KeypadConfig& cfg = cfg::KEYPAD_CONFIG); // cdf::KEYPAD_CONFIG -> default value

        void begin();

        void update(uint32_t now_ms);

        bool consumeKey(char& out_key);

    private:

        const cfg::KeypadConfig& _cfg;
        uint32_t _next_step_ms = 0;
        uint8_t _active_col = 0;

        // debounce Arrays (ignore false presses caused by electrical noise)
        bool _raw[cfg::KEYPAD_ROWS][cfg::KEYPAD_COLS] {}; // {} initialisiert mit: bool -> false und uint32_t -> 0 
        bool _stable[cfg::KEYPAD_ROWS][cfg::KEYPAD_COLS] {};
        uint32_t _changed_at[cfg::KEYPAD_ROWS][cfg::KEYPAD_COLS]{};

        bool _key_pending = false;
        char _key = 0;

        bool isTimeReached(uint32_t now, uint32_t t) const {
            return (int32_t)(now - t) >= 0;
        }

        void driveAllColsHigh();
        
        void driveColLow(uint8_t col);
};