#pragma once

#include <stdint.h>

class FourDigit7Seg {
    public:

        void begin();

        void update(uint32_t now_ms);

        void setVoltage_cV(int16_t centi_v);

        // Für Debug: z.B. "8888" mit Punkten
        void setRawGlyph(uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3, uint8_t dp_mask);

    private:

        uint32_t _next_refresh_us = 0;
        uint8_t _digit = 0;
        uint8_t _glyph[4] = {0,0,0,0};
        uint8_t _dp_mask = 0;

        uint8_t _prev_digit = 0xFF;      // merkt sich, welcher Digit zuletzt an war
        uint8_t _pattern[4] = {0,0,0,0}; // fertiges Segmentpattern pro Digit (Glyph + DP)
        uint8_t _last_pattern = 0xFF;    // zuletzt ausgegebenes Segmentpattern

        enum class Phase : uint8_t {
            off_set,
            wait_on
        };

        Phase _phase = Phase::off_set;

        uint32_t _dead_until_us = 0;
        uint8_t _pending_digit = 0;
        uint8_t _pending_pattern = 0;

        void allDigitsOff();

        void setSegments(uint8_t glyph_with_dp);
        
        void enableDigit(uint8_t digit_index);

        void disableDigit(uint8_t digit_index);

        void rebuildPatterns();
};