#include "psg.h"
#include "hal.h"

static byte prev_write;
static int p = 10;

inline byte psg_io_read(word address)
{
    // Joystick 1
    if (address == 0x4016) {
        if (p++ < 9) {
            return nes_key_state(p);
        }
    }
    return 0;
}

inline void psg_io_write(word address, byte data)
{
    if (address == 0x4016) {
        if ((data & 1) == 0 && prev_write == 1) {
            // strobe
            p = 0;
        }
    }
    prev_write = data & 1;
}
