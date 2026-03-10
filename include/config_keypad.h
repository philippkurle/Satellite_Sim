#pragma once

#include <stdint.h>

namespace cfg {

    constexpr uint8_t KEYPAD_ROWS = 4;
    constexpr uint8_t KEYPAD_COLS = 4;

    // Zuordnung PINs --> Reihen & Spalten
    inline constexpr uint8_t KEYPAD_ROW_PINS[KEYPAD_ROWS] = {29, 28, 27, 26};
    inline constexpr uint8_t KEYPAD_COL_PINS[KEYPAD_COLS] = {25, 24, 23, 22};
    
    // Keymap
    inline constexpr char KEYPAD_MAP[KEYPAD_ROWS][KEYPAD_COLS] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'},
    };

    struct KeypadConfig {
        uint16_t scan_step_ms; // pro Update eine Spalte scannen
        uint16_t debounce_ms; // Entprellzeit
    };

    inline constexpr KeypadConfig KEYPAD_CONFIG {
        .scan_step_ms = 3,
        .debounce_ms = 25,
    };
}