#include <cstdint>
#include <fstream>
#include "mbc1.hpp"
#include "../../exceptions.hpp"

// Clear RAM and set registers to their default power-on values.
void Mbc1::reset() {
    BaseMbc::reset();
    is_ram_enabled = false;
    rom_bank_number = 1;
    ram_bank_number = 0;
    advanced_banking = false;
}

/* Return the 8 bit value in the currently selected ROM bank at the given
 * address.
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t Mbc1::read_rom(uint16_t address) const {

    if (address < ROM_BANK_SIZE) {
        uint8_t selected_rom_bank = advanced_banking ? 
            (ram_bank_number << 5) & (rom_size - 1) : 0;
        return rom[selected_rom_bank][address];
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        uint8_t selected_rom_bank =
            ((ram_bank_number << 5) | rom_bank_number) & (rom_size - 1);
        return rom[selected_rom_bank][address & (ROM_BANK_SIZE - 1)];
    }
    
    else {
        throw MemoryAccessException(
            "MBC1 ROM", "out of bounds", address, true
        );
    }
}

/* Modify the registers depending on the region the given address falls in and
 * the given value as follows:
 * - 0x0000-0x1FFF - is_ram_enabled
 * Set true if the trailing 4 bits of value are 0xA, otherwise false.
 * - 0x2000-0x3FFF - rom_bank_number
 * Set to the trailing 5 bits of value. If set to 0 is incremented to 1.
 * - 0x4000-0x5FFF - ram_bank_number
 * Set to the trailing 2 bits of value.
 * - 0x6000-0x7FFF - advanced_banking
 * Set to the trailing bit of value. 
 * Throws a MemoryAccessException if the address is out of bounds. */
void Mbc1::write_rom(uint16_t address, uint8_t value) {

    if (address < IS_RAM_ENABLED_UPPER) {
        is_ram_enabled = ((value & 0xF) == 0xA);
    }

    else if (address < ROM_BANK_SIZE) {
        rom_bank_number = value & 0b11111;
        rom_bank_number = rom_bank_number ? rom_bank_number : 1;
    }

    else if (address < RAM_BANK_NUMBER_UPPER) {
        ram_bank_number = value & 0b11;
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        advanced_banking = value & 1;
    }
    
    else {
        throw MemoryAccessException(
            "MBC1 ROM", "out of bounds", address, false
        );
    }
}

/* Return the 8 bit value in the currently selected RAM bank at the given
 * address.
 * Throws a MemoryAccessException if there are no external RAM banks, if RAM is
 * not accessible or if the address is out of bounds. */
uint8_t Mbc1::read_ram(uint16_t address) const {

    if (!ram_size) {
        throw MemoryAccessException(
            "MBC1 RAM", "there are no external RAM banks", address, true
        );
    }

    if (!is_ram_enabled) {
        throw MemoryAccessException(
            "MBC1 RAM", "external RAM is currently not accessible",
            address, true
        );
    }

    if (address >= RAM_BANK_SIZE) {
        throw MemoryAccessException(
            "MBC1 RAM", "out of bounds", address, true
        );
    }

    uint8_t selected_ram_bank = advanced_banking ? ram_bank_number : 0;
    return ram[selected_ram_bank][address & (RAM_BANK_SIZE - 1)];
}

/* Write the given 8 bit value to the given address in the currently selected
 * RAM bank.
 * Throws a MemoryAccessException if there are no external RAM banks, if RAM is
 * not accessible or if the address is out of bounds. */
void Mbc1::write_ram(uint16_t address, uint8_t value) {

    if (!ram_size) {
        throw MemoryAccessException(
            "MBC1 RAM", "there are no external RAM banks", address, false
        );
    }

    if (!is_ram_enabled) {
        throw MemoryAccessException(
            "MBC1 RAM", "external RAM is currently not accessible",
            address, false
        );
    }

    if (address >= RAM_BANK_SIZE) {
        throw MemoryAccessException(
            "MBC1 RAM", "out of bounds", address, false
        );
    }

    uint8_t selected_ram_bank = advanced_banking ? ram_bank_number : 0;
    ram[selected_ram_bank][address & (RAM_BANK_SIZE - 1)] = value;
}