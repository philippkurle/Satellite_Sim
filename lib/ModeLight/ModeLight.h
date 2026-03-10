#pragma once

#include <stdint.h>
#include "types.h"

class ModeLight {
    public:

        void begin();

        void setMode(Mode m);

    private:

        void write(bool r, bool y, bool g);
};