/*
 * mmu.c
 */
#include "mmu.h"
#include <stdio.h>
#include <string.h>

void mmu_init(MMU* mmu, Cartridge* cart)
{
    if (!mmu) return;

    memset(mmu, 0, sizeof(MMU));

    // Clear memory regions
    memset(mmu->vram, 0, sizeof(mmu->vram));
    memset(mmu->wram, 0, sizeof(mmu->wram));
    memset(mmu->oam, 0, sizeof(mmu->oam));
    memset(mmu->hram, 0, sizeof(mmu->hram));
    memset(mmu->io, 0, sizeof(mmu->io));

    // Set default I/O register values (after boot)
    mmu->io[0x00] = 0xCF;  // P1/JOYP
    mmu->io[0x01] = 0x00;  // SB (Serial transfer data)
    mmu->io[0x02] = 0x7E;  // SC (Serial transfer control)
    mmu->io[0x04] = 0xAB;  // DIV (Divider register)
    mmu->io[0x05] = 0x00;  // TIMA (Timer counter)
    mmu->io[0x06] = 0x00;  // TMA (Timer modulo)
    mmu->io[0x07] = 0xF8;  // TAC (Timer control)
    mmu->io[0x0F] = 0xE1;  // IF (Interrupt flag)
    mmu->io[0x10] = 0x80;  // NR10 (Sound channel 1)
    mmu->io[0x11] = 0xBF;  // NR11
    mmu->io[0x12] = 0xF3;  // NR12
    mmu->io[0x14] = 0xBF;  // NR14
    mmu->io[0x16] = 0x3F;  // NR21
    mmu->io[0x17] = 0x00;  // NR22
    mmu->io[0x19] = 0xBF;  // NR24
    mmu->io[0x1A] = 0x7F;  // NR30
    mmu->io[0x1B] = 0xFF;  // NR31
    mmu->io[0x1C] = 0x9F;  // NR32
    mmu->io[0x1E] = 0xBF;  // NR33
    mmu->io[0x20] = 0xFF;  // NR41
    mmu->io[0x21] = 0x00;  // NR42
    mmu->io[0x22] = 0x00;  // NR43
    mmu->io[0x23] = 0xBF;  // NR44
    mmu->io[0x24] = 0x77;  // NR50
    mmu->io[0x25] = 0xF3;  // NR51
    mmu->io[0x26] = 0xF1;  // NR52 (Sound on/off)
    mmu->io[0x40] = 0x91;  // LCDC (LCD control)
    mmu->io[0x41] = 0x85;  // STAT (LCD status)
    mmu->io[0x42] = 0x00;  // SCY (Scroll Y)
    mmu->io[0x43] = 0x00;  // SCX (Scroll X)
    mmu->io[0x45] = 0x00;  // LYC (LY compare)
    mmu->io[0x47] = 0xFC;  // BGP (Background palette)
    mmu->io[0x48] = 0xFF;  // OBP0 (Object palette 0)
    mmu->io[0x49] = 0xFF;  // OBP1 (Object palette 1)
    mmu->io[0x4A] = 0x00;  // WY (Window Y)
    mmu->io[0x4B] = 0x00;  // WX (Window X)

    mmu->cart = cart;
    mmu->ie_register = 0x00;
    mmu->dma_active = false;
}
