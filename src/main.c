#include <stdio.h>
#include <stdlib.h>

// function defination
void open_gb_rom_file(const char *rom_file);

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    // open the gb rom file
    open_gb_rom_file(argv[1]);

    exit(EXIT_SUCCESS);
}

void open_gb_rom_file(const char *rom_file)
{
    FILE *file;
    unsigned char buffer[1024];
    size_t bytes_read;
    
    file = fopen(rom_file, "rb");
    if(!file){
	printf("Cant open:%s\n", rom_file);
        fclose(file);
	exit(EXIT_FAILURE);
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%02X ", buffer[i]);
        }
    }

    fclose(file);
}
