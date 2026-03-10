#pragma once

#include <stdint.h>
#include "config_ui.h"

static inline  int16_t adcToPvCentiV(uint16_t adc) {  
    // PV_mV = adc * Vref_mV * divider_factor / 1023
    // mit divider_factor = 2
    uint32_t mv = (uint32_t)adc * ui::ADC_VREF_MV;
    mv = mv * ui::PV_DIVIDER_FACTOR_NUM / ui::PV_DIVIDER_FACTOR_DEN;

    // Rundung: +511 vor /1023
    mv = (mv + 511) / 1023; // 511 für kaufmännische Rundung statt abrunden

    // centiV = mV/10 (gerundet)
    uint16_t cv = (mv + 5) / 10;
    return (int16_t)cv;
}