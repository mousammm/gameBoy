#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "cpu.h"

Timer* timer_create(void) {
    Timer* timer = malloc(sizeof(Timer));
    memset(timer, 0, sizeof(Timer));
    return timer;
}

void timer_init(Timer* timer, CPU* cpu_ptr) {
    // timer->cpu = cpu_ptr;
    
    // Initial values
    timer->div = 0x00;
    timer->tima = 0x00;
    timer->tma = 0x00;
    timer->tac = 0x00;
    
    timer->div_cycles = 0;
    timer->tima_cycles = 0;
}

void timer_free(Timer* timer) {
    free(timer);
}

void timer_step(Timer* timer, int cycles) {
    // Update DIV (always increments at 16384 Hz)
    timer->div_cycles += cycles;
    while (timer->div_cycles >= 256) {  // 4.194304 MHz / 16384 Hz = 256
        timer->div++;
        timer->div_cycles -= 256;
    }
    
    // Update TIMA if enabled
    if (timer->tac & 0x04) {  // Timer enabled (bit 2)
        // Get TIMA frequency based on TAC bits 1-0
        int tima_cycles_per_increment;
        switch(timer->tac & 0x03) {
            case 0x00: tima_cycles_per_increment = 1024; break; // 4096 Hz
            case 0x01: tima_cycles_per_increment = 16;   break; // 262144 Hz
            case 0x02: tima_cycles_per_increment = 64;   break; // 65536 Hz
            case 0x03: tima_cycles_per_increment = 256;  break; // 16384 Hz
            default: tima_cycles_per_increment = 1024; break;
        }
        
        timer->tima_cycles += cycles;
        while (timer->tima_cycles >= tima_cycles_per_increment) {
            timer->tima++;
            
            // Check for overflow
            if (timer->tima == 0x00) {
                // TIMA overflowed!
                timer->tima = timer->tma;  // Reload with TMA
                
                // Request timer interrupt
                if (timer->cpu) {
                    // We need to set bit 2 in IF register (0xFF0F)
                    // cpu_request_interrupt(timer->cpu, 0x04);
                }
            }
            
            timer->tima_cycles -= tima_cycles_per_increment;
        }
    }
}

// Special write handlers
void timer_write_div(Timer* timer, uint8_t value) {
    // Writing ANY value to DIV resets it to 0
    timer->div = 0;
    timer->div_cycles = 0;
}

void timer_write_tima(Timer* timer, uint8_t value) {
    timer->tima = value;
}

void timer_write_tma(Timer* timer, uint8_t value) {
    timer->tma = value;
}

void timer_write_tac(Timer* timer, uint8_t value) {
    timer->tac = value & 0x07;  // Only bits 0-2 are used
}
