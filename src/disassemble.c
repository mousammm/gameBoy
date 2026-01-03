#include <stdio.h>
#include <stdint.h>
#include "disassemble.h"

void d_asm_opcode(uint8_t opcode){
    switch(opcode) {
        case 0x00: printf("NOP"); break;
        case 0x01: printf("LD BC, nn"); break;
        case 0x06: printf("LD B, n"); break;
        case 0x0E: printf("LD C, n"); break;
        case 0x11: printf("LD DE, nn"); break;
        case 0x16: printf("LD D, n"); break;
        case 0x1E: printf("LD E, n"); break;
        case 0x21: printf("LD HL, nn"); break;
        case 0x26: printf("LD H, n"); break;
        case 0x2E: printf("LD L, n"); break;
        case 0x31: printf("LD SP, nn"); break;
        case 0x3E: printf("LD A, n"); break;
        case 0xC3: printf("JP nn"); break;
        case 0x18: printf("JR n"); break;
        case 0xCD: printf("CALL nn"); break;
        case 0xC9: printf("RET"); break;
        case 0xF5: printf("PUSH AF"); break;
        case 0xF1: printf("POP AF"); break;
        case 0x3C: printf("INC A"); break;
        case 0x05: printf("DEC B"); break;
        case 0x80: printf("ADD A, B"); break;
        case 0xFE: printf("CP n"); break;
        case 0x07: printf("RLCA"); break;
        case 0x76: printf("HALT"); break;
        case 0x10: printf("STOP"); break;
        default: printf("???"); break;
    }
}
