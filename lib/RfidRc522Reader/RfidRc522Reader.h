#pragma once

#include <stdint.h>
#include <SPI.h>
#include <MFRC522.h>
#include "config_rfid.h"

class RfidRc522Reader {
    public:

        // takes constant reference to RfidConfig object, with default initializer
        explicit RfidRc522Reader(const cfg::RfidConfig& cfg = cfg::RFID_CONFIG); // explicit prevents implicit conversion from constructor parameter type to class type

        void begin();
        
        void update(uint32_t now_ms);

        bool consumeUid(uint8_t* out_uid, uint8_t& out_len); // & -> reference; * -> pointer: stores memory address of another variable (access data)

    private:

        const cfg::RfidConfig& _cfg; // _cfg refers to existing RfidConfig object, cannot modify it
        MFRC522 _mfrc; // declares instance of MFRC522 class -> interact with hardware through this object

        uint32_t _next_poll_ms = 0;
        uint8_t _uid[10] {}; // initialized with 10x 0
        uint8_t _uid_len = 0;

        bool _uid_pending = false;

        bool isTimeReached(uint32_t now, uint32_t t) const { // is time t reached? -> returns true
            return (int32_t)(now - t) >= 0;
        }
};