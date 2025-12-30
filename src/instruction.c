/*
 * instruction.h
 */
#include "instructions.h"
#include <stdio.h>
#include <string.h>

// ============================================
// HELPER FUNCTIONS
// ============================================

// Read next byte from program counter
uint8_t read_byte_operand(CPU* cpu, MMU* mmu) {
    uint8_t value = mmu_read_byte(mmu, cpu->pc);
    cpu->pc++;
    return value;
}

// Read next word (2 bytes) from program counter
uint16_t read_word_operand(CPU* cpu, MMU* mmu) {
    uint16_t value = mmu_read_word(mmu, cpu->pc);
    cpu->pc += 2;
    return value;
}

// Push value onto stack
void push(CPU* cpu, MMU* mmu, uint16_t value) {
    cpu->sp -= 2;
    mmu_write_word(mmu, cpu->sp, value);
}

// Pop value from stack
uint16_t pop(CPU* cpu, MMU* mmu) {
    uint16_t value = mmu_read_word(mmu, cpu->sp);
    cpu->sp += 2;
    return value;
}

// Set Z flag based on value
void set_flags_z(CPU* cpu, uint8_t value) {
    cpu_set_flag(cpu, FLAG_Z, value == 0);
}

// Set Z/N/H/C flags
void set_flags_znhc(CPU* cpu, uint8_t value, uint8_t prev_value, uint8_t mask) {
    set_flags_z(cpu, value);
    cpu_set_flag(cpu, FLAG_N, (mask & 0x08) != 0);
    cpu_set_flag(cpu, FLAG_H, (mask & 0x04) != 0);
    cpu_set_flag(cpu, FLAG_C, (mask & 0x02) != 0);
}

// Set flags for addition
void set_flags_add(CPU* cpu, uint16_t result, uint16_t a, uint16_t b, uint8_t carry) {
    cpu_set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((a & 0xF) + (b & 0xF) + carry) > 0xF);
    cpu_set_flag(cpu, FLAG_C, result > 0xFF);
}

// Set flags for subtraction
void set_flags_sub(CPU* cpu, uint16_t result, uint16_t a, uint16_t b, uint8_t carry) {
    cpu_set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, (int)(a & 0xF) - (int)(b & 0xF) - carry < 0);
    cpu_set_flag(cpu, FLAG_C, (int)a - (int)b - carry < 0);
}

// ============================================
// BASIC INSTRUCTIONS (NOP, LD, INC, DEC, etc.)
// ============================================

// 0x00: NOP - No operation
int instr_nop(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->cycles = 4;
    return 0;
}

// 0x01: LD BC, d16 - Load 16-bit immediate into BC
int instr_ld_bc_d16(CPU* cpu, MMU* mmu) {
    cpu->bc = read_word_operand(cpu, mmu);
    cpu->cycles = 12;
    return 0;
}

// 0x02: LD (BC), A - Load A into address pointed by BC
int instr_ld_bc_a(CPU* cpu, MMU* mmu) {
    mmu_write_byte(mmu, cpu->bc, cpu->a);
    cpu->cycles = 8;
    return 0;
}

// 0x03: INC BC - Increment BC
int instr_inc_bc(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->bc++;
    cpu->cycles = 8;
    return 0;
}

// 0x04: INC B - Increment B
int instr_inc_b(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t prev = cpu->b;
    cpu->b++;
    
    // Set flags: Z, N, H
    cpu_set_flag(cpu, FLAG_Z, cpu->b == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, (prev & 0x0F) == 0x0F);
    
    cpu->cycles = 4;
    return 0;
}

// 0x05: DEC B - Decrement B
int instr_dec_b(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t prev = cpu->b;
    cpu->b--;
    
    // Set flags: Z, N, H
    cpu_set_flag(cpu, FLAG_Z, cpu->b == 0);
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, (prev & 0x0F) == 0x00);
    
    cpu->cycles = 4;
    return 0;
}

// 0x06: LD B, d8 - Load 8-bit immediate into B
int instr_ld_b_d8(CPU* cpu, MMU* mmu) {
    cpu->b = read_byte_operand(cpu, mmu);
    cpu->cycles = 8;
    return 0;
}

// 0x07: RLCA - Rotate A left (old bit 7 to Carry)
int instr_rlca(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t bit7 = (cpu->a >> 7) & 1;
    cpu->a = (cpu->a << 1) | bit7;
    
    // Set flags: Z=0, N=0, H=0, C=bit7
    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, bit7);
    
    cpu->cycles = 4;
    return 0;
}

// 0x08: LD (a16), SP - Store SP at given address
int instr_ld_a16_sp(CPU* cpu, MMU* mmu) {
    uint16_t addr = read_word_operand(cpu, mmu);
    mmu_write_word(mmu, addr, cpu->sp);
    cpu->cycles = 20;
    return 0;
}

