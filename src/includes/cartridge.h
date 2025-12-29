/*
 * cartdige.h
 */

#pragma once
#include <stdint.h>

// Memory constants
#define ROM_BANK_SIZE   0x4000      // 16KB
#define RAM_BANK_SIZE   0x2000      // 8KB
#define ROM_HEADER_SIZE 0x150       // 336

// ROM Header offsets
// 0100-0103 Entry point
// 0104-0133 — Nintendo logo
#define HEADER_TITLE            0x134
#define HEADER_CGB_FLAG         0x143
#define HEADER_SGB_FLAG         0x146
#define HEADER_CARTIDGE_TYPE    0x147
#define HEADER_ROM_SIZE         0x148
#define HEADER_RAM_SIZE         0x149
#define HEADER_DESTINATION_CODE 0x14A
#define HEADER_OLD_LICENSE_CODE 0x14B
#define HEADER_MASK_ROM_VERSION 0x14C
#define HEADER_CHECKSUM         0x14D
#define HEADER_GLOBAL_CHECKSUM  0x14E

// Cartridge type 
enum CartridgeType {
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E
};

// ROM sizes
static const uint32_t ROM_SIZES[] = {
    32 * 1024,      // 32KB
    64 * 1024,      // 64KB
    128 * 1024,     // 128KB
    256 * 1024,     // 256KB
    512 * 1024,     // 512KB
    1024 * 1024,    // 1MB
    2048 * 1024,    // 2MB
    4096 * 1024,    // 4MB
    8192 * 1024     // 8MB
};

// RAM sizes
static const uint32_t RAM_SIZES[] = {
    0,              // None
    2 * 1024,       // 2KB
    8 * 1024,       // 8KB
    32 * 1024,      // 32KB
    128 * 1024,     // 128KB
    64 * 1024       // 64KB
};

// Cartridge Structure
typedef struct {
    uint8_t   *rom_data;
    uint32_t  rom_size;
    uint8_t   *ram_data;
    uint32_t  ram_size;
    uint8_t   cartridge_type;
    uint8_t   rom_banks;
    uint8_t   ram_banks;
    bool      battery_backed;
    bool      has_ram;
    char      title[17];
    uint8_t   checksum;
    uint8_t   global_checksum;
    uint8_t   destination_code;
    uint8_t   old_licensee_code;
    uint8_t   mask_rom_version;
} Cartridge;

// Function declaration
Cartridge* load_rom(const char* filename);
