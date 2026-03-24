#pragma once

#include <stdint.h>
#include "types.h"

class ModeControl {
    public:

        void begin();

        void update(uint32_t now_ms, const SensorData& s); // passes on SensorData struct content for decision making; reference instead of copy for efficiency

        Mode getMode() const {
            return _mode;
        }

        bool consumeModeChanged(Mode& from, Mode& to) {
            if (_mode_changed == false) {
                return false;
            }
            _mode_changed = false;
            from = _prev_mode;
            to = _mode;
            return true;
        }

    private:

        Mode _mode = Mode::NOMINAL;
        Mode _prev_mode = Mode::NOMINAL;
        
        bool _mode_changed = false;

        uint32_t _next_ms = 0;

        void setMode(Mode m);
};