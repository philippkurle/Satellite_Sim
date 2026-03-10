#include "ModeLight.h"
#include "config_pins.h"

void ModeLight::begin() {
    pinMode(pins::PIN_LED_R, OUTPUT);
    pinMode(pins::PIN_LED_Y, OUTPUT);
    pinMode(pins::PIN_LED_G, OUTPUT);
    write(false, false, false);
}

void ModeLight::setMode(Mode m) {
    switch (m) {
        case Mode::NOMINAL:
            write(false, false, true);
            break;
        case Mode::WARN:
            write(false, true, false);
            break;
        case Mode::SAFE:
            write(true, false, false);
            break;
        default:
            write(false, false, false);
            break;
    }
}

void ModeLight::write(bool r, bool y, bool g) {
    digitalWrite(pins::PIN_LED_R, r ? HIGH : LOW); // Bedingung ? Wert_wenn_true : Wert_wenn_false
    digitalWrite(pins::PIN_LED_Y, y ? HIGH : LOW);
    digitalWrite(pins::PIN_LED_G, g ? HIGH : LOW);
}