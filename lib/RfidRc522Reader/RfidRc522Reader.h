#pragma once

#include <stdint.h>
#include <SPI.h>
#include <MFRC522.h>
#include "config_rfid.h"

class RfidRc522Reader {
    public:

        explicit RfidRc522Reader(const cfg::RfidConfig& cfg = cfg::RFID_CONFIG);

        void begin();
        
        void update(uint32_t now_ms);

        bool consumeUid(uint8_t* out_uid, uint8_t& out_len);

    private:

        const cfg::RfidConfig& _cfg;
        MFRC522 _mfrc;

        uint32_t _next_poll_ms = 0;
        uint8_t _uid[10] {};
        uint8_t _uid_len = 0;

        bool _uid_pending = false;

        bool isTimeReached(uint32_t now, uint32_t t) const {
            return (int32_t)(now - t) >= 0;
        }
};