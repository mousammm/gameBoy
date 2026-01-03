#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmu.h"

MMU* mmu_create(void)
{
    MMU* mmu = malloc(sizeof(MMU));
    memset(mmu, 0, sizeof(MMU));
    return mmu;
}

void mmu_init(MMU* mmu, Cartridge *cart)
{
    mmu->cartridge = cart;

    // Clear all memory
    memset(mmu->memory, 0, sizeof(mmu->memory));

    // For type 0x00 only
    // if (cart->cartridge_type == 0x00) {
    if (cart->cartridge_type) {
        printf("Loading type 0x00 ROM (no banking)\n");

        // Bank 0: 0x0000-0x3FFF (16kb)
        // if rom size is > 16kb bank0_size = 16kb else bank0_size = rom size
        size_t bank0_size = (cart->size > 0x4000) ? 0x4000 : cart->size;
        memcpy(&mmu->memory[0x0000], cart->data, bank0_size);

        // Bank 1: 0x4000-0x7FFF (16kb) if rom has it
        if (cart->size > 0x4000) {
            size_t remaning = cart->size - 0x4000; // 20 - 16 = 4kb
            // if 4kb is > 16kb :T=16kb F:4kb
            size_t bank1_size = (remaning > 0x4000) ? 0x4000 : remaning;
            memcpy(&mmu->memory[0x4000], &cart->data[0x4000], bank1_size);
        }

        printf("ROM loaded: %zu bytes mapped to memory\n", cart->size);
    }

    switch (cart->cartridge_type) {
        case 0x00:
            mmu->mbc = mbc_none_create(cart);
            break;

        default:
            printf("Not Implemented for cartridge_type:0x%02X", cart->cartridge_type);
    
    }
}

// 0x00 ROM only
MBC* mbc_none_create(Cartridge* cart)
{
    MBC* mbc = malloc(sizeof(MBC));

    mbc->read_rom = mbc_none_read_rom;
    mbc->read_ram = NULL; // does nothing

    mbc->rom_data = cart->data;
    mbc->rom_size= cart->size;
    mbc->ram_data = NULL;   // No ram
    mbc->ram_size = 0;

    return mbc;
}

uint8_t mbc_none_read_rom(MBC* mbc, uint16_t address)
{
    if (address < mbc->rom_size) {
        return mbc->rom_data[address];
    }
    return 0xFF;
}
