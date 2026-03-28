#pragma once

#include <stdint.h>
#include "types.h"
#include "config_pins.h"
#include "config_thresholds.h"

class Sensors {
    public: // usable from outside the class eg. from outside Sensors

        // Setup: PinModes etc.
        void begin();
               
        void update(uint32_t now_ms); // non blocking via use of now_ms
        
        // returns read only, referenced struct
        const SensorData& data() const; // & -> referencing the original, not a copy; const -> read only

    private: // only useable within class Sensors

        // saves recent data
        SensorData _data;

        uint32_t _next_sample_ms = 0;
        uint16_t _sample_period_ms = 50; // 50ms -> 20Hz

        // returns an average of the last <samples> measurements from <pin>
        uint16_t readAnalogAveraged(uint8_t pin, uint8_t samples);

        // returns Hysteresis state for high = event measurements
        struct BoolHysteresisHigh {
            uint16_t on;
            uint16_t off;

            bool state = false;

            bool update(uint16_t x) {
                if (state == false) {
                    if (x >= on) {
                        state = true;
                    }
                } else {
                    if (x <= off) {
                        state = false;
                    }
                }
                return state;
            }
        };

        // returns Hysteresis state for low = event measurements
        struct BoolHysteresisLow {
            uint16_t on;
            uint16_t off;

            bool state = false;

            bool update(uint16_t x) {
                if (state == false) {
                    if (x <= on) {
                        state = true;
                    }
                } else {
                    if (x >= off) {
                        state = false;
                    }
                }
                return state;
            }
        };


        BoolHysteresisHigh _water_suspect_hys; // initializes an object of type BoolHysteresisHigh within class Sensors
        BoolHysteresisHigh _water_confirm_hys;

        BoolHysteresisLow _pv_warn_hys;
        BoolHysteresisLow _pv_safe_hys;
};