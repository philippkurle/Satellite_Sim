#pragma once

#include <stdint.h>
#include "types.h"

struct UiModel { // displayed on lcd
    Mode mode;
    int16_t centi_v;

    bool water_suspect;
    bool water_confirm;

    SecurityUiState sec;
    uint8_t pin_len;

    uint8_t sec_raw = 0;
    char pin_text[9] = {0};
};

class LcdUi {
    public:

        void begin();

        void update(uint32_t now_ms, const UiModel& m);
           
        void notifyAuthSuccess(uint32_t now_ms);

        void notifyRfidAccepted(uint32_t now_ms);

        void notifyKeyActivity(uint32_t now_ms);

    private:

        uint32_t _next_ms = 0;
        uint32_t _next_page_ms = 0;
        uint8_t _page = 0;

        bool _show_auth_ok = false;
        uint32_t _auth_ok_until_ms = 0;

        bool _force_security = false;
        uint32_t _force_until_ms = 0; 

        void renderPage0(const UiModel& m); // mode + pv

        void renderPage1(const UiModel& m); // leak
        
        void renderPage2(const UiModel& m); // security
};
