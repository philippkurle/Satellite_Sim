#include "ModeControl.h"
#include "config_modes.h"

void ModeControl::begin() {
    _mode = Mode::NOMINAL;
    _prev_mode = Mode::NOMINAL;
    _mode_changed = false;
    _next_ms = 0;
}

void ModeControl::update(uint32_t now_ms, const SensorData& s) {
    if ((int32_t)(now_ms - _next_ms) < 0) {
        return;
    }
    _next_ms = now_ms + modes::MODECTRL_PERIOD_MS;

    switch (_mode) {
        case Mode::NOMINAL:
            if (s.pv_low_safe == true) {
                setMode(Mode::SAFE);
            } else if (s.pv_low_warn == true) {
                setMode(Mode::WARN);
            }
            break;
        case Mode::WARN:
            if (s.pv_low_safe == true) {
                setMode(Mode::SAFE);
            } else if (s.pv_low_warn == false) {
                setMode(Mode::NOMINAL);
            }
            break;
        case Mode::SAFE:
            if (s.pv_low_safe == false) {
                setMode(Mode::WARN);
            }
            break;
    }

    switch (_mode) {
        case Mode::NOMINAL:
            if (s.water_confirm == true) {
                setMode(Mode::SAFE);
            } else if (s.water_suspect == true) {
                setMode(Mode::WARN);
            }
            break;
        case Mode::WARN:
            if (s.water_confirm == true) {
                setMode(Mode::SAFE);
            }
            break;
        case Mode::SAFE:
            break;
    }
}

void ModeControl::setMode(Mode m) {
    if (m == _mode) {
        return;
    }
    _prev_mode = _mode;
    _mode = m;
    _mode_changed = true;
}

