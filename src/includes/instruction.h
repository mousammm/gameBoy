/*
 * instruction.h
 */

#pragma once

#include <stdint.h>
#include "cpu.h"
#include "mmu.h"

// Instruction timing (in CPU cycles)
#define INSTR_CYCLES 0
#define CB_CYCLES 1

// Function pointer type for instructions
typedef int (*instruction_func)(CPU* cpu, MMU* mmu);

// Instruction table
typedef struct {
    instruction_func func;    // Function to execute
    const char* mnemonic;     // Human-readable name
    uint8_t length;           // Instruction length in bytes
    uint8_t cycles;           // Base cycles
    uint8_t alt_cycles;       // Alternative cycles (for branches)
} Instruction;

// Opcode tables (for base 0x00-0xFF and CB-prefixed)
extern Instruction instruction_table[256];
extern Instruction cb_instruction_table[256];

// Helper functions for instruction implementation
uint8_t read_byte_operand(CPU* cpu, MMU* mmu);
uint16_t read_word_operand(CPU* cpu, MMU* mmu);
void push(CPU* cpu, MMU* mmu, uint16_t value);
uint16_t pop(CPU* cpu, MMU* mmu);

// Flag setting helpers
void set_flags_z(CPU* cpu, uint8_t value);
void set_flags_znhc(CPU* cpu, uint8_t value, uint8_t prev_value, uint8_t mask);
void set_flags_add(CPU* cpu, uint16_t result, uint16_t a, uint16_t b, uint8_t carry);
void set_flags_sub(CPU* cpu, uint16_t result, uint16_t a, uint16_t b, uint8_t carry);

// Main execution function
int execute_instruction(CPU* cpu, MMU* mmu);
