#include "cartridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Cartridge* load_rom(char *file)
{
    // open rom file
    FILE *f = fopen(file, "rb");
    // TODO: !f
    Cartridge* cart = malloc(sizeof(Cartridge));
    // TODO: !cart
    
    // check rom size
    fseek(f, 0, SEEK_END);
    cart->size = ftell(f);
    rewind(f);

    // put rom binary data in Cartridge->data
    cart->data = malloc(cart->size);
    fread(cart->data, 1, cart->size, f);
    fclose(f);

    // parse gameboy header $100-$14F
    parse_gb_header(cart);
    print_header(cart);

    return cart;
}

void parse_gb_header(Cartridge* cart)
{
    // Parse cartdige header (starts from $100 in ROM)
    memcpy(cart->entry, &cart->data[0x100], 4);
    memcpy(cart->logo, &cart->data[0x104], 0x30);

    // title
    int title_len = 0;
    for (int i = 0; i < 16; i++) {
        char c = cart->data[0x134 + i];
        if (c == 0) break;
        cart->title[i] = c;
        title_len++;
    }
    cart->title[title_len] = '\0';

    // Extract other header info
    cart->cartridge_type = cart->data[0x147];
    cart->rom_size = cart->data[0x148];
    cart->ram_size = cart->data[0x149];
    cart->destination = cart->data[0x14A];
    cart->version = cart->data[0x14C];
    cart->header_checksum = cart->data[0x14D];
}

void print_header(Cartridge* cart)
{
    // printf(":%0X\n",cart->logo[0]);
    printf("\n\t%s\n",cart->title);
    printf("\tROM size:%ld\n",cart->size);
    printf("\tCartridge Type: 0x%02X\n", cart->cartridge_type);
    printf("\tROM Size Code: 0x%02X\n", cart->rom_size);
    printf("\tRAM Size Code: 0x%02X\n\n", cart->ram_size);
}

void cartridge_free(Cartridge* cart)
{
    free(cart->data);
    free(cart);
}
