#include <stddef.h>
#include <stdio.h>

#include "cart.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage <path/to/rom>\n");
        return 0;
    } 

    Cartridge* cart = load_rom(argv[1]);
    // TODO: !cart

    free_cartridge(cart);
    return 0;
}
