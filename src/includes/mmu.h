#pragma once

#include <cstdint>
#include "cartridge.h"

typedef struct {
    // 64KB gameboy address namespace 
    uint8_t memory[0x10000];    // 65536 bytes
 
    // pointer to cartridge (for rom access)
    Cartridge* cartridge;

    // Type 0x00 dosent nedd MBC but wll still point for future
    void* mbc;  // NULL for type 0x00

} MMU;

MMU* mmu_create(void);
void mmu_init(MMU* mmu, Cartridge* cart);
void mmu_free(MMU* mmu);

// memory access
uint8_t mmu_read(MMU* mmu, uint16_t address);
void mmu_write(MMU* mmu, uint16_t address, uint8_t value);
