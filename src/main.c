#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    load_rom(argv[1]);

    exit(EXIT_SUCCESS);
}

