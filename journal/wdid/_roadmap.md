# **Complete Game Boy Emulator Development Guide**

## **Phase 1: Foundation (Week 1-2)**

### **Step 1: Project Structure**
```
gameboy_emu/
├── src/
│   ├── main.c
│   ├── cartridge.c/h
│   ├── mmu.c/h
│   ├── mbc.c/h
│   ├── cpu.c/h
│   ├── timer.c/h
│   ├── ppu.c/h
│   ├── apu.c/h
│   └── joypad.c/h
├── roms/
├── Makefile
└── README.md
```

### **Step 2: Cartridge System**
```c
// 1. Load ROM file
FILE* f = fopen("game.gb", "rb");
fseek(f, 0, SEEK_END);
size_t size = ftell(f);
rewind(f);

// 2. Read into memory
uint8_t* rom_data = malloc(size);
fread(rom_data, 1, size, f);
fclose(f);

// 3. Parse header (0x100-0x14F)
typedef struct {
    uint8_t* data;
    size_t size;
    uint8_t cartridge_type;  // 0x147
    uint8_t rom_size;        // 0x148
    uint8_t ram_size;        // 0x149
    // ... other header fields
} Cartridge;
```

### **Step 3: Memory Management Unit (MMU)**
```c
// 64KB memory space
uint8_t memory[0x10000];

// Memory map:
// 0x0000-0x7FFF: ROM (handled by MBC)
// 0x8000-0x9FFF: VRAM (graphics)
// 0xA000-0xBFFF: External RAM (cartridge saves)
// 0xC000-0xDFFF: WRAM (work RAM)
// 0xE000-0xFDFF: Echo RAM
// 0xFE00-0xFE9F: OAM (sprites)
// 0xFF00-0xFF7F: I/O Registers
// 0xFF80-0xFFFE: HRAM
// 0xFFFF: Interrupt Enable
```

### **Step 4: CPU Implementation**
```c
// Registers
uint8_t a, f, b, c, d, e, h, l;
uint16_t pc, sp;

// Instruction cycle:
int cpu_step() {
    // 1. Fetch
    uint8_t opcode = mmu_read(pc++);
    
    // 2. Decode & Execute
    switch(opcode) {
        case 0x00: return 4;  // NOP
        case 0x3E: // LD A, n
            a = mmu_read(pc++);
            return 8;
        // ... implement 50-60 instructions
    }
}
```

## **Phase 2: Core Components (Week 3-4)**

### **Step 5: Timer System**
```c
// Timer registers
uint8_t div;   // 0xFF04 - Divider (16384 Hz)
uint8_t tima;  // 0xFF05 - Timer counter
uint8_t tma;   // 0xFF06 - Timer modulo
uint8_t tac;   // 0xFF07 - Timer control

void timer_step(int cycles) {
    // Update DIV every 256 cycles
    div_cycles += cycles;
    if (div_cycles >= 256) {
        div++;
        div_cycles -= 256;
    }
    
    // Update TIMA if enabled
    if (tac & 0x04) {
        // Based on TAC frequency
        tima_cycles += cycles;
        if (tima_cycles >= frequency) {
            tima++;
            if (tima == 0) {
                tima = tma;  // Reload
                request_interrupt(TIMER_INT);
            }
        }
    }
}
```

### **Step 6: Interrupt System**
```c
// Interrupt flags
#define VBLANK_INT   0x01
#define LCD_STAT_INT 0x02
#define TIMER_INT    0x04
#define SERIAL_INT   0x08
#define JOYPAD_INT   0x10

uint8_t ie = 0;  // 0xFFFF - Interrupt Enable
uint8_t if = 0;  // 0xFF0F - Interrupt Flag

void handle_interrupts() {
    if (cpu.ime) {
        uint8_t fired = ie & if;
        if (fired) {
            // Find highest priority interrupt
            if (fired & VBLANK_INT) {
                service_interrupt(VBLANK_INT, 0x40);
            }
            // ... others
        }
    }
}
```

