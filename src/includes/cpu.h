/* 
 * cpu.h
 * https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
 * SHARP LR35902, a "hybrid" between Intel 8080 and Zilog Z80
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

// CPU flags
typedef enum {
    FLAG_Z = 1 << 7,    // Zero flag
    FLAG_N = 1 << 6,    // Subtract flag
    FLAG_H = 1 << 5,    // Half carry flag
    FLAG_C = 1 << 4,    // Carry flag
} CPU_FLAGS;

// CPU register 
typedef struct {
    // 8-bit register 
    union {
        struct {
            uint8_t f;  // Flags
            uint8_t a;  // Accumulator
        };
        uint16_t af;    // combined AF register
    };

    union {
        struct {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };

    union {
        struct {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };

    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };
    
    // 16-bit register
    uint16_t sp;    // stack pointer
    uint16_t pc;    // Program counter

    // CPU state 
    bool ime;       // Interrupt Master enabled
    bool halted;    // HALT state
    bool stopped;   // STOP state

    // Timing
    int cycles;     // Cycles exexuted in current instuction
} CPU;

// Functions declaration
void cpu_init(CPU* cpu);
void cpu_reset(CPU* cpu);
uint8_t cpu_get_flag(CPU* cpu, CPU_FLAGS flag);
void cpu_set_flag(CPU* cpu, CPU_FLAGS flag, bool value);
void cpu_print_registers(CPU* cpu);
