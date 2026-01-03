#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "mmu.h"

typedef struct CPU{
    // Registers
    uint8_t a, f;   // Accumulator & Flags
    uint8_t b, c;
    uint8_t d, e;
    uint8_t h, l;
    
    // 16bit Registers
    uint16_t pc;    // Program Counter
    uint16_t sp;    // Stack Pointer

    // CPU state 
    bool ime;       // Interrupt Master Enable
    bool halted;
    int cycles;     // Total cycles executed

    // Current MMU for memory access
    MMU* mmu;
} CPU;

// Flags
#define FLAG_Z 0x80  // Zero
#define FLAG_N 0x40  // Subtract
#define FLAG_H 0x20  // Half Carry
#define FLAG_C 0x10  // Carry

// CPU functions
CPU* cpu_create(void);
void cpu_init(CPU* cpu, MMU* mmu);
void cpu_free(CPU* cpu);

// Execute one instruction
int cpu_step(CPU* cpu);   // Execute on instruction
void cpu_request_interrupt(CPU* cpu, uint8_t interrupt);

// Flag helpers
static inline bool cpu_get_flag(CPU* cpu, uint8_t flag)
{
    return (cpu->f & flag) != 0;
}

static inline void cpu_set_flag(CPU* cpu, uint8_t flag, bool value) {
    if (value) {
        cpu->f |= flag;
    } else {
        cpu->f &= ~flag;
    }
}

// 16-bit register helpers
static inline uint16_t cpu_get_bc(CPU* cpu) {
    return (cpu->b << 8) | cpu->c;
}

static inline void cpu_set_bc(CPU* cpu, uint16_t value) {
    cpu->b = value >> 8;
    cpu->c = value & 0xFF;
}

// Same for DE, HL
static inline uint16_t cpu_get_de(CPU* cpu) {
    return (cpu->d << 8) | cpu->e;
}

static inline void cpu_set_de(CPU* cpu, uint16_t value) {
    cpu->d = value >> 8;
    cpu->e = value & 0xFF;
}

static inline uint16_t cpu_get_hl(CPU* cpu) {
    return (cpu->h << 8) | cpu->l;
}

static inline void cpu_set_hl(CPU* cpu, uint16_t value) {
    cpu->h = value >> 8;
    cpu->l = value & 0xFF;
}

