/*
 * cpu.c
 */
#include "cpu.h"
#include <stdio.h>

// Initiliaze CPU with startup values
void cpu_init(CPU* cpu)
{
    if (!cpu) return;

    cpu_reset(cpu);
}

// Reset CPU to boot state 
void cpu_reset(CPU* cpu)
{
    cpu->af = 0x01B0;   // After boot ROM: A=0x01, F=0xB0
    cpu->bc = 0x0013;
    cpu->de = 0x00D8;
    cpu->hl = 0x014D;
    cpu->sp = 0xFFFE;   // Stack grows down from (0xFFFE)
    cpu->pc = 0x0100;   // Start execution at 0x0100 (after boot)

    cpu->ime = false;   // Interrupt disabled at first
    cpu->halted = false;
    cpu->stopped = false;
    cpu->cycles = 0;
}

// Get flag value
uint8_t cpu_get_flag(CPU* cpu, CPU_FLAGS flag)
{
    return (cpu->f & flag) ? 1 : 0;
}

// Set or clear a flag
void cpu_set_flag(CPU* cpu, CPU_FLAGS flag, bool value)
{
    if (value) {
        cpu->f |= flag;
    } else {
        cpu->f &= -flag;
    }
}

// Print CPU state for debugging
void cpu_print_registers(CPU* cpu)
{
    printf("\n=== CPU State ===\n");
    printf("AF: 0x%04X (A: 0x%02X, F: 0x%02X)\n", cpu->af, cpu->a, cpu->f);
    printf("BC: 0x%04X (B: 0x%02X, C: 0x%02X)\n", cpu->bc, cpu->b, cpu->c);
    printf("DE: 0x%04X (D: 0x%02X, E: 0x%02X)\n", cpu->de, cpu->d, cpu->e);
    printf("HL: 0x%04X (H: 0x%02X, L: 0x%02X)\n", cpu->hl, cpu->h, cpu->l);
    printf("SP: 0x%04X\n", cpu->sp);
    printf("PC: 0x%04X\n", cpu->pc);
    
    printf("Flags: %c%c%c%c\n",
        cpu_get_flag(cpu, FLAG_Z) ? 'Z' : '-',
        cpu_get_flag(cpu, FLAG_N) ? 'N' : '-',
        cpu_get_flag(cpu, FLAG_H) ? 'H' : '-',
        cpu_get_flag(cpu, FLAG_C) ? 'C' : '-'
    );
    
    printf("IME: %s\n", cpu->ime ? "Enabled" : "Disabled");
    printf("Halted: %s\n", cpu->halted ? "Yes" : "No");
    printf("Stopped: %s\n", cpu->stopped ? "Yes" : "No");
    printf("Cycles: %d\n", cpu->cycles);
    printf("=================\n");
}
