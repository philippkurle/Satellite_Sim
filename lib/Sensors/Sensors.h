#pragma once

#include <stdint.h>
#include "types.h"
#include "config_pins.h"
#include "config_thresholds.h"

class Sensors {
    public: // extern nutzbar

        // Setup: PinModes etc.
        void begin();
               
        void update(uint32_t now_ms); // blockiert nicht
        
        // Zugriff auf aktuelle Messdaten
        const SensorData& data() const; // & --> Referenz auf Original (keine Kopie!); const --> nicht änderbar (nur lesen)

    private: // nur sichtbar für Sensors (nur intern nutzbar)

        // speichert letzte Messdaten
        SensorData _data;

        uint32_t _next_sample_ms = 0;
        uint16_t _sample_period_ms = 50; // 20Hz

        // Hilfsfunktion
        uint16_t readAnalogAveraged(uint8_t pin, uint8_t samples);

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


        BoolHysteresisHigh _water_suspect_hys; // legt Objekt des Typs BoolHysteresis in der class Sensors an
        BoolHysteresisHigh _water_confirm_hys;

        BoolHysteresisLow _pv_warn_hys;
        BoolHysteresisLow _pv_safe_hys;
};