#pragma once

#include <stdint.h>
#include "config_actuators.h"
#include "config_pins.h"

class Actuators {
    public:
    
        void begin();

        void update(uint32_t now_ms);

        void setServoAllowed(bool allowed); // from  ModeControl

        void requestServoActive(); // trigger

        bool isServoAllowed() const {
            return _servo_allowed;
        }

        bool isServoActive() const {
            return _servo_active;
        }

    private:

        void startServoActive(uint32_t now_ms);

        void stopServoActive();

        bool _servo_allowed = false;
        bool _servo_requested = false;
        bool _servo_active = false;

        uint32_t _next_step_at = 0;
        uint8_t _step_index = 0;
};