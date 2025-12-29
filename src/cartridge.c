/*
 * cartridge.c
 */
#include "cartridge.h"
#include <stdlib.h>     // malloc
#include <stdio.h>      // printf
#include <string.h>     // memcpy

Cartridge* load_rom(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening ROM file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // TODO: file_size < ROM_HEADER_SIZE

    // Allocates memory for cartridge 
    Cartridge *cart = (Cartridge*)malloc(sizeof(Cartridge));
    // TODO: !cart
    
    memset(cart, 0, sizeof(Cartridge));

    // Read ROM data
    cart->rom_data = (uint8_t*)malloc(file_size);
    // TODO: !cart->rom_data

    // put romFile data in cart->rom_data
    size_t bytes_read = fread(cart->rom_data, 1, file_size, file);
    fclose(file);

    // TODO: bytes_read != file_size

    cart->rom_size = file_size;

    // Parse ROM headers
    // TITLE (16char max, but some have 15 or 11)
    int title_length = 16;
    memcpy(cart->title, &cart->rom_data[HEADER_TITLE], title_length);
    cart->title[title_length] = '\0';

    // Cartridge type
    cart->cartridge_type = cart->rom_data[HEADER_CARTIDGE_TYPE];
    // TODO: Check if Cartridge has ram and battery

    // ROM size
    uint8_t rom_size_code = cart->rom_data[HEADER_ROM_SIZE];
    if (rom_size_code < sizeof(ROM_SIZES)/sizeof(ROM_SIZES[0])) {
        cart->rom_banks = (1 << (rom_size_code + 1));
    } else {
        cart->rom_banks = 2; // Default to 32kb
    }

    // RAM size
    uint8_t ram_size_code = cart->rom_data[HEADER_RAM_SIZE];
    if (ram_size_code < sizeof(RAM_SIZES)/sizeof(RAM_SIZES[0])) {
        cart->ram_size = ROM_SIZES[ram_size_code];
        if (cart->ram_size > 0) {
            cart->ram_banks = cart->ram_size / RAM_BANK_SIZE;
            cart->ram_data = (uint8_t*)calloc(cart->ram_size, 1);
            // TODO: !cart->ram_data && cart->ram_size > 0
        }
    }

    // Destination code 
    cart->destination_code = cart->rom_data[HEADER_DESTINATION_CODE];
    // Old license code 
    cart->old_licensee_code = cart->rom_data[HEADER_OLD_LICENSE_CODE];
    // MASK rom version
    cart->mask_rom_version = cart->rom_data[HEADER_MASK_ROM_VERSION];

    // Checksums
    cart->checksum = cart->rom_data[HEADER_CHECKSUM];
    cart->global_checksum = (cart->rom_data[HEADER_GLOBAL_CHECKSUM] << 8) | 
                            cart->rom_data[HEADER_GLOBAL_CHECKSUM + 1];

    return cart;
}

void print_rom_info(Cartridge *cart)
{
    printf("====================\n\n");
    printf("Title: %s\n", cart->title);
    printf("ROM Size: %u bytes (%d banks)\n", cart->rom_size, cart->rom_banks);
    printf("RAM Size: %u bytes (%d banks)\n", cart->ram_size, cart->ram_banks);

    // Destination code
    const char* destination_str = "Unknown";
    switch(cart->destination_code) {
        case 0x00: destination_str = "Japanese"; break;
        case 0x01: destination_str = "Non-Japanese"; break;
    }
    printf("Destination: %s\n", destination_str);

    // Old licensee code (common ones)
    printf("Old Licensee Code: 0x%02X\n", cart->old_licensee_code);
    printf("Mask ROM Version: %d\n", cart->mask_rom_version);
    printf("Header Checksum: 0x%02X\n", cart->checksum);
    printf("Global Checksum: 0x%04X\n", cart->global_checksum);
    printf("====================\n\n");
}
