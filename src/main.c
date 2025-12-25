#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint8_t* data;
    size_t size;
    uint8_t rom_size_code;
    uint8_t ram_size_code;
    uint8_t cartridge_type;
    uint8_t rom_banks;
    uint8_t ram_banks;
    char title[17];
} Cartridge;

// function defination
Cartridge *load_rom(const char *filename);
void free_cartridge(Cartridge* cart);

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    // open the gb rom file
    Cartridge *cart = load_rom(argv[1]);

    free_cartridge(cart);
    exit(EXIT_SUCCESS);
}

Cartridge* load_rom(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening ROM file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory for ROM data
    Cartridge* cart = malloc(sizeof(Cartridge));
    cart->data = malloc(file_size);
    cart->size = file_size;
    
    // Read entire ROM
    fread(cart->data, 1, file_size, file);
    fclose(file);
    
    // Parse header
    // Title (0x0134-0x0143)
    memcpy(cart->title, &cart->data[0x0134], 16);
    cart->title[16] = '\0'; // Null terminate
    
    // Cartridge type (0x0147)
    cart->cartridge_type = cart->data[0x0147];
    
    // ROM size (0x0148)
    cart->rom_size_code = cart->data[0x0148];
    
    // RAM size (0x0149)
    cart->ram_size_code = cart->data[0x0149];
    
    // Calculate banks
    switch(cart->rom_size_code) {
        case 0x00: cart->rom_banks = 2; break;
        case 0x01: cart->rom_banks = 4; break;
        case 0x02: cart->rom_banks = 8; break;
        case 0x03: cart->rom_banks = 16; break;
        case 0x04: cart->rom_banks = 32; break;
        case 0x05: cart->rom_banks = 64; break;
        case 0x06: cart->rom_banks = 128; break;
        case 0x07: cart->rom_banks = 256; break;
        case 0x08: cart->rom_banks = 512; break;
        case 0x52: cart->rom_banks = 72; break;
        case 0x53: cart->rom_banks = 80; break;
        case 0x54: cart->rom_banks = 96; break;
        default: cart->rom_banks = 2; break;
    }
    
    // Calculate RAM banks
    switch(cart->ram_size_code) {
        case 0x00: cart->ram_banks = 0; break;
        case 0x01: cart->ram_banks = 1; break;  // 2KB
        case 0x02: cart->ram_banks = 1; break;  // 8KB
        case 0x03: cart->ram_banks = 4; break;  // 32KB
        case 0x04: cart->ram_banks = 16; break; // 128KB
        case 0x05: cart->ram_banks = 8; break;  // 64KB
        default: cart->ram_banks = 0; break;
    }
    
    printf("Loaded ROM: %s\n", cart->title);
    printf("Size: %zu bytes (%d banks)\n", file_size, cart->rom_banks);
    printf("Cartridge Type: 0x%02X\n", cart->cartridge_type);
    printf("RAM: %d banks\n", cart->ram_banks);
    
    return cart;
}

void free_cartridge(Cartridge* cart) {
    free(cart->data);
    free(cart);
}
