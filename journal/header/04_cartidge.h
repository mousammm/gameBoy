#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Cartridge {
    // ROM Data
    uint8_t* data;
    size_t size;
    
    // RAM Data (if any)
    uint8_t* ram;
    size_t ram_size;
    
    // Header Info (0x100-0x14F)
    uint8_t entry[4];
    uint8_t logo[0x30];
    char title[16];
    uint16_t new_licensee;
    uint8_t sgb_flag;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t destination;
    uint8_t old_licensee;
    uint8_t version;
    uint8_t header_checksum;
    uint16_t global_checksum;
    
    // Save file
    char save_path[256];
    bool battery;
    
    // Multi-game cartridge (for MBC1 multi-cart mode)
    bool multicart;
} Cartridge;

// Public interface
Cartridge* cartridge_create(void);
bool cartridge_load(Cartridge* cart, const char* path);
void cartridge_free(Cartridge* cart);

// Header validation
bool cartridge_verify_header(Cartridge* cart);
bool cartridge_verify_logo(Cartridge* cart);

// Save/RAM management
void cartridge_load_ram(Cartridge* cart);
void cartridge_save_ram(Cartridge* cart);

// ROM/RAM size helpers
uint32_t cartridge_get_rom_size(uint8_t size_code);
uint32_t cartridge_get_ram_size(uint8_t size_code);
const char* cartridge_get_type_name(uint8_t type);

#endif
