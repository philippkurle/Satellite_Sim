#include "Security.h"
#include <string.h>
#include <Arduino.h>

Security::Security(const cfg::SecurityConfig& _cfg) : _cfg(_cfg) {}

void Security::begin() {
    reset(0);
}

void Security::update(uint32_t now_ms) {
    if (_state == SecurityState::LOCKOUT) {
        if (timeReached(now_ms, _lockout_until_ms)) {
            toWaitRfid(now_ms);
            return;
        }
    }
    if (_state == SecurityState::WAIT_CODE) {
        if (_code_deadline_ms != 0 && timeReached(now_ms, _code_deadline_ms) == true) {
            toWaitRfid(now_ms);
            return;
        }
        if (_interkey_deadline_ms != 0 && timeReached(now_ms, _interkey_deadline_ms) == true) {
            toWaitRfid(now_ms);
            return;
        }
    }
}

void Security::onRfidUid(const uint8_t* uid, uint8_t len, uint32_t now_ms) {
    if (_state == SecurityState::LOCKOUT) {
        return;
    }
    if (_state != SecurityState::WAIT_RFID) {
        return;
    }
    if (isRfidAllowed(uid, len) == true) {
        toWaitCode(now_ms);
    }
}

void Security::onKey(char key, uint32_t now_ms) {
    if (_state == SecurityState::LOCKOUT) {
        return;
    }
    if (_state != SecurityState::WAIT_CODE) {
        return;
    }
    _interkey_deadline_ms = now_ms + _cfg.interkey_timeout_ms;

    if (key == '*') {
        clearEntered();
        return;
    }
    if (key == '#') {
        if (_entered_len == 0) {
            return;
        }
        if (codeMatches() == true) {
            _auth_success_event = true;
            toWaitRfid(now_ms);
        } else {
            _attempts_used++;
            if (_attempts_used >= _cfg.max_attempts) {
                toLockout(now_ms);
            } else {
                toWaitRfid(now_ms);
            }
        }
        return;
    }
    if (key < '0' || key > '9') {
        return;
    }
    if (_entered_len < _cfg.code_len_max && _entered_len < kBufMax - 1) {
        _entered[_entered_len++] = key;
        _entered[_entered_len] = '\0';
    }
}

bool Security::consumeAuthSuccessEvent() {
    if (_auth_success_event == true) {
        _auth_success_event = false;
        return true;
    } else {
        return false;
    }
}

void Security::reset(uint32_t now_ms) {
    (void)now_ms;
    _state = SecurityState::WAIT_RFID;
    _auth_success_event= false;
    _attempts_used = 0;
    _lockout_until_ms = 0;
    _code_deadline_ms = 0;
    _interkey_deadline_ms = 0;
    clearEntered();
}

void Security::copyEntered(char* out, uint8_t out_sz) const {
  if (out_sz == 0) {
    return;
  }
  uint8_t n = _entered_len;
  if (n > out_sz - 1) {
    n = out_sz - 1;
  }
  memcpy(out, _entered, n);
  out[n] = '\0';
}

// intern
void Security::toWaitRfid(uint32_t now_ms) {
    (void)now_ms;
    _state = SecurityState::WAIT_RFID;
    _code_deadline_ms = 0;
    _interkey_deadline_ms = 0;
    clearEntered();
}

void Security::toWaitCode(uint32_t now_ms) {
    _state = SecurityState::WAIT_CODE;
    clearEntered();
    _code_deadline_ms = now_ms + _cfg.code_timeout_ms;
    _interkey_deadline_ms = now_ms + _cfg.interkey_timeout_ms;
}

void Security::toLockout(uint32_t now_ms) {
    _state = SecurityState::LOCKOUT;
    clearEntered();
    _code_deadline_ms = 0;
    _interkey_deadline_ms = 0;
    _lockout_until_ms = now_ms + _cfg.lockout_ms;
}

void Security::clearEntered() {
    _entered_len = 0;
    _entered[0] = '\0';
}

bool Security::codeMatches() const {
    return strcmp(_entered, _cfg.pin_code) == 0;
}

bool Security::isRfidAllowed(const uint8_t* uid, uint8_t len) const {
    if (cfg::RFID_ALLOW_ANY == true) {
        return true;
    }
}