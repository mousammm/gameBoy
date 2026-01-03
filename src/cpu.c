// cpu.c - MINIMAL IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "mmu.h"

CPU* cpu_create(void) {
    CPU* cpu = malloc(sizeof(CPU));
    memset(cpu, 0, sizeof(CPU));
    return cpu;
}

void cpu_init(CPU* cpu, MMU* mmu) {
    cpu->mmu = mmu;
    
    cpu->pc = 0x0100; // cartridge entry point
    cpu->sp = 0xFFFE; // Initial stack pointer
    
    // Initial register values after boot
    cpu->a = 0x01;
    cpu->f = 0xB0;  // Flags: Z=1, N=0, H=1, C=1
    cpu->b = 0x00;
    cpu->c = 0x13;
    cpu->d = 0x00;
    cpu->e = 0xD8;
    cpu->h = 0x01;
    cpu->l = 0x4D;

    cpu->ime = true;    // Interrupts enabled
    cpu->halted = false;
    cpu->cycles = 0;
}

void cpu_free(CPU *cpu)
{
    free(cpu);
}

// Helper: Read 16-bit value from memory
static uint16_t cpu_read16(CPU* cpu, uint16_t address) {
    uint8_t low = mmu_read(cpu->mmu, address);
    uint8_t high = mmu_read(cpu->mmu, address + 1);
    return (high << 8) | low;
}

int cpu_step(CPU* cpu) {
    // Dont execute if halted
    if (cpu->halted) {
        return 4;   // still consumes cycles
    }

    // Fetch instruction at PC
    uint8_t opcode = mmu_read(cpu->mmu, cpu->pc);
    cpu->pc++;  // Move to next byte
    
    // Decode and execute
    switch(opcode) {
        case 0x00:  // NOP - Do nothing
            return 4;  // NOP takes 4 cycles
            
        case 0x3E:  // LD A, n - Load immediate into A
        {
            cpu->a = mmu_read(cpu->mmu, cpu->pc++);
            return 8;  // 8 cycles
        }
        
        case 0xC3:  // JP nn - Jump to address
        {
            cpu->pc = cpu_read16(cpu, cpu->pc);
            return 16;  // 16 cycles
        }
        
        // === HALT/STOP ===
        case 0x76:  // HALT
            cpu->halted = true;
            return 4;
            
        case 0x10:  // STOP (treated as NOP for now)
            return 4;

        default:
            return 4;
    }
}
