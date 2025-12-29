/*
 * cartridge.c
 */
#include "cartridge.h"
#include <stdlib.h>
#include <stdio.h>

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

    // Check if loaded rom has actuall header size
    if (file_size < ROM_HEADER_SIZE) {
        printf("File is too samll to be a valid ROM!\n");
        fclose(file);
    }

    return NULL;
}
