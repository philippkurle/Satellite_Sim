#include "FourDigit7Seg.h"
#include <stdint.h>
#include "config_pins.h"
#include "config_ui.h"
#include "config_display7seg.h"
/*
using namespace seg7;


namespace ui {
  uint32_t g_steps = 0;
  uint32_t g_last_step_us = 0;
  uint32_t g_max_gap_us = 0;
  uint32_t g_report_ms = 0;
  uint32_t g_steps_last_report = 0;
}


constexpr uint8_t DIGIT_PINS[4] = {
    pins::PIN_DIGIT_1,
    pins::PIN_DIGIT_2,
    pins::PIN_DIGIT_3,
    pins::PIN_DIGIT_4
};

constexpr uint8_t SEG_PINS[8] = {
    pins::PIN_SEG_A,
    pins::PIN_SEG_B,
    pins::PIN_SEG_C,
    pins::PIN_SEG_D,
    pins::PIN_SEG_E,
    pins::PIN_SEG_F,
    pins::PIN_SEG_G,
    pins::PIN_SEG_DP
};


constexpr uint8_t glyphForDigit(uint8_t d) {
    switch (d) {
        case 0:
            return GLYPH_0;
        case 1:
            return GLYPH_1;
        case 2:
            return GLYPH_2;
        case 3:
            return GLYPH_3;
        case 4:
            return GLYPH_4;
        case 5:
            return GLYPH_5;
        case 6:
            return GLYPH_6;
        case 7:
            return GLYPH_7;
        case 8:
            return GLYPH_8;
        case 9:
            return GLYPH_9;
        default:
            return GLYPH_BLANK;
    }
}

void FourDigit7Seg::begin() {
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(DIGIT_PINS[i], OUTPUT);
    }

    for (uint8_t i = 0; i < 8; i++) {
        pinMode(SEG_PINS[i], OUTPUT);
    }

    allDigitsOff();
    setSegments(GLYPH_BLANK);

    _prev_digit = 0xFF;
    _last_pattern = 0xFF;
    
    rebuildPatterns();
}

void FourDigit7Seg::update(uint32_t now_ms) {
    (void)now_ms;
    uint32_t now_us = micros();

    constexpr uint32_t REFRESH_US = 200;
    constexpr uint32_t DEAD_US = 5;

    //uint8_t catch_up = 0;

    if (_next_refresh_us == 0) {
        _next_refresh_us = now_us + REFRESH_US;
        _phase = Phase::off_set;
        return;
    }

    uint8_t events = 0;
    constexpr uint8_t MAX_EVENTS_PER_CALL = 8;

    //while ((int32_t)(now_us - _next_refresh_us) >= 0 && catch_up < 4) {

    while (events < MAX_EVENTS_PER_CALL) {

        if (_phase == Phase::off_set) {
            if ((int32_t)(now_us - _next_refresh_us) < 0) {
                break;
            }

            uint8_t p = _pattern[_digit];

            _pending_digit = _digit;
            _pending_pattern = p;

            if (_prev_digit != 0xFF) {
                disableDigit(_prev_digit);
            }

            setSegments(0);
            _last_pattern = 0xFF;

            _dead_until_us = _next_refresh_us + DEAD_US;
            _phase = Phase::wait_on;

        } else {
            if ((int32_t)(now_us - _dead_until_us) < 0) {
                break;
            }

            if (_pending_pattern != _last_pattern) {
                setSegments(_pending_pattern);
                _last_pattern = _pending_pattern;
            }
            // Digit einschalten
            enableDigit(_pending_digit);

            // Digit merken
            _prev_digit = _pending_digit;

            // next Digit
            _digit = (_digit + 1) & 0x03;

            _next_refresh_us += REFRESH_US;
            _phase = Phase::off_set;
            
            //catch_up++;

            ui::g_steps++;

            if (ui::g_last_step_us != 0) {
                uint32_t gap = now_us - ui::g_last_step_us;

                if (gap > ui::g_max_gap_us) {
                    ui::g_max_gap_us = gap;
                }
            } 

            ui::g_last_step_us = now_us;
        }

        events++;
    }
}

void FourDigit7Seg::setVoltage_cV(int16_t centi_v) {
    uint16_t v = (uint16_t) centi_v;

    if (v < 0) {
        _glyph[0] = GLYPH_DASH;
        _glyph[1] = GLYPH_DASH;
        _glyph[2] = GLYPH_DASH;
        _glyph[3] = GLYPH_DASH;
        _dp_mask = 0;
        return;
    }

    if (v > 9999) {
        _glyph[0] = GLYPH_DASH;
        _glyph[1] = GLYPH_DASH;
        _glyph[2] = GLYPH_DASH;
        _glyph[3] = GLYPH_DASH;
        _dp_mask = 0;
        return;
    }

    if (v < 1000) {
        // x.xx => [x].[x][x][blank]
        uint8_t ones = (v / 100) % 10;
        uint8_t tenths = (v / 10) % 10;
        uint8_t hundreds = (v / 1) % 10;

        _glyph[0] = glyphForDigit(ones);
        _glyph[1] = glyphForDigit(tenths);
        _glyph[2] = glyphForDigit(hundreds);
        _glyph[3] = GLYPH_BLANK;

        _dp_mask = (1 << 0); // dp nach Digit0

    } else {
        // xx.xx => [x][x].[x][x]
        uint8_t tens = (v / 1000) % 10;
        uint8_t ones = (v / 100) % 10;
        uint8_t tenths = (v / 10) % 10;
        uint8_t hundreds = (v / 1) % 10;

        _glyph[0] = glyphForDigit(tens);
        _glyph[1] = glyphForDigit(ones);
        _glyph[2] = glyphForDigit(tenths);
        _glyph[3] = glyphForDigit(hundreds);

        _dp_mask = (1 << 1); // dp nach Digit1
    }
    rebuildPatterns();
}

void FourDigit7Seg::setRawGlyph(uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3, uint8_t dp_mask) {
    _glyph[0] = g0;
    _glyph[1] = g1;
    _glyph[2] = g2;
    _glyph[3] = g3;
    _dp_mask = dp_mask;

    rebuildPatterns();
}

void FourDigit7Seg::allDigitsOff() {
    for (uint8_t i = 0; i < 4; i++) {
        digitalWrite(DIGIT_PINS[i], DIGITS_ACTIVE_LOW ? HIGH : LOW);
    }
}

void FourDigit7Seg::setSegments(uint8_t glyph_with_dp) {
    for (uint8_t i = 0; i < 8; i++) {

        bool on = (glyph_with_dp & (1 << i)) != 0;

        digitalWrite(SEG_PINS[i], (SEGMENT_ACTIVE_LOW ? !on : on) ? HIGH : LOW);
    }
}

void FourDigit7Seg::enableDigit(uint8_t digit_index) {
    digitalWrite(DIGIT_PINS[digit_index], DIGITS_ACTIVE_LOW ? LOW : HIGH);
}

void FourDigit7Seg::disableDigit(uint8_t digit_index) {
    if (digit_index > 3) { 
        return; // 0xFF etc. ignorieren
    }

    digitalWrite(DIGIT_PINS[digit_index], DIGITS_ACTIVE_LOW ? HIGH :LOW);
}

void FourDigit7Seg::rebuildPatterns() {
    for (uint8_t i = 0; i < 4; i++) {

        uint8_t p = _glyph[i];

        if (_dp_mask & (1 << i)) {
            p |= SEG_DP;
        }
        _pattern[i] = p;
    }
}
*/