#pragma once

#include <stdint.h>
#include "config_ui.h"
#include "ModeLight.h"
#include "BuzzerBeacon.h"
#include "LcdUi.h"

class UiFacade {
    public:

        void begin();

        // lcd + buzzer + traffic light
        void update(uint32_t now_ms, const UiModel& m);

        void notifyAuthSuccess(uint32_t now_ms);

        void notifyRfidAccepted(uint32_t now_ms);

        void notifyKeyActivity(uint32_t now_ms);

    private:

        ModeLight _mode_light;
        BuzzerBeacon _buz;
        LcdUi _lcd;
        uint32_t _update = 0;
};
