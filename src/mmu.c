#include "mmu.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MMU* mmu_create(void)
{
    MMU* mmu = malloc(sizeof(MMU));
    memset(mmu, 0, sizeof(MMU));
    return mmu;
}

void mmu_init(MMU* mmu, Cartridge *cart)
{
    mmu->cartridge = cart;
    mmu->mbc = NULL;

    // Clear all memory
    memset(mmu->memory, 0, sizeof(mmu->memory));

    // For type 0x00 only
    if (cart->cartridge_type == 0x00) {
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
}

uint8_t mmu_read(MMU *mmu, uint16_t address)
{
    // for 0x00, rom is already mapped to memory 
    // JUst read from it directly
    return mmu->memory[address];
}

void mmu_write(MMU *mmu, uint16_t address, uint8_t value)
{
    // Dont allow wirting to ROM area for type 0x00
    if (address < 0x8000) {
        // ROM area - ignore
        return; 
    }

    // Allow writes to other areas
    mmu->memory[address] = value;
}
