#ifndef MMU_H
#define MMU_H

#include "cartridge.h"
#include "mbc.h"

typedef struct MMU {
    // 64KB address space
    uint8_t memory[0x10000];
    
    // Components
    Cartridge* cart;
    MBC* mbc;
    
} MMU;

// Public interface
MMU* mmu_create(void);
void mmu_init(MMU* mmu, Cartridge* cart);
void mmu_free(MMU* mmu);

// Memory access
uint8_t mmu_read(MMU* mmu, uint16_t address);
void mmu_write(MMU* mmu, uint16_t address, uint8_t value);
uint16_t mmu_read16(MMU* mmu, uint16_t address);
void mmu_write16(MMU* mmu, uint16_t address, uint16_t value);

#endif
