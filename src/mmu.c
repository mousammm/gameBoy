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

// Read byte from memory
uint8_t mmu_read_byte(MMU* mmu, uint16_t address)
{
    if (!mmu) return 0xFF;

    // Memory map decoding
    if (address < 0x8000) {
        // ROM area - handled by cartridge
        if (!mmu->cart || !mmu->cart->rom_data) {
            return 0xFF;
        }
        
        // Simple ROM_ONLY implementation for now
        // TODO: Implement proper banking
        if (address < mmu->cart->rom_size) {
            return mmu->cart->rom_data[address];
        }
        return 0xFF;
    }
    else if (address < 0xA000) {
        // Video RAM
        return mmu->vram[address - 0x8000];
    }
    else if (address < 0xC000) {
        // External RAM (cartridge)
        if (mmu->cart && mmu->cart->ram_data) {
            // TODO: Implement banking
            uint32_t offset = address - 0xA000;
            if (offset < mmu->cart->ram_size) {
                return mmu->cart->ram_data[offset];
            }
        }
        return 0xFF;
    }
    else if (address < 0xE000) {
        // Work RAM
        return mmu->wram[address - 0xC000];
    }
    else if (address < 0xFE00) {
        // Echo RAM (mirror of WRAM)
        return mmu->wram[address - 0xE000];
    }
    else if (address < 0xFEA0) {
        // OAM (Sprite attribute table)
        return mmu->oam[address - 0xFE00];
    }
    else if (address < 0xFF00) {
        // Unusable area (often returns 0x00)
        return 0x00;
    }
    else if (address < 0xFF80) {
        // I/O registers
        uint8_t io_addr = address - 0xFF00;
        if (io_addr < 0x80) {
            return mmu->io[io_addr];
        }
        return 0xFF;
    }
    else if (address < 0xFFFF) {
        // High RAM
        return mmu->hram[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Register
        return mmu->ie_register;
    }

    return 0xFF;    // Open bus (shouldnt happen)
}

// Write byte to memory
void mmu_write_byte(MMU* mmu, uint16_t address, uint8_t value)
{
    if (!mmu) return;
    
    // Memory map decoding
    if (address < 0x8000) {
        // ROM area - usually bank switching writes
        // TODO: Implement MBC handling
        // For now, ignore writes to ROM
        return;
    }
    else if (address < 0xA000) {
        // Video RAM
        mmu->vram[address - 0x8000] = value;
    }
    else if (address < 0xC000) {
        // External RAM (cartridge)
        if (mmu->cart && mmu->cart->ram_data) {
            uint32_t offset = address - 0xA000;
            if (offset < mmu->cart->ram_size) {
                mmu->cart->ram_data[offset] = value;
            }
        }
    }
    else if (address < 0xE000) {
        // Work RAM
        mmu->wram[address - 0xC000] = value;
    }
    else if (address < 0xFE00) {
        // Echo RAM (mirror of WRAM)
        mmu->wram[address - 0xE000] = value;
    }
    else if (address < 0xFEA0) {
        // OAM (Sprite attribute table)
        mmu->oam[address - 0xFE00] = value;
    }
    else if (address < 0xFF00) {
        // Unusable area (writes ignored)
        return;
    }
    else if (address < 0xFF80) {
        // I/O registers
        uint8_t io_addr = address - 0xFF00;
        if (io_addr < 0x80) {
            // Handle special I/O registers
            switch (io_addr) {
                case 0x00:  // JOYP - Joypad
                    mmu->io[io_addr] = value | 0xCF;  // Upper bits always 1
                    break;
                case 0x04:  // DIV - Divider register (writes reset to 0)
                    mmu->io[io_addr] = 0x00;
                    break;
                case 0x46:  // DMA - OAM DMA transfer
                    mmu->io[io_addr] = value;
                    // Start DMA transfer
                    mmu->dma_active = true;
                    mmu->dma_source = value << 8;
                    mmu->dma_counter = 0;
                    break;
                default:
                    mmu->io[io_addr] = value;
                    break;
            }
        }
    }
    else if (address < 0xFFFF) {
        // High RAM
        mmu->hram[address - 0xFF80] = value;
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Register
        mmu->ie_register = value;
    }
}

// Read 16-bit word (little-endian)
uint16_t mmu_read_word(MMU* mmu, uint16_t address) {
    uint8_t low = mmu_read_byte(mmu, address);
    uint8_t high = mmu_read_byte(mmu, address + 1);
    return (high << 8) | low;
}

// Write 16-bit word (little-endian)
void mmu_write_word(MMU* mmu, uint16_t address, uint16_t value) {
    mmu_write_byte(mmu, address, value & 0xFF);
    mmu_write_byte(mmu, address + 1, value >> 8);
}

// Print memory for debugging
void mmu_print_memory(MMU* mmu, uint16_t address, int bytes) {
    printf("Memory at 0x%04X:\n", address);
    for (int i = 0; i < bytes; i++) {
        if (i % 16 == 0) {
            if (i > 0) printf("\n");
            printf("0x%04X: ", address + i);
        }
        printf("%02X ", mmu_read_byte(mmu, address + i));
    }
    printf("\n");
}
