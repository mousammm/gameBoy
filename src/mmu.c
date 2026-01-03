#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mmu.h"

MMU* mmu_create(void) {
    MMU* mmu = malloc(sizeof(MMU));
    memset(mmu, 0, sizeof(MMU));
    return mmu;
}

void mmu_init(MMU* mmu, Cartridge* cart) {
    mmu->cart = cart;
    
    // Clear all memory
    memset(mmu->memory, 0, sizeof(mmu->memory));
    
    // Create appropriate MBC based on cartridge type
    switch(cart->cartridge_type) {
        case 0x00:  // ROM ONLY
            mmu->mbc = mbc_none_create(cart);
            printf("Created MBC: ROM ONLY (type 0x00)\n");
            break;
            
        case 0x01:  // MBC1
        case 0x02:  // MBC1 + RAM
        case 0x03:  // MBC1 + RAM + BATTERY
            // mmu->mbc = mbc1_create(cart);  // Implement later
            printf("MBC1 not implemented yet (type 0x%02X)\n", 
                   cart->cartridge_type);
            // Fall back to ROM-only for now
            mmu->mbc = mbc_none_create(cart);
            break;
            
        default:
            printf("Unsupported cartridge type: 0x%02X\n", 
                   cart->cartridge_type);
            // Fall back to ROM-only
            mmu->mbc = mbc_none_create(cart);
            break;
    }
    
    // DO NOT copy ROM to memory array!
    // MBC will handle ROM reads dynamically
}

uint8_t mmu_read(MMU* mmu, uint16_t address) {
    // Handle different memory areas
    if (address < 0x8000) {
        // ROM area - delegate to MBC
        return mmu->mbc->read_rom(mmu->mbc, address);
    }
    else if (address >= 0xA000 && address < 0xC000) {
        // External RAM area - delegate to MBC
        return mmu->mbc->read_ram(mmu->mbc, address);
    }
    else {
        // Internal memory (VRAM, WRAM, OAM, IO, HRAM)
        return mmu->memory[address];
    }
}

void mmu_write(MMU* mmu, uint16_t address, uint8_t value) {
    // Handle ROM area writes (for banking)
    if (address < 0x8000) {
        // Delegate to MBC (may change banking)
        mmu->mbc->write_rom(mmu->mbc, address, value);
        return;
    }
    else if (address >= 0xA000 && address < 0xC000) {
        // External RAM writes
        mmu->mbc->write_ram(mmu->mbc, address, value);
        return;
    }
    else {
        // Internal memory writes
        mmu->memory[address] = value;
    }
}

void mmu_free(MMU* mmu) {
    if (mmu->mbc) {
        free(mmu->mbc->ram_data);
        free(mmu->mbc);
    }
    free(mmu);
}
