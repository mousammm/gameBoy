#include <stddef.h>
#include <stdio.h>

#include "cart.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage <path/to/rom>\n");
        return 0;
    } 

    load_rom(argv[1]);

    return 0;
}
