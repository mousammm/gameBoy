#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mmu.h"

MMU* mmu_create(void) {
    MMU* mmu = malloc(sizeof(MMU));
    memset(mmu, 0, sizeof(MMU));
    return mmu;
}

int mmu_init(MMU* mmu, Cartridge* cart) {
    mmu->cart = cart;
    
    // Clear all memory
    memset(mmu->memory, 0, sizeof(mmu->memory));

    // Set up some default register values
    mmu->memory[0xFF05] = 0x00;  // TIMA
    mmu->memory[0xFF06] = 0x00;  // TMA
    mmu->memory[0xFF07] = 0x00;  // TAC
    mmu->memory[0xFF40] = 0x91;  // LCDC
    mmu->memory[0xFF42] = 0x00;  // SCY
    mmu->memory[0xFF43] = 0x00;  // SCX
    mmu->memory[0xFF45] = 0x00;  // LYC
    mmu->memory[0xFF47] = 0xFC;  // BGP
    mmu->memory[0xFF48] = 0xFF;  // OBP0
    mmu->memory[0xFF49] = 0xFF;  // OBP1
    mmu->memory[0xFF4A] = 0x00;  // WY
    mmu->memory[0xFF4B] = 0x00;  // WX
    mmu->memory[0xFFFF] = 0x00;  // IE
    
    // Create appropriate MBC based on cartridge type
    switch(cart->cartridge_type) {
        case 0x00:  // ROM ONLY
            mmu->mbc = mbc_none_create(cart);
            printf("Cartridge Type (type 0x00)\n");
            return 0;
            break;
            
        case 0x01:  // MBC1
        case 0x02:  // MBC1 + RAM
        case 0x03:  // MBC1 + RAM + BATTERY
            // mmu->mbc = mbc1_create(cart);  // Implement later
            printf("Cartridge Type NOT Implemented(type 0x%02X)\n",cart->cartridge_type);
            mmu->mbc = mbc_none_create(cart);
            return 0;
            break;
            
        default:
            printf("Cartridge Type NOT Implemented(type 0x%02X)\n",cart->cartridge_type);
            return -1;
            break;
    }
}

void mmu_free(MMU* mmu) {
    if (mmu->mbc) {
        free(mmu->mbc->ram_data);
        free(mmu->mbc);
    }
    free(mmu);
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
