#include "Actuators.h"
#include <Servo.h>

static Servo g_servo;
static bool g_servo_attached = false;

// Winkel im Servo-Bereich clampen
static uint8_t clampServoAngle(int angleDeg) { 
    if (angleDeg < 0) {
        return 0;
    }
    if (angleDeg > 180) {
        return 180;
    }
    return (uint8_t) angleDeg;
}

void Actuators::begin() {
    // Startzustand (setzt alle falgs zurück)
    _servo_allowed = false;
    _servo_requested = false;
    _servo_active = false;

    _next_step_at = 0; // Startwert timing
    _step_index = 0; // _step_index  zurücksetzen

    // gegen sofort attach
    if (g_servo_attached == true) {
        g_servo.detach();
        g_servo_attached = false;
    }
}

void Actuators::update(uint32_t now_ms) {
    if (_servo_requested == true && _servo_allowed == true && _servo_active == false) {
        startServoActive(now_ms);
    }
    if (_servo_active == false) {
        return;
    }
    // gegen millis() overflow
    if ((int32_t)(now_ms - _next_step_at) < 0) {
        return;
    }
    _next_step_at = now_ms + SERVO_CONFIG.step_ms;

    const uint8_t total_steps = (uint8_t)(2 * SERVO_CONFIG.repeats + 1); // +1 für zurück zu center

    if (_step_index >= total_steps) {
        stopServoActive();
        return;
    }
    int target = SERVO_CONFIG.center_deg;

    if (_step_index < (uint8_t)(2*SERVO_CONFIG.repeats + 1)) {
        // abwechselnd Amplitude +/-
        const bool even = ((_step_index % 2) == 0);
        target = SERVO_CONFIG.center_deg + (even ? SERVO_CONFIG.amplitude_deg : - (int)SERVO_CONFIG.amplitude_deg);
    } else {
        target = SERVO_CONFIG.center_deg; // zurück zur Mitte
    }
    const uint8_t angle = clampServoAngle(target);

    // Servo ansteuern (write in Grad)
    if (g_servo_attached == true) {
        g_servo.write(angle);
        /*
        Serial.print(F("servoWrite="));
        Serial.print(angle);
        Serial.println(F(" degrees"));
        */
    }
    _step_index++;

    // beenden falls letzter step
    if (_step_index >= total_steps) {
        stopServoActive();
    }
}

void Actuators::setServoAllowed(bool allowed) {
    _servo_allowed = allowed;

    // stoppt wenn Freigabe entzogen
    if (_servo_allowed == false) {
        _servo_requested = false;
        if (_servo_active == true) {
            stopServoActive();
        }
        // detach, falls Servo noch hängt
        if (g_servo_attached == true) {
            g_servo.detach();
            g_servo_attached = false;
        }
    }
}

// Trigger Funktion
void Actuators::requestServoActive() {
    _servo_requested = true;
}

void Actuators::startServoActive(uint32_t now_ms) {
    _servo_requested = false;
    // Bewegung aktiv- und State zurücksetzen
    _servo_active = true;
    _step_index = 0;

    if (g_servo_attached == false) {
        g_servo.attach(pins::PIN_SERVO);
        
        Serial.print(F("servoAttached="));
        Serial.println(g_servo_attached);
        
        g_servo_attached = true;
    }
    g_servo.write(clampServoAngle(SERVO_CONFIG.center_deg)); // Startposition center

    // ersten step über step_ms takten
    _next_step_at = now_ms + SERVO_CONFIG.step_ms;
}

void Actuators::stopServoActive() {
    _servo_active = false;

    if (g_servo_attached == true) {
        g_servo.write(clampServoAngle(SERVO_CONFIG.center_deg)); // zurück zur Ausgangsposition in der Mitte
    }
    if (g_servo_attached == true) {
        g_servo.detach();
        g_servo_attached = false;
    }
}


