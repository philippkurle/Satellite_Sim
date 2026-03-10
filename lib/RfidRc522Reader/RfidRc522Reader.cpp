#include "RfidRc522Reader.h"

RfidRc522Reader::RfidRc522Reader(const cfg::RfidConfig& cfg) : _cfg(cfg), _mfrc(cfg.ss_pin, cfg.rst_pin) {}

void RfidRc522Reader::begin() {
    pinMode(_cfg.ss_pin, OUTPUT);

    SPI.begin();
    _mfrc.PCD_Init();
    _next_poll_ms = 0;
}

void RfidRc522Reader::update(uint32_t now_ms) {
    if (isTimeReached(now_ms, _next_poll_ms) == false) {
        return;
    }
    _next_poll_ms = now_ms + _cfg.poll_interval_ms;

    if (_uid_pending == true) { // event noch nicht konsumiert -> nicht überschreiben
        return;
    }
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
        _uid[i] = _mfrc.uid.uidByte[i];
    }
    _uid_pending = true;

    // damit Karte sauber freigegeben wird
    _mfrc.PICC_HaltA();
    _mfrc.PCD_StopCrypto1();
}

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