#pragma once

#include <stdint.h>
#include "cartridge.h"

// mbc.h - Memory Bank Controller interface
typedef struct MBC {
    // All MBCs must implement these
    uint8_t (*read_rom)(struct MBC* mbc, uint16_t address);
    void (*write_rom)(struct MBC* mbc, uint16_t address, uint8_t value);
    uint8_t (*read_ram)(struct MBC* mbc, uint16_t address);
    void (*write_ram)(struct MBC* mbc, uint16_t address, uint8_t value);
    
    // Common data
    uint8_t* rom_data;
    size_t rom_size;
    uint8_t* ram_data;
    size_t ram_size;
} MBC;

typedef struct {
    // 64KB gameboy address namespace 
    uint8_t memory[0x10000];    // 65536 bytes
 
    // pointer to cartridge (for rom access)
    Cartridge* cartridge;

    // Type 0x00 dosent nedd MBC but wll still point for future
    MBC* mbc;  // NULL for type 0x00
} MMU;

MMU* mmu_create(void);
void mmu_init(MMU* mmu, Cartridge* cart);
void mmu_free(MMU* mmu);

// 0x00 ROM only
MBC* mbc_none_create(Cartridge* cart);
uint8_t mbc_none_read_rom(MBC* mbc, uint16_t address);
