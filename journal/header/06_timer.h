#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct Timer {
    // Registers
    uint8_t div;    // Divider (incremented at 16384Hz)
    uint8_t tima;   // Timer counter
    uint8_t tma;    // Timer modulo
    uint8_t tac;    // Timer control
    
    // Internal state
    int div_cycles;
    int tima_cycles;
    bool overflow;
    
    // Linked components
    void* cpu;  // For interrupt requests
} Timer;

// Public interface
Timer* timer_create(void);
void timer_init(Timer* timer, void* cpu_ptr);
void timer_free(Timer* timer);

// Execution
void timer_step(Timer* timer, int cycles);

// Register access
void timer_write_div(Timer* timer, uint8_t value);
void timer_write_tima(Timer* timer, uint8_t value);
void timer_write_tma(Timer* timer, uint8_t value);
void timer_write_tac(Timer* timer, uint8_t value);

#endif
