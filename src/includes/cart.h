#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;          // Raw rom data
    size_t size;            // Rom file size

    // Cartridge header info (0x100-0x14F)
    uint8_t entry[4];           // 0x100-0x103: entry point
    uint8_t logo[0x30];         // 0x104-0x133: nintendo logo
    char title[16];             // 0x134-0x143: Title
    uint16_t new_licensee;      // 0x144-0x145
    uint8_t sgb_flag;           // 0x146
    uint8_t cartridge_type;     // 0x147: MBC type + RAM + Battery
    uint8_t rom_size;           // 0x148: ROM size code
    uint8_t ram_size;           // 0x149: RAM size code
    uint8_t destination;        // 0x14A: Japanese/Non-Japanese
    uint8_t old_licensee;       // 0x14B
    uint8_t version;            // 0x14C: ROM version
    uint8_t header_checksum;    // 0x14D
    uint16_t global_checksum;   // 0x14E-0x14F
} Cartridge;

Cartridge* load_rom(char *file);
void parse_gb_header(Cartridge* cart);
void print_header(Cartridge* cart);
void free_cartridge(Cartridge* cart);
