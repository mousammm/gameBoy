#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"
#include "cpu.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    Cartridge *cart = load_rom(argv[1]);
    // TODO: !cart

    print_rom_info(cart);

    if(!verify_checksum(cart)) {
        printf("WARNING: ROM checksum failed - ROM may be corrupted!\n");
    } else {
        printf("ROM checksum OK\n");
    }

    // Initialize CPU and MMU
    CPU cpu;
    cpu_init(&cpu);
    cpu_print_registers(&cpu);

    free_cartridge(cart);
    exit(EXIT_SUCCESS);
}

