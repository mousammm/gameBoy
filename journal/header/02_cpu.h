#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct CPU {
    // Registers (Little-endian: low byte first in union)
                    // Flags    Accumulator
    union { struct { uint8_t f; uint8_t a; }; uint16_t af; };
    union { struct { uint8_t c; uint8_t b; }; uint16_t bc; };
    union { struct { uint8_t e; uint8_t d; }; uint16_t de; };
    union { struct { uint8_t l; uint8_t h; }; uint16_t hl; };
    
    // Special registers
    uint16_t pc;        // Program Counter
    uint16_t sp;        // Stack Pointer
    
    // CPU State
    bool ime;           // Interrupt Master Enable
    bool ime_scheduled; // IME will be enabled after next instruction
    bool halted;
    bool stopped;
    
    // Cycle counting
    int cycles;
    int last_instruction_cycles;
    
    // Debug
    uint16_t breakpoint;
    bool breakpoint_hit;
} CPU;

void cpu_init(CPU* cpu);
void cpu_reset(CPU* cpu);
int cpu_execute_instruction(CPU* cpu, void* mmu_ptr);
void cpu_request_interrupt(CPU* cpu, uint8_t interrupt_flag);
void cpu_handle_interrupts(CPU* cpu, void* mmu_ptr);

#define CPU_FLAG_Z 0x80  // Zero flag
#define CPU_FLAG_N 0x40  // Subtract flag
#define CPU_FLAG_H 0x20  // Half Carry flag
#define CPU_FLAG_C 0x10  // Carry flag

static inline bool cpu_get_flag(CPU* cpu, uint8_t flag) {
    return (cpu->f & flag) != 0;
}

static inline void cpu_set_flag(CPU* cpu, uint8_t flag, bool value) {
    if (value) cpu->f |= flag;
    else cpu->f &= ~flag;
}

#endif
