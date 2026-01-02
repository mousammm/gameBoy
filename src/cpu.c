// cpu.c - MINIMAL IMPLEMENTATION
#include "cpu.h"
#include "mmu.h"
#include <stdio.h>
#include <string.h>

CPU* cpu_create(void) {
    CPU* cpu = malloc(sizeof(CPU));
    memset(cpu, 0, sizeof(CPU));
    return cpu;
}

void cpu_init(CPU* cpu, MMU* mmu) {
    cpu->mmu = mmu;
    
    // Set initial PC to cartridge entry point (0x0100)
    cpu->pc = 0x0100;
    
    // Initial stack pointer
    cpu->sp = 0xFFFE;
    
    // Initial register values (typical after boot)
    cpu->a = 0x01;
    cpu->f = 0xB0;
    cpu->b = 0x00;
    cpu->c = 0x13;
    cpu->d = 0x00;
    cpu->e = 0xD8;
    cpu->h = 0x01;
    cpu->l = 0x4D;
}

int cpu_step(CPU* cpu) {
    // Fetch instruction at PC
    uint8_t opcode = mmu_read(cpu->mmu, cpu->pc);
    cpu->pc++;  // Move to next byte
    
    // Decode and execute (start with just NOP)
    switch(opcode) {
        case 0x00:  // NOP - Do nothing
            printf("NOP at PC: 0x%04X\n", cpu->pc - 1);
            return 4;  // NOP takes 4 cycles
            
        case 0x3E:  // LD A, n - Load immediate into A
        {
            uint8_t value = mmu_read(cpu->mmu, cpu->pc);
            cpu->pc++;
            cpu->a = value;
            printf("LD A, 0x%02X at PC: 0x%04X\n", value, cpu->pc - 2);
            return 8;  // 8 cycles
        }
        
        case 0xC3:  // JP nn - Jump to address
        {
            uint8_t low = mmu_read(cpu->mmu, cpu->pc);
            cpu->pc++;
            uint8_t high = mmu_read(cpu->mmu, cpu->pc);
            cpu->pc++;
            uint16_t address = (high << 8) | low;
            cpu->pc = address;
            printf("JP 0x%04X at PC: 0x%04X\n", address, cpu->pc);
            return 16;  // 16 cycles
        }
        
        default:
            printf("Unknown opcode: 0x%02X at PC: 0x%04X\n", 
                   opcode, cpu->pc - 1);
            return 4;
    }
}
