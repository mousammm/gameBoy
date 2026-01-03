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
    mmu_init(mmu, cart);

    // Init cpu
    CPU* cpu = cpu_create();
    cpu_init(cpu, mmu);

    printf("\nExecuting first 10 instructions:\n");
    for (int i = 0; i < 10; i++) {
        cpu_step(cpu,mmu);
    }

    
    free(cpu);
    free(mmu);
    cartridge_free(cart);
    return 0;
}