// 0x09: ADD HL, BC - Add BC to HL
int instr_add_hl_bc(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint32_t result = cpu->hl + cpu->bc;
    
    // Set flags: N=0, H, C
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, (cpu->hl & 0xFFF) + (cpu->bc & 0xFFF) > 0xFFF);
    cpu_set_flag(cpu, FLAG_C, result > 0xFFFF);
    
    cpu->hl = result & 0xFFFF;
    cpu->cycles = 8;
    return 0;
}

// 0x0A: LD A, (BC) - Load from address pointed by BC into A
int instr_ld_a_bc(CPU* cpu, MMU* mmu) {
    cpu->a = mmu_read_byte(mmu, cpu->bc);
    cpu->cycles = 8;
    return 0;
}

// 0x0B: DEC BC - Decrement BC
int instr_dec_bc(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->bc--;
    cpu->cycles = 8;
    return 0;
}

// 0x0C: INC C - Increment C
int instr_inc_c(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t prev = cpu->c;
    cpu->c++;
    
    cpu_set_flag(cpu, FLAG_Z, cpu->c == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, (prev & 0x0F) == 0x0F);
    
    cpu->cycles = 4;
    return 0;
}

// 0x0D: DEC C - Decrement C
int instr_dec_c(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t prev = cpu->c;
    cpu->c--;
    
    cpu_set_flag(cpu, FLAG_Z, cpu->c == 0);
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, (prev & 0x0F) == 0x00);
    
    cpu->cycles = 4;
    return 0;
}

// 0x0E: LD C, d8 - Load 8-bit immediate into C
int instr_ld_c_d8(CPU* cpu, MMU* mmu) {
    cpu->c = read_byte_operand(cpu, mmu);
    cpu->cycles = 8;
    return 0;
}

// 0x0F: RRCA - Rotate A right (old bit 0 to Carry)
int instr_rrca(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    uint8_t bit0 = cpu->a & 1;
    cpu->a = (cpu->a >> 1) | (bit0 << 7);
    
    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, bit0);
    
    cpu->cycles = 4;
    return 0;
}

// ============================================
// JUMP AND CALL INSTRUCTIONS
// ============================================

// 0xC3: JP a16 - Jump to address
int instr_jp_a16(CPU* cpu, MMU* mmu) {
    cpu->pc = read_word_operand(cpu, mmu);
    cpu->cycles = 16;
    return 0;
}

// 0xC2: JP NZ, a16 - Jump if Z flag is 0
int instr_jp_nz_a16(CPU* cpu, MMU* mmu) {
    uint16_t addr = read_word_operand(cpu, mmu);
    if (!cpu_get_flag(cpu, FLAG_Z)) {
        cpu->pc = addr;
        cpu->cycles = 16;
    } else {
        cpu->cycles = 12;
    }
    return 0;
}

// 0xCA: JP Z, a16 - Jump if Z flag is 1
int instr_jp_z_a16(CPU* cpu, MMU* mmu) {
    uint16_t addr = read_word_operand(cpu, mmu);
    if (cpu_get_flag(cpu, FLAG_Z)) {
        cpu->pc = addr;
        cpu->cycles = 16;
    } else {
        cpu->cycles = 12;
    }
    return 0;
}

// 0xCD: CALL a16 - Call subroutine
int instr_call_a16(CPU* cpu, MMU* mmu) {
    uint16_t addr = read_word_operand(cpu, mmu);
    push(cpu, mmu, cpu->pc);
    cpu->pc = addr;
    cpu->cycles = 24;
    return 0;
}

// 0xC9: RET - Return from subroutine
int instr_ret(CPU* cpu, MMU* mmu) {
    cpu->pc = pop(cpu, mmu);
    cpu->cycles = 16;
    return 0;
}

// 0xD9: RETI - Return and enable interrupts
int instr_reti(CPU* cpu, MMU* mmu) {
    cpu->pc = pop(cpu, mmu);
    cpu->ime = true;
    cpu->cycles = 16;
    return 0;
}

// ============================================
// INTERRUPT AND MISC INSTRUCTIONS
// ============================================

// 0xF3: DI - Disable interrupts
int instr_di(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->ime = false;
    cpu->cycles = 4;
    return 0;
}

// 0xFB: EI - Enable interrupts
int instr_ei(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->ime = true;
    cpu->cycles = 4;
    return 0;
}

// 0x76: HALT - Halt CPU until interrupt
int instr_halt(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->halted = true;
    cpu->cycles = 4;
    return 0;
}

// 0x10: STOP - Stop CPU and LCD
int instr_stop(CPU* cpu, MMU* mmu) {
    (void)mmu; // Unused
    cpu->stopped = true;
    cpu->cycles = 4;
    
    // Read next byte (should be 0x00)
    read_byte_operand(cpu, mmu);
    
    return 0;
}

// ============================================
// INSTRUCTION TABLE INITIALIZATION
// ============================================

