#include "Sensors.h"
#include <Arduino.h>

void Sensors::begin() {
    _next_sample_ms = 0; // initializes when begin() is called

    // first sensor measurements
    _data.solar_data_raw = readAnalogAveraged(pins::PIN_SOLAR, 8);
    _data.water_data_raw = readAnalogAveraged(pins::PIN_WATER, 8);

    _water_suspect_hys.on = thresholds::WATER_SUSPECT_ON; // assigns constant value from theshold namespace to on member variable from object _water_suspect_hys
    _water_suspect_hys.off = thresholds::WATER_SUSPECT_OFF;
    _water_confirm_hys.on = thresholds::WATER_CONFIRM_ON;
    _water_confirm_hys.off = thresholds::WATER_CONFIRM_OFF;

    _pv_warn_hys.on = thresholds::PV_WARN_ON;
    _pv_warn_hys.off = thresholds::PV_WARN_OFF;
    _pv_safe_hys.on = thresholds::PV_SAFE_ON;
    _pv_safe_hys.off = thresholds::PV_SAFE_OFF;

    // set starting state for hysteresis
    _water_suspect_hys.state = (_data.water_data_raw >= _water_suspect_hys.on); // bool
    _data.water_suspect = _water_suspect_hys.state;

    _water_confirm_hys.state = (_data.water_data_raw >= _water_confirm_hys.on);
    _data.water_confirm = _water_confirm_hys.state;

    _pv_warn_hys.state = (_data.solar_data_raw <= _pv_warn_hys.on);
    _data.pv_low_warn = _pv_warn_hys.state;
    
    _pv_safe_hys.state = (_data.solar_data_raw <= _pv_safe_hys.on);
    _data.pv_low_safe = _pv_safe_hys.state;
}

void Sensors::update(uint32_t now_ms) {
    // protects against millis() overflow by casting to int32_t (signed integer)
    if ((int32_t)(now_ms - _next_sample_ms) < 0) {
        return; 
    }
    _next_sample_ms = now_ms + _sample_period_ms;

    _data.solar_data_raw = readAnalogAveraged(pins::PIN_SOLAR, 8); // adds reading to solar_data_raw from struct SensorData
    _data.water_data_raw = readAnalogAveraged(pins::PIN_WATER, 8);

    _data.water_suspect = _water_suspect_hys.update(_data.water_data_raw);
    _data.water_confirm = _water_confirm_hys.update(_data.water_data_raw);

    _data.pv_low_warn = _pv_warn_hys.update(_data.solar_data_raw);
    _data.pv_low_safe = _pv_safe_hys.update(_data.solar_data_raw);
}

const SensorData& Sensors::data() const {
    return _data;
}

uint16_t Sensors::readAnalogAveraged(uint8_t pin, uint8_t samples) {
    if (samples == 0) {
        return 0;
    }

    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        sum += analogRead(pin);
    }
    
    uint32_t avg = (sum + (samples / 2u)) / samples; // rounding to nearest integer
    return static_cast <uint16_t> (avg); // static_cast<type>(expression) -> converts avg to uint16_t via static_cast (explicit and safe for numeric conversions)
}                                                
