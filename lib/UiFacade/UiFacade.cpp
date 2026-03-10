#include "UiFacade.h"
#include <Arduino.h>

void UiFacade::begin() {
    _mode_light.begin();
    _buz.begin();
    _lcd.begin();
}

void UiFacade::update(uint32_t now_ms, const UiModel& m) {
    _mode_light.setMode(m.mode);
    _buz.setEnabled(m.mode == Mode::SAFE);
    _buz.update(now_ms);
    _lcd.update(now_ms, m);
}

void UiFacade::notifyAuthSuccess(uint32_t now_ms) {
    _lcd.notifyAuthSuccess(now_ms);
}

void UiFacade::notifyRfidAccepted(uint32_t now_ms) {
    _lcd.notifyRfidAccepted(now_ms);
}

void UiFacade::notifyKeyActivity(uint32_t now_ms) {
    _lcd.notifyKeyActivity(now_ms);
}