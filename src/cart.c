#include "cart.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;          // Raw rom data
    size_t size;            // Rom file size
} Cartridge;

void load_rom(char *file)
{
    // open rom file
    FILE *f = fopen(file, "rb");
    Cartridge* cart = malloc(sizeof(Cartridge));
    
    // check rom size
    fseek(f, 0, SEEK_END);
    cart->size = ftell(f);
    rewind(f);

    // get rom binary data
    cart->data = malloc(cart->size);
    fread(cart->data, 1, cart->size, f);
    fclose(f);

    printf("ROM size:%ld\n",cart->size);
}
