#include <stdio.h>
#include <stdlib.h>

// Function defination
void load_rom(const char* filename);

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    load_rom(argv[1]);

    exit(EXIT_SUCCESS);
}

void load_rom(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening ROM file: %s\n", filename);
        return;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("File sixe: %zu\n", file_size);
}