### **Step 7: Graphics (PPU) - Basic Version**
```c
// 160x144 screen
uint32_t framebuffer[160 * 144];

void ppu_step(int cycles) {
    // PPU has 4 modes per scanline:
    // Mode 2: OAM Search (80 cycles)
    // Mode 3: Pixel Transfer (172-289 cycles)
    // Mode 0: HBlank
    // Mode 1: VBlank (lines 144-153)
    
    dot_counter += cycles;
    
    switch(mode) {
        case 2: // OAM Search
            if (dot_counter >= 80) {
                mode = 3;
                start_pixel_transfer();
            }
            break;
        // ... other modes
    }
    
    // Draw pixels to framebuffer
}
```

## **Phase 3: Getting Games Running (Week 5-6)**

### **Step 8: Input Handling**
```c
// Joypad register (0xFF00)
typedef struct {
    uint8_t state;  // bits: Down, Up, Left, Right, Start, Select, B, A
    uint8_t select; // 0x20 for buttons, 0x10 for directions
} Joypad;

uint8_t joypad_read() {
    uint8_t value = 0x0F;
    
    if (joypad.select & 0x20) {
        // Buttons
        if (!(joypad.state & 0x80)) value &= ~0x01; // A
        if (!(joypad.state & 0x40)) value &= ~0x02; // B
        if (!(joypad.state & 0x20)) value &= ~0x04; // Select
        if (!(joypad.state & 0x10)) value &= ~0x08; // Start
    }
    
    if (joypad.select & 0x10) {
        // Directions
        if (!(joypad.state & 0x08)) value &= ~0x01; // Right
        if (!(joypad.state & 0x04)) value &= ~0x02; // Left
        if (!(joypad.state & 0x02)) value &= ~0x04; // Up
        if (!(joypad.state & 0x01)) value &= ~0x08; // Down
    }
    
    return value | 0xC0;  // Bits 6-7 always 1
}
```

### **Step 9: MBC Implementations**

**A. MBC None (Type 0x00):**
```c
uint8_t mbc_none_read(uint16_t addr) {
    return cart->data[addr];  // Direct mapping
}
```

**B. MBC1 (Types 0x01-0x03):**
```c
void mbc1_write(uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        ram_enabled = (value & 0x0F) == 0x0A;
    } else if (addr < 0x4000) {
        rom_bank = value & 0x1F;
        if (rom_bank == 0) rom_bank = 1;
    }
    // ... more banking
}
```

**C. MBC3 (Types 0x0F-0x13):** - Adds Real Time Clock
**D. MBC5 (Types 0x19-0x1E):** - Supports 8MB ROM, Rumble

### **Step 10: Audio (APU) - Basic**
```c
// 4 sound channels
typedef struct {
    SquareChannel ch1, ch2;
    WaveChannel ch3;
    NoiseChannel ch4;
} APU;

void apu_step(int cycles) {
    // Update each channel
    // Generate samples at 44100 Hz
}
```

## **Phase 4: Polish & Optimization (Week 7-8)**

### **Step 11: SDL Display**
```c
// Initialize SDL
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
SDL_Window* window = SDL_CreateWindow("GB Emu", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    160*4, 144*4, 0);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
SDL_Texture* texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
    160, 144);

// Main loop
while (running) {
    // Update texture with framebuffer
    SDL_UpdateTexture(texture, NULL, framebuffer, 160*4);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN) handle_key(event.key.keysym.sym);
    }
}
```

### **Step 12: Save States**
```c
typedef struct {
    CPUState cpu;
    uint8_t memory[0x10000];
    uint8_t cart_ram[RAM_SIZE];
    // ... other state
} SaveState;

void save_state(const char* filename) {
    FILE* f = fopen(filename, "wb");
    fwrite(&state, sizeof(SaveState), 1, f);
    fclose(f);
}
```

