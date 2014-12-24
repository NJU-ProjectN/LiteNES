#include "mmc.h"
#include "ppu.h"
#include <string.h>

#define MMC_MAX_PAGE_COUNT 256

byte mmc_prg_pages[MMC_MAX_PAGE_COUNT][0x4000];
byte mmc_chr_pages[MMC_MAX_PAGE_COUNT][0x2000];
int mmc_prg_pages_number, mmc_chr_pages_number;

byte memory[0x10000];

inline byte mmc_read(word address)
{
    return memory[address];
}

inline void mmc_write(word address, byte data)
{
    switch (mmc_id) {
        case 0x3: {
            ppu_copy(0x0000, &mmc_chr_pages[data & 3][0], 0x2000);
        }
        break;
    }
    memory[address] = data;
}

inline void mmc_copy(word address, byte *source, int length)
{
    memcpy(&memory[address], source, length);
}

inline void mmc_append_chr_rom_page(byte *source)
{
    memcpy(&mmc_chr_pages[mmc_chr_pages_number++][0], source, 0x2000);
}
