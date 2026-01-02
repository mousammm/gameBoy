#ifndef MMU_H
#define MMU_H

#include "cartridge.h"

// Forward declaration for circular dependency
typedef struct CPU CPU;

// Memory Bank Controller interface
typedef struct MBC {
    uint8_t type;
    uint8_t rom_bank;
    uint8_t ram_bank;
    bool ram_enabled;
    bool banking_mode;
    bool battery;
    bool has_rtc;
    bool has_rumble;
    
    // RTC registers (if applicable)
    uint8_t rtc_registers[5];
    bool rtc_latched;
    
    // Bank counts
    uint32_t rom_bank_count;
    uint32_t ram_bank_count;
} MBC;

typedef struct MMU {
    // Memory regions
    uint8_t wram[0x8000];       // 32KB Work RAM (8KB banks)
    uint8_t vram[0x2000];       // 8KB Video RAM
    uint8_t hram[0x80];         // 128B High RAM
    uint8_t oam[0xA0];          // 160B Object Attribute Memory
    
    // I/O Registers
    uint8_t io[0x80];           // I/O registers (0xFF00-0xFF7F)
    
    // External components
    Cartridge* cart;            // Cartridge ROM + RAM
    MBC mbc;                    // Memory Bank Controller
    
    // Boot ROM
    uint8_t bootrom[0x100];
    bool bootrom_enabled;
    
    // DMA transfer
    bool dma_active;
    uint8_t dma_source;
    uint8_t dma_cycles;
    
    // Linked components (for callbacks)
    CPU* cpu;
    void* ppu;  // Will be PPU* when implemented
    void* apu;  // Will be APU* when implemented
} MMU;

// Public interface
MMU* mmu_create(CPU* cpu);
void mmu_init(MMU* mmu, Cartridge* cart);
void mmu_free(MMU* mmu);

// Memory access
uint8_t mmu_read(MMU* mmu, uint16_t address);
void mmu_write(MMU* mmu, uint16_t address, uint8_t value);
uint16_t mmu_read16(MMU* mmu, uint16_t address);
void mmu_write16(MMU* mmu, uint16_t address, uint16_t value);

// DMA
void mmu_start_dma(MMU* mmu, uint8_t source);
void mmu_step_dma(MMU* mmu);

// Boot ROM
void mmu_load_bootrom(MMU* mmu, const char* path);
void mmu_disable_bootrom(MMU* mmu);

#endif
