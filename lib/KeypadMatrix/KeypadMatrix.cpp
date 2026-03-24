#include "KeypadMatrix.h"
#include <Arduino.h>

KeypadMatrix::KeypadMatrix(const cfg::KeypadConfig& cfg) : _cfg(cfg) {}

void KeypadMatrix::begin() {
    for (uint8_t r = 0; r < cfg::KEYPAD_ROWS; r++) {
        pinMode(cfg::KEYPAD_ROW_PINS[r], INPUT_PULLUP);
    }

    for (uint8_t c = 0; c < cfg::KEYPAD_COLS; c++) {
        pinMode(cfg::KEYPAD_COL_PINS[c], OUTPUT);
        digitalWrite(cfg::KEYPAD_COL_PINS[c], HIGH);
    }

    _active_col = 0;
    _next_step_ms = 0;
}

void KeypadMatrix::update(uint32_t now_ms) {
    if (isTimeReached(now_ms, _next_step_ms) != true) {
        return;
    }

    _next_step_ms = now_ms + _cfg.scan_step_ms;

    // scan one column per update
    driveColLow(_active_col);

    // read rows
    for (uint8_t r = 0; r < cfg::KEYPAD_ROWS; r++) {
        bool pressed = (digitalRead(cfg::KEYPAD_ROW_PINS[r]) == LOW);
        bool prev_raw = _raw[r][_active_col];

        if (pressed != prev_raw) {
            _raw[r][_active_col] = pressed;
            _changed_at[r][_active_col] = now_ms;
        } else {
            if ((int32_t)(now_ms - _changed_at[r][_active_col]) >= (int32_t)_cfg.debounce_ms) {
                bool prev_stable = _stable[r][_active_col];
                if (prev_stable != pressed) {
                    _stable[r][_active_col] = pressed;

                    if (_key_pending == false && pressed == true) {
                        _key = cfg::KEYPAD_MAP[r][_active_col];
                        _key_pending = true;
                    }
                }
            }
        }
    }
    
    // next column
    _active_col++;

    if (_active_col >= cfg::KEYPAD_COLS) {
        _active_col = 0;
    }
}

bool KeypadMatrix::consumeKey(char& out_key) {
    if (_key_pending == false) {
        return false;
    }
    
    out_key = _key;
    _key_pending = false;
    return true;
}

void KeypadMatrix::driveAllColsHigh() {
    for (uint8_t c = 0; c < cfg::KEYPAD_COLS; c++) {
        digitalWrite(cfg::KEYPAD_COL_PINS[c], HIGH);
    }
}

void KeypadMatrix::driveColLow(uint8_t col) {
    driveAllColsHigh(); // all HIGH first, then one LOW (reduces ghosting)
    digitalWrite(cfg::KEYPAD_COL_PINS[col], LOW);
}