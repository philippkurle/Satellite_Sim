#pragma once

#include <stdint.h>

namespace seg7 {
    
    constexpr uint8_t SEG_A = 1 << 0;
    constexpr uint8_t SEG_B = 1 << 1;
    constexpr uint8_t SEG_C = 1 << 2;
    constexpr uint8_t SEG_D = 1 << 3;
    constexpr uint8_t SEG_E = 1 << 4;
    constexpr uint8_t SEG_F = 1 << 5;
    constexpr uint8_t SEG_G = 1 << 6;
    constexpr uint8_t SEG_DP = 1 << 7;
    
    constexpr bool SEGMENT_ACTIVE_LOW = false;
    constexpr bool DIGITS_ACTIVE_LOW = false;
    
    constexpr uint8_t GLYPH_0 = SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F;
    constexpr uint8_t GLYPH_1 = SEG_B|SEG_C;
    constexpr uint8_t GLYPH_2 = SEG_A|SEG_B|SEG_D|SEG_E|SEG_G;
    constexpr uint8_t GLYPH_3 = SEG_A|SEG_B|SEG_C|SEG_D|SEG_G;
    constexpr uint8_t GLYPH_4 = SEG_B|SEG_C|SEG_F|SEG_G;
    constexpr uint8_t GLYPH_5 = SEG_A|SEG_C|SEG_D|SEG_F|SEG_G;
    constexpr uint8_t GLYPH_6 = SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G;
    constexpr uint8_t GLYPH_7 = SEG_A|SEG_B|SEG_C;
    constexpr uint8_t GLYPH_8 = SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G;
    constexpr uint8_t GLYPH_9 = SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G;
    constexpr uint8_t GLYPH_BLANK = 0;
    constexpr uint8_t GLYPH_DASH = SEG_G;
}