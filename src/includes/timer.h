#pragma once

#include <stdint.h>
#include "cpu.h"

typedef struct {
    // Registers
    uint8_t div;        // Divider (0xFF04)
    uint8_t tima;       // Timer counter (0xFF05)
    uint8_t tma;        // Timer module (0xFF06)
    uint8_t tac;        // Timer contol (0xFF07)

    // Internel counters
    int div_cycles;     // Cycles since last DIV increment
    int tima_cycles;    // Cycles since last TIMA increment

    // For interrupt requesting
    CPU* cpu;
} Timer;

Timer* timer_create(void);
void timer_init(Timer* timer, CPU* cpu_ptr);
void timer_free(Timer* timer);

// Execution
void timer_step(Timer* timer, int cycles);

// Register access
void timer_write_div(Timer* timer, uint8_t value);
void timer_write_tima(Timer* timer, uint8_t value);
void timer_write_tma(Timer* timer, uint8_t value);
void timer_write_tac(Timer* timer, uint8_t value);

uint8_t timer_read_div(Timer* timer);
uint8_t timer_read_tima(Timer* timer);
uint8_t timer_read_tma(Timer* timer);
uint8_t timer_read_tac(Timer* timer);
