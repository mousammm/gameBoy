#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;          // Raw rom data
    size_t size;            // Rom file size

    // Cartridge header info ($100-$14F)
    uint8_t entry[4];           // $100-$103: entry point
    uint8_t logo[0x30];         // $104-$133: nintendo logo
    char title[16];             // $104-$133: nintendo logo
    uint16_t new_licensee;      // $144-$145
    uint8_t sgb_flag;           // $146
    uint8_t cartridge_type;     // $147: MBC type + RAM + Battery
    uint8_t rom_size;           // $148: ROM size code
    uint8_t ram_size;           // $149: RAM size code
    uint8_t destination;        // $14A: Japanese/Non-Japanese
    uint8_t old_licensee;       // $14B
    uint8_t version;            // $14C: ROM version
    uint8_t header_checksum;    // $14D
    uint16_t global_checksum;   // $14E-0x14F
} Cartridge;

Cartridge* load_rom(char *file);
void parse_gb_header(Cartridge* cart);
void print_header(Cartridge* cart);
void free_cartridge(Cartridge* cart);
