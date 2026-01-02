#ifndef APU_H
#define APU_H

#include <stdint.h>
#include <stdbool.h>

// Channel structures
typedef struct {
    bool enabled;
    uint16_t frequency;
    uint8_t volume;
    uint8_t duty_cycle;
    bool length_enabled;
    uint8_t length_counter;
    // ... more channel-specific fields
} SquareChannel;

typedef struct {
    bool enabled;
    uint16_t frequency;
    uint8_t volume;
    uint8_t wave_pattern[16];
    bool length_enabled;
    uint8_t length_counter;
} WaveChannel;

typedef struct {
    bool enabled;
    uint16_t frequency;
    uint8_t volume;
    uint8_t shift_clock;
    uint8_t dividing_ratio;
    bool counter_width;
    bool length_enabled;
    uint8_t length_counter;
} NoiseChannel;

typedef struct APU {
    // Audio channels
    SquareChannel ch1;
    SquareChannel ch2;
    WaveChannel ch3;
    NoiseChannel ch4;
    
    // Control registers
    uint8_t nr50;  // Channel control / volume
    uint8_t nr51;  // Selection of sound output terminal
    uint8_t nr52;  // Sound on/off
    
    // Frame sequencer
    uint8_t frame_sequencer;
    int frame_cycles;
    
    // Audio output
    float sample_buffer[735 * 2];  // 44100Hz / 60fps = 735 samples per frame
    uint32_t sample_index;
    
    // Linked components
    void* mmu;  // For register access
} APU;

// Public interface
APU* apu_create(void);
void apu_init(APU* apu, void* mmu_ptr);
void apu_free(APU* apu);

// Execution
void apu_step(APU* apu, int cycles);
void apu_generate_samples(APU* apu, float* buffer, uint32_t samples);

// Register access
void apu_write(APU* apu, uint16_t address, uint8_t value);
uint8_t apu_read(APU* apu, uint16_t address);

#endif
