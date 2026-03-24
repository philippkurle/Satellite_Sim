#include "BuzzerBeacon.h"
#include "config_pins.h"
#include "config_ui.h"

void BuzzerBeacon::begin() {
    pinMode(pins::PIN_BUZZER, OUTPUT);
    buz(false);
    _state = State::OFF;
    _enabled = false;
    _next_ms = 0;
}

void BuzzerBeacon::update(uint32_t now_ms) {
    if (_enabled == false) {
        return;
    }

    if ((int32_t)(now_ms - _next_ms) < 0) {
        return;
    }

    switch (_state) {
        case State::ON1:
            setState(State::OFF1, now_ms, ui::buzzer::OFF1_MS);
            buz(false);
            break;
        case State::OFF1:
            setState(State::ON2, now_ms, ui::buzzer::ON2_MS);
            buz(true);
            break;
        case State::ON2:
            setState(State::OFF2, now_ms, ui::buzzer::OFF2_MS);
            buz(false);
            break;
        case State::OFF2:
            setState(State::ON1, now_ms, ui::buzzer::ON1_MS);
            buz(true);
            break;
        default:
            break;
    }
}

void BuzzerBeacon::setEnabled(bool enabled) {
    if (enabled == _enabled) {
        return;
    }
    
    _enabled = enabled;

    if (_enabled == false) {
        _state = State::OFF;
        buz(false);
    } else {
        _state = State::ON1;
        buz(true);
        _next_ms = millis() + ui::buzzer::ON1_MS;
    }
}

void BuzzerBeacon::buz(bool on) {
    digitalWrite(pins::PIN_BUZZER, on ? HIGH : LOW);
}

void BuzzerBeacon::setState(State s, uint32_t now_ms, uint16_t duration_ms) {
    _state = s;
    _next_ms = now_ms + duration_ms;
}