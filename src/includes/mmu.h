/*
 * mmu.h Memory Bus
 * https://gbdev.io/pandocs/Memory_Map.html
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cartridge.h"

// Memory regions 
typedef enum {
    MEM_ROM0        = 0,    // 0x0000 - 0x3FFFF: ROM bank 0 (fixed)
    MEM_ROM1        = 1,    // 0x4000-0x7FFF: ROM Bank 1-127 (switchable)
    MEM_VRAM        = 2,    // 0x8000-0x9FFF: Video RAM
    MEM_ERAM        = 3,    // 0xA000-0xBFFF: External RAM
    MEM_WRAM        = 4,    // 0xC000-0xDFFF: Work RAM
    MEM_WRAM_ECHO   = 5,    // 0xE000-0xFDFF: Echo RAM (mirror of WRAM)
    MEM_OAM         = 6,    // 0xFE00-0xFE9F: Object Attribute Memory
    MEM_UNUSED      = 7,    // 0xFEA0-0xFEFF: Unused (but often used)
    MEM_IO          = 8,    // 0xFF00-0xFF7F: I/O Registers
    MEM_HRAM        = 9,    // 0xFF80-0xFFFE: High RAM
    MEM_IE          = 10    // 0xFFFF: Interrupt Enable Register
} MemoryRegions;

// Memory Management Unit
typedef struct {
    // Memory blocks
    uint8_t vram[0x2000];     // 8KB Video RAM
    uint8_t wram[0x2000];     // 8KB Work RAM
    uint8_t oam[0x00A0];      // 160 bytes OAM (sprites)
    uint8_t hram[0x007F];     // 127 bytes High RAM
    uint8_t io[0x0080];       // 128 bytes I/O registers

    // Cartridge (external)
    Cartridge* cart;
    
    // Interrupt Enable Register
    uint8_t ie_register;
    
    // DMA transfer
    bool dma_active;
    uint16_t dma_source;
    uint8_t dma_counter;
} MMU;

// Function declaration 
void mmu_init(MMU* mmu, Cartridge* cart);
uint8_t mmu_read_byte(MMU* mmu, uint16_t address);
void mmu_write_byte(MMU* mmu, uint16_t address, uint8_t value);
uint16_t mmu_read_word(MMU* mmu, uint16_t address);
void mmu_write_word(MMU* mmu, uint16_t address, uint16_t value);
void mmu_print_memory(MMU* mmu, uint16_t address, int bytes);
