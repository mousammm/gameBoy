```txt
GameBoy (Top-level)
├── CPU (Central Processing Unit)
├── MMU (Memory Management Unit)
│   ├── Cartridge (ROM + MBC)
│   ├── WRAM (Work RAM)
│   ├── VRAM (Video RAM)
│   ├── HRAM (High RAM)
│   └── I/O Registers
├── PPU (Pixel Processing Unit)
│   ├── LCD Controller
│   ├── Sprite Renderer
│   ├── Background Renderer
│   └── Pixel FIFO
├── APU (Audio Processing Unit)
│   ├── Channel 1 (Square with sweep)
│   ├── Channel 2 (Square)
│   ├── Channel 3 (Wave)
│   ├── Channel 4 (Noise)
│   └── Mixer
├── Timer
├── Interrupt Controller
└── Joypad

```
