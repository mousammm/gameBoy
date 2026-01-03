#include <stdio.h>
#include "cartridge.h"
#include "cpu.h"
#include "disassemble.h"
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

    int total_instructions = 100;
    int total_cycles = 0;

    printf("PC OP CYCLES AF:BC:DE:HL\n\n");

    // execute 1st 10 ins
    for (int i = 0; i < total_instructions; i++) {
        uint16_t current_pc = cpu->pc;
        int cycles = cpu_step(cpu);
        total_cycles += cycles;

        // Update timer with cycles that passed
        // timer_step(timer, cycles);

        // get the current opcode that was just executed
        uint8_t opcode = mmu_read(mmu, current_pc);
        printf("$%04X $%02X (%d) (%02X%02X-%02X%02X-%02X%02X-%02X%02X)\t",
                current_pc, opcode, cycles,
                cpu->a, cpu->f, cpu->b, cpu->c,
                cpu->d, cpu->e, cpu->h, cpu->l);

        // disassemble opcode
        d_asm_opcode(opcode);
        printf("\n");

        // Stop if CPU halted
        if (cpu->halted) {
            printf("\nCPU halted at instruction %d\n", i + 1);
            break;
        }
    }

    // Cleanup
    cpu_free(cpu);
    mmu_free(mmu);
    cartridge_free(cart);
    return 0;
}
