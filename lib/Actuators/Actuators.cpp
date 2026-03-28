#include "Actuators.h"
#include <Servo.h>

static Servo g_servo;
static bool g_servo_attached = false;

// angle clamping
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
    // reset everything
    _servo_allowed = false;
    _servo_requested = false;
    _servo_active = false;

    _next_step_at = 0;
    _step_index = 0; // reset _step_index

    // against immediate attach
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

    // prevent millis() overflow
    if ((int32_t)(now_ms - _next_step_at) < 0) {
        return;
    }

    _next_step_at = now_ms + SERVO_CONFIG.step_ms;

    const uint8_t total_steps = (uint8_t)(2 * SERVO_CONFIG.repeats + 1); // +1 -> back to center

    if (_step_index >= total_steps) {
        stopServoActive();
        return;
    }

    int target = SERVO_CONFIG.center_deg;

    if (_step_index < (uint8_t)(2*SERVO_CONFIG.repeats + 1)) {
        // switching amplitude +/-
        const bool even = ((_step_index % 2) == 0);
        target = SERVO_CONFIG.center_deg + (even ? SERVO_CONFIG.amplitude_deg : - (int)SERVO_CONFIG.amplitude_deg);
    } else {
        target = SERVO_CONFIG.center_deg; // back to starting position
    }

    const uint8_t angle = clampServoAngle(target);

    if (g_servo_attached == true) {
        g_servo.write(angle);
        /*
        Serial.print(F("servoWrite="));
        Serial.print(angle);
        Serial.println(F(" degrees"));
        */
    }

    _step_index++;

    // stop if last step
    if (_step_index >= total_steps) {
        stopServoActive();
    }
}

void Actuators::setServoAllowed(bool allowed) {
    _servo_allowed = allowed;

    // stops if clearance revoked
    if (_servo_allowed == false) {
        _servo_requested = false;
        if (_servo_active == true) {
            stopServoActive();
        }
        // detach
        if (g_servo_attached == true) {
            g_servo.detach();
            g_servo_attached = false;
        }
    }
}

// trigger function
void Actuators::requestServoActive() {
    _servo_requested = true;
}

void Actuators::startServoActive(uint32_t now_ms) {
    _servo_requested = false;
    // initialize movement and reset state
    _servo_active = true;
    _step_index = 0;

    if (g_servo_attached == false) {
        g_servo.attach(pins::PIN_SERVO);
        
        Serial.print(F("servoAttached="));
        Serial.println(g_servo_attached);
        
        g_servo_attached = true;
    }
    g_servo.write(clampServoAngle(SERVO_CONFIG.center_deg)); // starting position center

    _next_step_at = now_ms + SERVO_CONFIG.step_ms;
}

void Actuators::stopServoActive() {
    _servo_active = false;

    if (g_servo_attached == true) {
        g_servo.write(clampServoAngle(SERVO_CONFIG.center_deg)); // back to starting position
    }
    
    if (g_servo_attached == true) {
        g_servo.detach();
        g_servo_attached = false;
    }
}


