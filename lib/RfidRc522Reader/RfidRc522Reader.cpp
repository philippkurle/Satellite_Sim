#include "RfidRc522Reader.h"

RfidRc522Reader::RfidRc522Reader(const cfg::RfidConfig& cfg) : _cfg(cfg), _mfrc(cfg.ss_pin, cfg.rst_pin) {}
// constructor definition for class RfidRc522Reader; stores configuration of cfg object in member variable _cfg; creates MFRC object including these pins

void RfidRc522Reader::begin() {
    pinMode(_cfg.ss_pin, OUTPUT);

    SPI.begin();
    _mfrc.PCD_Init(); // initializes MFRC522 chip
    _next_poll_ms = 0;
}

void RfidRc522Reader::update(uint32_t now_ms) {
    if (isTimeReached(now_ms, _next_poll_ms) == false) {
        return;
    }

    _next_poll_ms = now_ms + _cfg.poll_interval_ms;

    if (_uid_pending == true) { // event isn't consumed yet -> don't override
        return;
    }

    // only proceed if card is present and unique serial number (uid) can be successfully read
    if (_mfrc.PICC_IsNewCardPresent() == false) {
        return;
    }
    if (_mfrc.PICC_ReadCardSerial() == false) {
        return;
    }

    _uid_len = _mfrc.uid.size;

    if (_uid_len > sizeof(_uid)) {
        _uid_len = sizeof(_uid);
    }

    for (uint8_t i = 0; i < _uid_len; i++) {
        _uid[i] = _mfrc.uid.uidByte[i]; // copies each byte from reader's uid buffer to local _uid array
    }

    _uid_pending = true; // uid waiting to be processed

    _mfrc.PICC_HaltA(); // avoid immediate duplicate reads
    _mfrc.PCD_StopCrypto1(); // stops the crypto session with the card, which is needed after authentication or reading secure data — releases the card for future use
}

// retrieve and clear previously read uid
bool RfidRc522Reader::consumeUid(uint8_t* out_uid, uint8_t& out_len) {
    if (_uid_pending == false) {
        return false;
    }

    out_len = _uid_len;

    for (uint8_t i = 0; i < _uid_len; i++) {
        out_uid[i] = _uid[i];
    }

    _uid_pending = false;
    return true;
}