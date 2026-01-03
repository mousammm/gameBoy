#pragma once

#include "mmu.h"
#include <stdint.h>

typedef struct CPU{
    // Registers
    uint8_t a, f;  // Accumulator & Flags
    uint8_t b, c;
    uint8_t d, e;
    uint8_t h, l;
    
    uint16_t pc;  // Program Counter
    uint16_t sp;  // Stack Pointer

    // Current MMU for memory access
    MMU* mmu;
} CPU;

CPU* cpu_create(void);
void cpu_init(CPU* cpu, MMU* mmu);

// Execute one instruction
int cpu_step(CPU* cpu, MMU* mmu);
