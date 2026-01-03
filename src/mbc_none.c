#include <stdio.h>
#include "mbc.h"

MBC* mbc_none_create(Cartridge* cart);
uint8_t mbc_none_read_rom(MBC* mbc, uint16_t address);
void mbc_none_write_rom(MBC* mbc, uint16_t address, uint8_t value);
uint8_t mbc_none_read_ram(MBC* mbc, uint16_t address);
void mbc_none_write_ram(MBC* mbc, uint16_t address, uint8_t value);

MBC* mbc_none_create(Cartridge* cart) {
    MBC* mbc = malloc(sizeof(MBC));
    
    // Setup function pointers
    mbc->read_rom = mbc_none_read_rom;
    mbc->write_rom = mbc_none_write_rom;
    mbc->read_ram = mbc_none_read_ram;
    mbc->write_ram = mbc_none_write_ram;
    
    // Store cartridge reference
    mbc->cart = cart;
    mbc->ram_data = NULL;
    mbc->ram_size = 0;
    mbc->type_data = NULL;
    
    return mbc;
}

uint8_t mbc_none_read_rom(MBC* mbc, uint16_t address) {
    // Simple direct mapping
    if (address < mbc->cart->size) {
        return mbc->cart->data[address];
    }
    return 0xFF;
}

void mbc_none_write_rom(MBC* mbc, uint16_t address, uint8_t value) {
    // ROM writes do nothing for type 0x00
    // (Some cartridge types use ROM writes for banking)
    (void)mbc;
    (void)address;
    (void)value;
}

uint8_t mbc_none_read_ram(MBC* mbc, uint16_t address) {
    // No external RAM
    (void)mbc;
    (void)address;
    return 0xFF;
}

void mbc_none_write_ram(MBC* mbc, uint16_t address, uint8_t value) {
    // No external RAM, ignore writes
    (void)mbc;
    (void)address;
    (void)value;
}
