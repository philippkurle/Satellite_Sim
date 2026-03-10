#include <Arduino.h>

#include "types.h"

#include "config_modes.h"
#include "config_ui.h"

#include "utils_adc.h"

#include "Sensors.h"
#include "ModeControl.h"
#include "Security.h"
#include "RfidRc522Reader.h"
#include "KeypadMatrix.h"
#include "Actuators.h"
#include "UiFacade.h"

#ifndef SATSIM_TELEMTRY
#define SATSIM_TELEMETRY 1
#endif

static inline bool timeReached(uint32_t now, uint32_t t) {
  return (int32_t)(now - t) >= 0;
}

static inline uint16_t displayPeriodFor(Mode m) {
  switch (m) {
    case Mode::NOMINAL:
      return modes::DISPLAY_PERIOD_NOMINAL_MS;
    case Mode::WARN:
      return modes::DISPLAY_PERIOD_WARN_MS;
    case Mode::SAFE:
      return modes::DISPLAY_PERIOD_SAFE_MS;
    default:
      return modes::DISPLAY_PERIOD_NOMINAL_MS;
  }
}

static inline uint16_t telemetryPeriodFor(Mode m) {
  switch (m) {
    case Mode::NOMINAL:
      return modes::TELEMETRY_PERIOD_NOMINAL_MS;
    case Mode::WARN:
      return modes::TELEMETRY_PERIOD_WARN_MS;
    case Mode::SAFE:
      return modes::TELEMETRY_PERIOD_SAFE_MS;
    default:
      return modes::TELEMETRY_PERIOD_NOMINAL_MS;
  }
}

static inline const __FlashStringHelper* modeName(Mode m) {
  switch (m) {
    case Mode::NOMINAL:
      return F("NOMINAL");
    case Mode::WARN:
      return F("WARN");
    case Mode::SAFE:
      return F("SAFE");
    default:
      return F("?");
  }
}

static inline SecurityUiState mapSecurityUiState(SecurityState s) {
  switch (s) {
    case SecurityState::WAIT_RFID:
      return SecurityUiState::WAIT_RFID;
    case SecurityState::WAIT_CODE:
      return SecurityUiState::WAIT_CODE;
    case SecurityState::LOCKOUT:
     return SecurityUiState::LOCKOUT;
    case SecurityState::AUTH_OK:
      return SecurityUiState::AUTH_OK;
    default:
      return SecurityUiState::WAIT_RFID;
  }
}

static Sensors g_sensors;
static ModeControl g_modectrl;
static RfidRc522Reader g_rfid;
static KeypadMatrix g_keypad;
static Security g_security;
static Actuators g_actuators;
static UiFacade g_ui;

static Mode g_mode = Mode::NOMINAL;

// ModeCtrl gate
static uint32_t g_next_modectrl_ms = 0;

// UI content gate (cache model wird nur periodisch aktualisiert)
static uint32_t g_next_ui_content_ms = 0;

static UiModel g_ui_model_cache {
  .mode = Mode::NOMINAL,
  .centi_v = 0,
  .water_suspect = false,
  .water_confirm = false,
  .sec = SecurityUiState::WAIT_RFID,
  .pin_len = 0
};

// Telemetry Gate
static uint32_t g_next_telemetry_ms = 0;

// Leak throttling latch + gating
struct LeakLatched {
  bool water_suspect = false;
  bool water_confirm = false;
};

static LeakLatched g_leak_latched;

// WARN duty cycle gate
static bool g_warn_leak_on = true;
static uint32_t g_warn_leak_next_toggle_ms = 0;

// SAFE sparse sampling gate
static uint32_t g_safe_leak_next_sample_ms = 0;

static void resetModeDependentSchedulers(uint32_t now_ms, Mode newMode) {
  // UI + telemetry should react quickly to mode changes
  g_next_ui_content_ms = now_ms;
  g_next_telemetry_ms = now_ms;

  // Leak gating reset on mode changes
  if (newMode == Mode::WARN) {
    g_warn_leak_on = true;
    g_warn_leak_next_toggle_ms = now_ms + modes::LEAK_DUTY_WARN_ON_MS;
  } else if (newMode == Mode::SAFE) {
    g_safe_leak_next_sample_ms = now_ms; // allow immediate sample
  }
}

static bool leakSamplingAllowed(uint32_t now_ms, Mode m) {
  if (m == Mode::NOMINAL) {
    return true;
  }
  if (m == Mode::WARN) {
    // Duty cycle: ON 400ms / OFF 1600ms
    if (timeReached(now_ms, g_warn_leak_next_toggle_ms) == true) {
      if (g_warn_leak_on == true) {
        g_warn_leak_on = false;
        g_warn_leak_next_toggle_ms = now_ms + modes::LEAK_DUTY_WARN_OFF_MS;
      } else {
        g_warn_leak_on = false;
        g_warn_leak_next_toggle_ms = now_ms + modes::LEAK_DUTY_WARN_ON_MS;
      }
    }
    return g_warn_leak_on;
  }
  if (m == Mode::SAFE) {
    // in SAFE nur alle 5000ms
    if (timeReached(now_ms, g_safe_leak_next_sample_ms) == true) {
      g_safe_leak_next_sample_ms = now_ms + modes::LEAK_PERIOD_SAFE_MS;
      return true;
    }
    return false;
  }
  return true;
}

static void updateLeakLatched(uint32_t now_ms, const SensorData& s, Mode m) {
  if (leakSamplingAllowed(now_ms, m) == false) {
    return;
  }
  // update nur wenn sampling erlaubt
  g_leak_latched.water_suspect = s.water_suspect;
  g_leak_latched.water_confirm = s.water_confirm;
}

