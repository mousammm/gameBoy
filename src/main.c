#include <stdio.h>
#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage <path/to/rom>\n");
        return 0;
    } 

    Cartridge* cart = load_rom(argv[1]);
    // TODO: !cart
    
    // Init memory managment unit
    MMU* mmu = mmu_create();
    if (mmu_init(mmu, cart) == -1) return 0;

    // Init cpu
    CPU* cpu = cpu_create();
    cpu_init(cpu, mmu);

    // execute 1st 10 ins
    for (int i = 0; i < 32768; i++) {
        cpu_step(cpu);
    }

    free(cpu);
    free(mmu);
    cartridge_free(cart);
    return 0;
}