### **Step 13: Debugging Tools**
```c
// Disassembler
void disassemble(uint16_t address) {
    uint8_t opcode = mmu_read(address);
    printf("0x%04X: ", address);
    
    switch(opcode) {
        case 0x00: printf("NOP"); break;
        case 0x01: printf("LD BC, 0x%04X", read16(address+1)); break;
        // ... all opcodes
    }
}

// Memory viewer
void view_memory(uint16_t start, uint16_t end) {
    for (uint16_t addr = start; addr <= end; addr += 16) {
        printf("0x%04X: ", addr);
        for (int i = 0; i < 16; i++) {
            printf("%02X ", mmu_read(addr + i));
        }
        printf("\n");
    }
}
```

## **Testing Strategy:**

### **Week 1-2: CPU Tests**
```bash
# Blargg's test ROMs
./emu roms/cpu_instrs.gb
./emu roms/instr_timing.gb
```

### **Week 3-4: Timer/Interrupt Tests**
```bash
./emu roms/timer_test.gb
```

### **Week 5-6: Graphics Tests**
```bash
./emu roms/dmg_sound.gb  # Actually tests graphics too
```

### **Week 7-8: Full Games**
```bash
# Start with simple games
./emu roms/tetris.gb
./emu roms/drmario.gb

# Then more complex
./emu roms/pokemon_red.gb
```

## **Common Problems & Solutions:**

### **1. Game Freezes at Boot**
- Check interrupt handling
- Verify timer is running
- Ensure CPU registers initialized correctly

### **2. Graphics Glitches**
- PPU timing incorrect
- VRAM/OAM access restrictions during modes
- Palette colors wrong

### **3. No Sound**
- APU not enabled (bit 7 of NR52)
- Channels not configured properly
- SDL audio initialization failed

### **4. Save Games Not Working**
- MBC RAM not enabled/disabled properly
- Battery save file not created/loaded
- RAM size incorrect for cartridge

## **Optimization Tips:**

### **1. Fast CPU Dispatch**
```c
// Use function pointer table instead of switch
typedef int (*InstructionFunc)(CPU*);
InstructionFunc instruction_table[256];

// Initialize
instruction_table[0x00] = opcode_00;
instruction_table[0x01] = opcode_01;
// ...

// Execution
int cycles = instruction_table[opcode](cpu);
```

### **2. Cached Memory Access**
```c
// Cache ROM pointer for faster access
uint8_t* rom_bank0 = &cart->data[0];
uint8_t* rom_bankN = &cart->data[current_bank * 0x4000];
```

### **3. PPU Optimization**
```c
// Pre-calculate tile colors
uint32_t palette_cache[4][4];  // [palette][color] -> ARGB

// Render scanline in batches
for (int x = 0; x < 160; x += 8) {
    render_tile_batch(x);
}
```

## **Final Architecture:**
```
Main Loop
    ├── CPU: Execute instruction
    ├── Timer: Update with cycles
    ├── PPU: Draw pixels (every 4 cycles)
    ├── APU: Generate audio samples
    ├── Handle interrupts
    └── Check input
```

## **Essential ROMs for Testing:**
1. **`cpu_instrs.gb`** - CPU accuracy
2. **`instr_timing.gb`** - Timing accuracy  
3. **`dmg_sound.gb`** - Graphics/APU
4. **`tetris.gb`** - Simple working game
5. **`pokemon_red.gb`** - Complex MBC1+RAM+Battery

## **You're Ready! Build It Step-by-Step:**

1. **Day 1-3:** Cartridge + MMU + Basic CPU (NOP, LD, JP)
2. **Day 4-7:** Complete CPU (50+ instructions)
3. **Day 8-10:** Timer + Interrupts
4. **Day 11-14:** Basic PPU (display something)
5. **Day 15-21:** MBC1 + Input
6. **Day 22-28:** Audio + Polish

**Start with Tetris (type 0x00) - get it running, then expand!**
