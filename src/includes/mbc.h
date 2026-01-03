#pragma once

#include <stdint.h>
#include "cartridge.h"

typedef struct MBC MBC;

// All MBCs must implement these
typedef uint8_t (*MBC_ReadRomFunc)(MBC* mbc, uint16_t address);
typedef void (*MBC_WriteRomFunc)(MBC* mbc, uint16_t address, uint8_t value);
typedef uint8_t (*MBC_ReadRamFunc)(MBC* mbc, uint16_t address);
typedef void (*MBC_WriteRamFunc)(MBC* mbc, uint16_t address, uint8_t value);

// mbc.h - Memory Bank Controller interface
typedef struct MBC {
    // Virtual function table
    MBC_ReadRomFunc read_rom;
    MBC_WriteRomFunc write_rom;
    MBC_ReadRamFunc read_ram;
    MBC_WriteRamFunc write_ram;
    
    // Common data
    Cartridge* cart;  // Reference to cartridge
    uint8_t* ram_data;
    size_t ram_size;
    
    // Type-specific data (use a union or void*)
    void* type_data;  // For MBC1, MBC2, etc. specific data
} MBC;

// Type-specific creators
MBC* mbc_none_create(Cartridge* cart);
MBC* mbc1_create(Cartridge* cart);