// Initialize instruction table
Instruction instruction_table[256];
Instruction cb_instruction_table[256];

void init_instruction_table() {
    // Initialize all to NULL/unimplemented
    for (int i = 0; i < 256; i++) {
        instruction_table[i].func = NULL;
        instruction_table[i].mnemonic = "UNIMPLEMENTED";
        instruction_table[i].length = 1;
        instruction_table[i].cycles = 4;
        instruction_table[i].alt_cycles = 0;
        
        cb_instruction_table[i].func = NULL;
        cb_instruction_table[i].mnemonic = "CB UNIMPLEMENTED";
        cb_instruction_table[i].length = 2;  // Includes CB prefix
        cb_instruction_table[i].cycles = 8;
        cb_instruction_table[i].alt_cycles = 0;
    }
    
    // Fill in implemented instructions
    instruction_table[0x00] = (Instruction){instr_nop, "NOP", 1, 4, 0};
    instruction_table[0x01] = (Instruction){instr_ld_bc_d16, "LD BC,d16", 3, 12, 0};
    instruction_table[0x02] = (Instruction){instr_ld_bc_a, "LD (BC),A", 1, 8, 0};
    instruction_table[0x03] = (Instruction){instr_inc_bc, "INC BC", 1, 8, 0};
    instruction_table[0x04] = (Instruction){instr_inc_b, "INC B", 1, 4, 0};
    instruction_table[0x05] = (Instruction){instr_dec_b, "DEC B", 1, 4, 0};
    instruction_table[0x06] = (Instruction){instr_ld_b_d8, "LD B,d8", 2, 8, 0};
    instruction_table[0x07] = (Instruction){instr_rlca, "RLCA", 1, 4, 0};
    instruction_table[0x08] = (Instruction){instr_ld_a16_sp, "LD (a16),SP", 3, 20, 0};
    instruction_table[0x09] = (Instruction){instr_add_hl_bc, "ADD HL,BC", 1, 8, 0};
    instruction_table[0x0A] = (Instruction){instr_ld_a_bc, "LD A,(BC)", 1, 8, 0};
    instruction_table[0x0B] = (Instruction){instr_dec_bc, "DEC BC", 1, 8, 0};
    instruction_table[0x0C] = (Instruction){instr_inc_c, "INC C", 1, 4, 0};
    instruction_table[0x0D] = (Instruction){instr_dec_c, "DEC C", 1, 4, 0};
    instruction_table[0x0E] = (Instruction){instr_ld_c_d8, "LD C,d8", 2, 8, 0};
    instruction_table[0x0F] = (Instruction){instr_rrca, "RRCA", 1, 4, 0};
    
    // Jump instructions
    instruction_table[0xC3] = (Instruction){instr_jp_a16, "JP a16", 3, 16, 0};
    instruction_table[0xC2] = (Instruction){instr_jp_nz_a16, "JP NZ,a16", 3, 12, 16};
    instruction_table[0xCA] = (Instruction){instr_jp_z_a16, "JP Z,a16", 3, 12, 16};
    instruction_table[0xCD] = (Instruction){instr_call_a16, "CALL a16", 3, 24, 0};
    instruction_table[0xC9] = (Instruction){instr_ret, "RET", 1, 16, 0};
    instruction_table[0xD9] = (Instruction){instr_reti, "RETI", 1, 16, 0};
    
    // Interrupt instructions
    instruction_table[0xF3] = (Instruction){instr_di, "DI", 1, 4, 0};
    instruction_table[0xFB] = (Instruction){instr_ei, "EI", 1, 4, 0};
    instruction_table[0x76] = (Instruction){instr_halt, "HALT", 1, 4, 0};
    instruction_table[0x10] = (Instruction){instr_stop, "STOP", 2, 4, 0};
    
    // TODO: Add more instructions as they're implemented
}

// ============================================
// MAIN INSTRUCTION EXECUTION
// ============================================

int execute_instruction(CPU* cpu, MMU* mmu) {
    // Read opcode
    uint8_t opcode = mmu_read_byte(mmu, cpu->pc);
    cpu->pc++;
    
    // Check for CB prefix (extended instruction set)
    if (opcode == 0xCB) {
        uint8_t cb_opcode = mmu_read_byte(mmu, cpu->pc);
        cpu->pc++;
        
        Instruction instr = cb_instruction_table[cb_opcode];
        if (instr.func) {
            return instr.func(cpu, mmu);
        } else {
            printf("Unimplemented CB opcode: 0x%02X at PC=0x%04X\n", 
                   cb_opcode, cpu->pc - 2);
            return -1;
        }
    }
    
    // Regular instruction
    Instruction instr = instruction_table[opcode];
    if (instr.func) {
        return instr.func(cpu, mmu);
    } else {
        printf("Unimplemented opcode: 0x%02X at PC=0x%04X\n", 
               opcode, cpu->pc - 1);
        return -1;
    }
}
