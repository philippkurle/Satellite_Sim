#pragma once 

#include <stdint.h>

namespace ui { // namespaces group related code (functions, classes, variables), better organisation, avoid naming conflicts

    constexpr uint8_t LCD_ADDR = 0x27; // lcd address
    constexpr uint16_t LCD_PAGE_ROTATE_MS = 4000;

    namespace buzzer {

        constexpr uint16_t ON1_MS = 120; // constexpr: value/result computed during compilation, not runtime
        constexpr uint16_t OFF1_MS = 80; // performance critical constants, configuration
        constexpr uint16_t ON2_MS = 120;
        constexpr uint16_t OFF2_MS = 1680;
    }

    // Mega: Vref typically ~5000mV
    constexpr uint16_t ADC_VREF_MV = 5000;

    constexpr uint8_t PV_DIVIDER_FACTOR_NUM = 2; // multiply
    constexpr uint8_t PV_DIVIDER_FACTOR_DEN = 1; // divide
}

