#pragma once

#include <stdint.h>
#include "config_security.h"

enum class SecurityState : uint8_t {
    WAIT_RFID,
    WAIT_CODE,
    LOCKOUT,
    AUTH_OK
};

class Security {
    public:

        explicit Security(const cfg::SecurityConfig& cfg = cfg::SECURITY_CONFIG);

        void begin();

        void update(uint32_t now_ms);

        // Inputs
        void onRfidUid(const uint8_t* uid, uint8_t len, uint32_t now_ms);
        void onKey(char key, uint32_t now_ms);

        // Event
        bool consumeAuthSuccessEvent();

        // Status für UI
        SecurityState state() const {
            return _state;
        }

        uint8_t attempsUsed() const {
            return _attempts_used;
        }

        uint8_t enteredLen() const {
            return _entered_len;
        }

        void reset(uint32_t now_ms);

        const char* enteredText() const {
            return _entered;
        }

        void copyEntered(char* out, uint8_t out_sz) const;

    private:

        const cfg::SecurityConfig& _cfg;

        SecurityState _state = SecurityState::WAIT_RFID;

        bool _auth_success_event = false;

        // Timing
        uint32_t _code_deadline_ms = 0;
        uint32_t _interkey_deadline_ms = 0;
        uint32_t _lockout_until_ms = 0;

        static constexpr uint8_t kBufMax = 16;
        char _entered[kBufMax] {}; // Array
        uint8_t _entered_len = 0;

        uint8_t _attempts_used = 0;

        static bool timeReached(uint32_t now, uint32_t t) {
            return (int32_t)(now - t) >= 0;
        }
        
        void toWaitRfid(uint32_t now_ms);

        void toWaitCode(uint32_t now_ms);
        
        void toLockout(uint32_t now_ms);

        void clearEntered();

        bool codeMatches() const;

        bool isRfidAllowed(const uint8_t* uid, uint8_t len) const;
};