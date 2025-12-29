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
    
    // Cartridge type
    cart->cartridge_type = cart->rom_data[HEADER_CARTIDGE_TYPE];
    
    // Determine if cartridge has RAM and battery
    switch(cart->cartridge_type) {
        case MBC1_RAM:
        case MBC1_RAM_BATTERY:
        case ROM_RAM:
        case ROM_RAM_BATTERY:
        case MMM01_RAM:
        case MMM01_RAM_BATTERY:
        case MBC3_TIMER_RAM_BATTERY:
        case MBC3_RAM:
        case MBC3_RAM_BATTERY:
        case MBC5_RAM:
        case MBC5_RAM_BATTERY:
        case MBC5_RUMBLE_RAM:
        case MBC5_RUMBLE_RAM_BATTERY:
            cart->has_ram = true;
            break;
    }
    
    switch(cart->cartridge_type) {
        case MBC1_RAM_BATTERY:
        case ROM_RAM_BATTERY:
        case MMM01_RAM_BATTERY:
        case MBC3_TIMER_BATTERY:
        case MBC3_TIMER_RAM_BATTERY:
        case MBC3_RAM_BATTERY:
        case MBC5_RAM_BATTERY:
        case MBC5_RUMBLE_RAM_BATTERY:
            cart->battery_backed = true;
            break;
    }

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

    // Color Game Boy flag
    uint8_t cgb_flag = cart->rom_data[HEADER_CGB_FLAG];
    const char* cgb_str = "DMG (Original)"; // most DMG games will be this
    if (cgb_flag == 0x80) cgb_str = "CGB Enhanced";
    else if (cgb_flag == 0xC0) cgb_str = "CGB Only";
    printf("CGB Flag: %s\n", cgb_str);
    
    // Super Game Boy flag
    uint8_t sgb_flag = cart->rom_data[HEADER_SGB_FLAG];
    printf("SGB Flag: %s\n", sgb_flag == 0x03 ? "Supported" : "Not Supported");

    // Old licensee code (common ones)
    printf("Old Licensee Code: 0x%02X\n", cart->old_licensee_code);
    printf("Mask ROM Version: %d\n", cart->mask_rom_version);
    printf("Header Checksum: 0x%02X\n", cart->checksum);
    printf("Global Checksum: 0x%04X\n", cart->global_checksum);

    // Cartridge type string
    const char* cartridge_type_str = "Unknown";
    switch(cart->cartridge_type) {
        case ROM_ONLY: cartridge_type_str = "ROM Only"; break;
        case MBC1: cartridge_type_str = "MBC1"; break;
        case MBC1_RAM: cartridge_type_str = "MBC1+RAM"; break;
        case MBC1_RAM_BATTERY: cartridge_type_str = "MBC1+RAM+BATTERY"; break;
        case MBC2: cartridge_type_str = "MBC2"; break;
        case MBC2_BATTERY: cartridge_type_str = "MBC2+BATTERY"; break;
        case ROM_RAM: cartridge_type_str = "ROM+RAM"; break;
        case ROM_RAM_BATTERY: cartridge_type_str = "ROM+RAM+BATTERY"; break;
        case MBC3_TIMER_BATTERY: cartridge_type_str = "MBC3+TIMER+BATTERY"; break;
        case MBC3_TIMER_RAM_BATTERY: cartridge_type_str = "MBC3+TIMER+RAM+BATTERY"; break;
        case MBC3: cartridge_type_str = "MBC3"; break;
        case MBC3_RAM: cartridge_type_str = "MBC3+RAM"; break;
        case MBC3_RAM_BATTERY: cartridge_type_str = "MBC3+RAM+BATTERY"; break;
        case MBC5: cartridge_type_str = "MBC5"; break;
        case MBC5_RAM: cartridge_type_str = "MBC5+RAM"; break;
        case MBC5_RAM_BATTERY: cartridge_type_str = "MBC5+RAM+BATTERY"; break;
        case MBC5_RUMBLE: cartridge_type_str = "MBC5+RUMBLE"; break;
        case MBC5_RUMBLE_RAM: cartridge_type_str = "MBC5+RUMBLE+RAM"; break;
        case MBC5_RUMBLE_RAM_BATTERY: cartridge_type_str = "MBC5+RUMBLE+RAM+BATTERY"; break;
    }
    printf("Cartridge Type: 0x%02X (%s)\n", cart->cartridge_type, cartridge_type_str);
    
    printf("Battery Backed: %s\n", cart->battery_backed ? "Yes" : "No");
    printf("Has RAM: %s\n", cart->has_ram ? "Yes" : "No");
    printf("====================\n\n");
}

bool verify_checksum(Cartridge *cart)
{
    uint8_t checksum = 0;

    // Header checksum is calculated from 0x134 to 0x14C
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - cart->rom_data[address] - 1;
    }

    return checksum == cart->checksum;
}

void free_cartridge(Cartridge *cart) {
    if (cart) {
        if (cart->rom_data) {
            free(cart->rom_data);
        }
        if (cart->ram_data) {
            free(cart->ram_data);
        }
        free(cart);
    }
}
