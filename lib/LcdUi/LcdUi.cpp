#include "LcdUi.h"
#include <Wire.h> // Standardbibliothek für I²C-Kommunikation
#include <LiquidCrystal_I2C.h> // Spezialbibliothek für I²C-Display-Steuerung, basiert auf <Wire.h>
#include "config_ui.h"
#include <Arduino.h>

static LiquidCrystal_I2C lcd(ui::LCD_ADDR, 16, 2);

static void printPadded(uint8_t col, uint8_t row, const char* s) {
    const uint8_t lcd_cols = 16;

    if (col >= lcd_cols) {
        return;
    }

    const uint8_t room = lcd_cols - col;
    lcd.setCursor(col, row);

    uint8_t i = 0;
    if (s != nullptr) {
        for (; i < room && s[i] != '\0'; i++) { // ; greift auf vorher definiertes i zu
            lcd.write((uint8_t)s[i]);
        }
    }
    for (; i < room; i++) {
        lcd.write((uint8_t)' ');
    }
}

static void printPadded(uint8_t col, uint8_t row, const String& s) {
    printPadded(col, row, s.c_str());
}

static const char* modeStr(Mode m) {
    switch (m) {
        case Mode::NOMINAL:
            return "NOMINAL";
        case Mode::WARN:
            return "WARN";
        case Mode::SAFE:
            return "SAFE";
        default:
            return "UNKNOWN";
    }
}

void LcdUi::begin() {
    Wire.begin();
    Wire.setClock(400000); // 400kHz statt oft 100kHz
    lcd.init();
    lcd.backlight();
    printPadded(0, 0, String("Booting"));
    printPadded(0, 1, String("UI init"));
    _next_ms = 0;
    _next_page_ms = 0;
}

void LcdUi::update(uint32_t now_ms, const UiModel& m) {
    const bool forced_active = _force_security && ((int32_t)(now_ms - _force_until_ms) < 0);

    if (forced_active == false) {
        if ((int32_t)(now_ms - _next_page_ms) >= 0) {
            _page = (_page + 1) % 3;
            _next_page_ms =  now_ms + ui::LCD_PAGE_ROTATE_MS;
        }
    } else {
        _page = 2;
    }

    if (_force_security == true && forced_active == false) {
        _force_security = false;
        _next_page_ms = now_ms + ui::LCD_PAGE_ROTATE_MS;
    }

    if (_show_auth_ok == true && (int32_t)(now_ms - _auth_ok_until_ms) >= 0) {
        _show_auth_ok = false;
    }

    const uint16_t LCD_RENDER_PERIOD_MS = 150;

    if ((int32_t)(now_ms - _next_ms) < 0) {
        return;
    }
    _next_ms = now_ms + LCD_RENDER_PERIOD_MS;

    if (_show_auth_ok == true) {
        printPadded(0, 0, String("AUTH: SUCCESS"));
        printPadded(0, 1, String("Welcome"));
        return;
    }
    switch (_page) {
        case 0:
            renderPage0(m);
            break;
        case 1:
            renderPage1(m);
            break;
        case 2:
            renderPage2(m);
            break;
        default:
            renderPage0(m);
            break;
    }
}

void LcdUi::notifyAuthSuccess(uint32_t now_ms) {
    _show_auth_ok = true;
    _auth_ok_until_ms = now_ms + 1000;
    _next_ms = now_ms; // sofortiges LCD update
}

void LcdUi::notifyRfidAccepted(uint32_t now_ms) {
    _force_security = true;
    _force_until_ms = now_ms + 6000;
    _page = 2;

    _next_ms = now_ms;
}

void LcdUi::notifyKeyActivity(uint32_t now_ms) {
    if (_force_security == true) {
        _force_until_ms = now_ms + 6000;
        _next_ms = now_ms;
    }
}

void LcdUi::renderPage0(const UiModel& m) {
    // Mode + Pv
    char buf[17];

    // PV z.B. "PV: 12.34"
    int16_t cv = m.centi_v;
    int16_t whole = cv / 100;
    int16_t frac = cv % 100;

    snprintf(buf, sizeof(buf), "MODE:%-7s", modeStr(m.mode));
    printPadded(0, 0, buf);

    snprintf(buf, sizeof(buf), "PV: %2d.%02d", whole, frac);
    printPadded(0, 1, buf);
}

void LcdUi::renderPage1(const UiModel& m) {
    // Leak Status
    char line1[17];
    printPadded(0, 0, "Leak Status");
    
    const char* s = m.water_confirm ? " CONF" : (m.water_suspect ? " SUS" : " -");
    snprintf(line1, sizeof(line1), "LEAK:%s", s);
    printPadded(0, 1, line1);
}

void LcdUi::renderPage2(const UiModel& m) {
    // Security
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "Security");
    printPadded(0,0, line0);
    
    switch (m.sec) {
        case SecurityUiState::WAIT_RFID:
            printPadded(0, 1, "Rfid: waiting...");
            break;
        case SecurityUiState::WAIT_CODE: {
            char masked[9];
            uint8_t n = m.pin_len;

            if (n > 8) {
                n = 8;
            }
            for (uint8_t i = 0; i < n; i++) {
                masked[i] = '*';
            }
            if (n > 0) {
                masked[n - 1] = m.pin_text[n - 1];
            }
            masked[n] = '\0';

            snprintf(line1, sizeof(line1), "PIN: %s", masked);
            printPadded(0, 1, line1);
            break;
        }
        case SecurityUiState::LOCKOUT:
            printPadded(0, 1, "LOCKOUT");
            break;
        case SecurityUiState::AUTH_OK:
            printPadded(0, 1, "Auth OK");
            break;
        default:
            printPadded(0, 1, "SEC: ?");
            break;
    };
}


