#pragma once

#include "common.h"

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sc;
} cpu_register;

typedef struct {
    cpu_register regs;
 
    // current fetch
    u16 fetch_data;
    u16 mem_dest;
    u8 cur_opcode;

    bool halted;
    bool stepping;

} cpu_context;

void cpu_init();
bool cpu_step();
