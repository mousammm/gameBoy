#include <stdio.h>
#include <stdlib.h>
#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
    	fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
    	exit(EXIT_FAILURE);
    }

    Cartridge *cart = load_rom(argv[1]);
    // TODO: !cart

    print_rom_info(cart);

    if(!verify_checksum(cart)) {
        printf("WARNING: ROM checksum failed - ROM may be corrupted!\n");
    } else {
        printf("ROM checksum OK\n");
    }

    // Initialize CPU and MMU
    CPU cpu;
    MMU mmu;
    cpu_init(&cpu);
    mmu_init(&mmu, cart);

    // print initial state register
    cpu_print_registers(&cpu);

    // Simple test: read first few bytes of ROM
    printf("\nFirst 16 bytes of ROM:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", mmu_read_byte(&mmu, 0x0100 + i));
    }
    printf("\n");

    // Simple test: execute a few instructions
    printf("\nExecuting first few instructions...\n");
    for (int i = 0; i < 10; i++) {
        // Read opcode at PC
        uint8_t opcode = mmu_read_byte(&mmu, cpu.pc);
        printf("PC=0x%04X: Opcode=0x%02X\n", cpu.pc, opcode);
        
        // TODO: Execute instruction
        // For now, just increment PC
        cpu.pc++;
    }

    // Cleanup
    free_cartridge(cart);

    printf("\nEmulator initialized successfully!\n");
    exit(EXIT_SUCCESS);
}

