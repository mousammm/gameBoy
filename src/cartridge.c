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
    // check for early termination
    for (int i = 0; i < 16; i++) {
        uint8_t c = cart->rom_data[HEADER_TITLE + i];
        if (c == 0x00 || c == 0x80 || c == 0xC0) {
            title_length = i;
            break;
        }
    }
    
    memcpy(cart->title, &cart->rom_data[HEADER_TITLE], title_length);
    cart->title[title_length] = '\0';

    return cart;
}

void print_rom_info(Cartridge *cart)
{
    printf("Title: %s\n", cart->title);
}