static void updateUiModelCache(uint32_t now_ms, const SensorData& s) {
  g_ui_model_cache.mode = g_mode;

  g_ui_model_cache.centi_v = adcToPvCentiV(s.solar_data_raw);

  g_ui_model_cache.water_suspect = g_leak_latched.water_suspect;
  g_ui_model_cache.water_confirm = g_leak_latched.water_confirm;

  g_ui_model_cache.sec = mapSecurityUiState(g_security.state());
  g_ui_model_cache.pin_len = g_security.enteredLen();
  g_security.copyEntered(g_ui_model_cache.pin_text, sizeof(g_ui_model_cache.pin_text));

  g_ui_model_cache.sec_raw = static_cast<uint8_t>(g_security.state());

  (void)now_ms;
}

static void emitTelemetry(uint32_t now_ms, const SensorData& s) {
  #if SATSIM_TELEMETRY

    Serial.print(F("["));
    Serial.print(now_ms);
    Serial.print(F("] mode="));
    Serial.print(modeName(g_mode));

    Serial.print(F(" pv_cV="));
    Serial.print(adcToPvCentiV(s.solar_data_raw));

    Serial.print(F(" water="));
    Serial.print(g_leak_latched.water_confirm ? "C" : (g_leak_latched.water_suspect ? "S" : "-"));

    Serial.print(F(" sec="));
    switch (g_security.state()) {
      case SecurityState::WAIT_RFID:
        Serial.print(F("RFID"));
        break;
      case SecurityState::WAIT_CODE:
        Serial.print(F("CODE"));
        break;
      case SecurityState::LOCKOUT:
        Serial.print(F("LOCK"));
        break;
      case SecurityState::AUTH_OK:
        Serial.print(F("AUTH_OK"));
        break;
    }
    Serial.print(F(" servoActive="));
    Serial.print(g_actuators.isServoActive() ? 1 : 0);

    Serial.println();

  #else
    (void)now_ms;
    (void) s;
  #endif    
}

void setup() {
  #if SATSIM_TELEMETRY
    Serial.begin(115200);
  #endif

   g_sensors.begin();
   g_modectrl.begin();

   g_rfid.begin();
   g_keypad.begin();
   g_security.begin();

   g_actuators.begin();
   g_ui.begin();

   const uint32_t now_ms = millis();

   // Initialize mode & policies
   g_mode = g_modectrl.getMode();
   g_ui_model_cache.mode = g_mode;

   g_actuators.setServoAllowed(g_mode == Mode::NOMINAL);

   resetModeDependentSchedulers(now_ms, g_mode);

   // Start gates
   g_next_modectrl_ms = now_ms;
   g_next_ui_content_ms = now_ms;
   g_next_telemetry_ms = now_ms;
}

void loop() {
  const uint32_t now_ms = millis();

  // Update always-on subsystems
  g_sensors.update(now_ms);

  g_rfid.update(now_ms);

  g_keypad.update(now_ms);

  // Route input events into Security (event-driven inputs)
  uint8_t uid[10];
  uint8_t uid_len = 0;

  if (g_rfid.consumeUid(uid, uid_len) == true) {
    SecurityUiState before = mapSecurityUiState(g_security.state());
    g_security.onRfidUid(uid, uid_len, now_ms);
    SecurityUiState after = mapSecurityUiState(g_security.state());

    if (before == SecurityUiState::WAIT_RFID && after == SecurityUiState::WAIT_CODE) {
      g_ui.notifyRfidAccepted(now_ms);
    }
  }
  
  char key;
  while (g_keypad.consumeKey(key) == true) {
    g_security.onKey(key, now_ms);
    g_ui.notifyKeyActivity(now_ms);
  }

  g_security.update(now_ms);

  // Security event routing → Servo trigger
  if (g_security.consumeAuthSuccessEvent() == true) {
    //Serial.println(F("[SEC] AUTH SUCCESS event consumed"));
    g_ui.notifyAuthSuccess(now_ms);

    if (g_mode == Mode::NOMINAL) {
      g_actuators.requestServoActive();
    }
  }
  // Actuators update always (non-blocking FSM)
  g_actuators.update(now_ms);

  // Supervisor: ModeControl ticking
  const SensorData& s = g_sensors.data();

  if (timeReached(now_ms, g_next_modectrl_ms) == true) {
    g_next_modectrl_ms = now_ms + modes::MODECTRL_PERIOD_MS;
    g_modectrl.update(now_ms, s);
  }
  // One-shot mode changed event
  Mode from, to;
  if (g_modectrl.consumeModeChanged(from, to) == true) {
    g_mode = to;

    // Policy reactions
    g_actuators.setServoAllowed(g_mode == Mode::NOMINAL);

    g_ui_model_cache.mode = g_mode; // for ModeLight/Buzzer reaction

    resetModeDependentSchedulers(now_ms, g_mode);
  } else {
    g_mode = g_modectrl.getMode();
    g_ui_model_cache.mode = g_mode;
  }
  updateLeakLatched(now_ms, s, g_mode); // Leak throttling
  
  // UI content update policy (cached model)
  const uint16_t displayPeriod = displayPeriodFor(g_mode);
  if (timeReached(now_ms, g_next_ui_content_ms) == true) {
    g_next_ui_content_ms = now_ms + displayPeriod;
    updateUiModelCache(now_ms, s);
  }

  g_ui.update(now_ms, g_ui_model_cache);

  // Telemetry policy
  const uint16_t telemetryPeriod = telemetryPeriodFor(g_mode);
  if (timeReached(now_ms, g_next_telemetry_ms) == true) {
    g_next_telemetry_ms = now_ms + telemetryPeriod;
    emitTelemetry(now_ms, s);
  }
}
