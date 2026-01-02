#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    PPU_MODE_HBLANK = 0,
    PPU_MODE_VBLANK = 1,
    PPU_MODE_OAM = 2,
    PPU_MODE_TRANSFER = 3
} PPU_Mode;

typedef struct Sprite {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t flags;
} Sprite;

typedef struct PPU {
    // Display buffer (160x144 pixels, 32-bit color)
    uint32_t framebuffer[160 * 144];
    
    // Internal state
    PPU_Mode mode;
    int mode_cycles;
    uint8_t line;
    uint8_t lyc;            // LY Compare
    
    // Registers (mirrored from MMU I/O)
    uint8_t lcdc;           // LCD Control
    uint8_t stat;           // LCD Status
    uint8_t scy, scx;       // Scroll Y, X
    uint8_t wy, wx;         // Window Y, X
    uint8_t bgp;            // Background palette
    uint8_t obp0, obp1;     // Object palettes
    
    // Internal rendering
    Sprite line_sprites[10];
    uint8_t sprite_count;
    
    // Pixel FIFO
    uint8_t fifo[16];
    uint8_t fifo_size;
    
    // Background FIFO
    uint8_t bg_fifo[16];
    uint8_t bg_fifo_size;
    
    // Linked components
    void* mmu;  // Will be MMU* when implemented
    
    // Debug
    bool draw_background;
    bool draw_window;
    bool draw_sprites;
} PPU;

// Public interface
PPU* ppu_create(void);
void ppu_init(PPU* ppu, void* mmu_ptr);
void ppu_free(PPU* ppu);

// Execution
void ppu_step(PPU* ppu, int cycles);
void ppu_update_framebuffer(PPU* ppu);

// Register access
void ppu_write_lcdc(PPU* ppu, uint8_t value);
void ppu_write_stat(PPU* ppu, uint8_t value);
uint8_t ppu_read_stat(PPU* ppu);

// Pixel processing
void ppu_render_scanline(PPU* ppu);
void ppu_fetch_sprites(PPU* ppu);

#endif
