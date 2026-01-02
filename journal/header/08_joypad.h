#ifndef JOYPAD_H
#define JOYPAD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Joypad {
    // Button states
    union {
        struct {
            bool a : 1;
            bool b : 1;
            bool select : 1;
            bool start : 1;
            bool right : 1;
            bool left : 1;
            bool up : 1;
            bool down : 1;
        };
        uint8_t state;
    };
    
    // Register
    uint8_t joyp;  // Joypad register (0xFF00)
    
    // Interrupt
    bool interrupt_requested;
    
    // Linked components
    void* cpu;  // For interrupt requests
} Joypad;

// Public interface
Joypad* joypad_create(void);
void joypad_init(Joypad* joypad, void* cpu_ptr);
void joypad_free(Joypad* joypad);

// Input handling
void joypad_set_button(Joypad* joypad, uint8_t button, bool pressed);
void joypad_update(Joypad* joypad);

// Register access
uint8_t joypad_read(Joypad* joypad);
void joypad_write(Joypad* joypad, uint8_t value);

#endif
