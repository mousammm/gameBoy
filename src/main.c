#include <stdio.h>
#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage <path/to/rom>\n");
        return 0;
    } 

    Cartridge* cart = load_rom(argv[1]);
    // TODO: !cart
    
    // Init memory managment unit
    MMU* mmu = mmu_create();
    mmu_init(mmu, cart);

    // Init cpu
    // CPU* cpu = cpu_create();
    // cpu_init(cpu, mmu);
    //
    // printf("\nExecuting first 10 instructions:\n");
    // for (int i = 0; i < 10; i++) {
    //     cpu_step(cpu,mmu);
    // }

    // Read entry point (should be at 0x0100)
    uint8_t byte1 = mmu_read(mmu, 0x0100);
    uint8_t byte2 = mmu_read(mmu, 0x0101);
    uint8_t byte3 = mmu_read(mmu, 0x0102);
    
    printf("Entry point bytes: 0x%02X 0x%02X 0x%02X\n", 
           byte1, byte2, byte3);
    
    // 4. Test writing to ROM area (should be ignored for type 0x00)
    printf("\nTesting ROM write (should be ignored):\n");
    mmu_write(mmu, 0x2000, 0x42);
    uint8_t after_write = mmu_read(mmu, 0x2000);
    printf("After writing 0x42 to 0x2000, read: 0x%02X\n", after_write);
    
    // 5. Test external RAM (should return 0xFF for type 0x00)
    printf("\nTesting external RAM read (should be 0xFF):\n");
    uint8_t ram_read = mmu_read(mmu, 0xA123);
    printf("Read from 0xA123: 0x%02X\n", ram_read);
    
    // 6. Cleanup
    mmu_free(mmu);

    // free(cpu);
    // free(mmu);
    cartridge_free(cart);
    return 0;
}
