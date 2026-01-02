#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "timer.h"
#include "apu.h"
#include "joypad.h"

typedef struct {
    // Core Components
    CPU cpu;
    MMU mmu;
    PPU ppu;
    APU apu;
    Timer timer;
    Joypad joypad;
    
    // System State
    bool running;
    bool paused;
    uint64_t total_cycles;
    
    // Configuration
    bool use_bootrom;
    bool debug_mode;
    
    // Performance
    uint32_t frame_count;
    uint64_t last_time;
    double fps;
} GameBoy;

// Lifecycle
GameBoy* gameboy_create(void);
void gameboy_init(GameBoy* gb, const char* rom_path, const char* bootrom_path);
void gameboy_free(GameBoy* gb);

// Execution
void gameboy_run_frame(GameBoy* gb);
void gameboy_step(GameBoy* gb);  // Single instruction
void gameboy_reset(GameBoy* gb);

// State Management
void gameboy_save_state(GameBoy* gb, const char* path);
void gameboy_load_state(GameBoy* gb, const char* path);

#endif
