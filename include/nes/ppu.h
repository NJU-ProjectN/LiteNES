#include "common.h"

#ifndef PPU_H
#define PPU_H

byte PPU_SPRRAM[0x100];
byte PPU_RAM[0x4000];

void ppu_init();
void ppu_finish();

byte ppu_ram_read(word address);
void ppu_ram_write(word address, byte data);
byte ppu_io_read(word address);
void ppu_io_write(word address, byte data);

bool ppu_generates_nmi();
void ppu_set_generates_nmi(bool yesno);

void ppu_set_mirroring(byte mirroring);

void ppu_run(int cycles);
void ppu_cycle();
int ppu_scanline();
void ppu_set_scanline(int s);
void ppu_copy(word address, byte *source, int length);
void ppu_sprram_write(byte data);

// PPUCTRL
bool ppu_shows_background();
bool ppu_shows_sprites();
bool ppu_in_vblank();
void ppu_set_in_vblank(bool yesno);


#endif
