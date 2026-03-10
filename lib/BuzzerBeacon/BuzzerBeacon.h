#pragma once

#include <stdint.h>

class BuzzerBeacon {
    public:

        void begin();

        void update(uint32_t now_ms);

        void setEnabled(bool enabled);

    private:

        enum class State : uint8_t {
            OFF,
            ON1,
            OFF1,
            ON2,
            OFF2
        };

        State _state = State::OFF;

        bool _enabled = false;

        uint32_t _next_ms = 0;

        void buz(bool on);

        void setState(State s, uint32_t now_ms, uint16_t duration_ms);
};