#pragma once

#include <stdint.h>
#include "config_ui.h"

static inline  int16_t adcToPvCentiV(uint16_t adc) {  
    // PV_mV = adc * Vref_mV * divider_factor / 1023
    // with divider_factor = 2
    uint32_t mv = (uint32_t)adc * ui::ADC_VREF_MV; // scales raw adc value to reference voltage
    mv = mv * ui::PV_DIVIDER_FACTOR_NUM / ui::PV_DIVIDER_FACTOR_DEN; // voltage divider correction

    // rounding: +511 before /1023
    mv = (mv + 511) / 1023; // banker's rounding

    // centiV = mV/10 (banker's rounding: +5 before dividing)
    uint16_t cv = (mv + 5) / 10;
    return (int16_t)cv;
}