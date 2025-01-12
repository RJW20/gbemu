#include <iostream>
#include <cstdint>
#include <fstream>
#include "mbc2.hpp"
#include "base_mbc.hpp"

/* Construct BaseMbc, initialise 512 x 4 bit RAM and set the registers to their
 * default values. */
Mbc2::Mbc2(std::ifstream& rom_file) : BaseMbc(rom_file, false) {
    ram_size = 1;
    reset();
}

// Clear RAM and set registers to their default power-on values.
void Mbc2::reset() {
    ram.resize(ram_size, std::vector<uint8_t>(BUILT_IN_RAM_SIZE));
    is_ram_enabled = false;
    rom_bank_number = 1;
}

/* Return the 8 bit value in the currently selected ROM bank at the given
 * address.
 * Returns 0xFF if the address is out of bounds. */
uint8_t Mbc2::read_rom(uint16_t address) const {

    if (address < ROM_BANK_SIZE) {
        return rom[0][address];
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        uint8_t selected_rom_bank = rom_bank_number & (rom_size - 1);
        return rom[selected_rom_bank][address & (ROM_BANK_SIZE - 1)];
    }
    
    else {
        std::cerr << "Invalid MBC2 ROM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }
}

/* Modify the registers depending on bit 8 in the given address and the given
 * value as follows:
 * - 0 - is_ram_enabled
 * Set true if the value is 0xA, otherwise false.
 * - 1 - rom_bank_number
 * Set to the trailing 4 bits of value. If set to 0 is incremented to 1. */
void Mbc2::write_rom(uint16_t address, uint8_t value) {

    if (address < ROM_BANK_SIZE) {
        if (!((address >> 8) & 1)) {
            is_ram_enabled = (value == 0xA);
        }
        else {
            rom_bank_number = value & 0b1111;
            rom_bank_number = rom_bank_number ? rom_bank_number : 1;
        }
    }

    if (address >= 2 * ROM_BANK_SIZE) {
        std::cerr << "Invalid MBC2 ROM write at address " << std::hex 
            << address << " - out of bounds." << std::endl;
    }
}

/* Return the trailing 4 bits of the value in the RAM at the given address.
 * The value will be padded with leading 0s.
 * Only the bottom 9 bits of the address are used, so those over 0x200 will be
 * echoes of the region 0x000-0x1FF.
 * Returns 0xFF if RAM is not accessible or if the address is out of bounds. */
uint8_t Mbc2::read_ram(uint16_t address) const {

    if (!is_ram_enabled) {
        std::cerr << "Invalid MBC2 RAM read at address " << std::hex
            << address << " - built-in RAM is currently not accessible."
            << std::endl;
        return 0xFF;
    }

    if (address >= BUILT_IN_RAM_SIZE)
    {
        std::cerr << "Invalid MBC2 RAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return ram[0][address & (BUILT_IN_RAM_SIZE - 1)] & 0xF;
}

/* Write the trailing 4 bits of the given 8 bit value to the RAM at the given
 * address.
 * Fails if RAM is not accessible or if the address is out of bounds. */
void Mbc2::write_ram(uint16_t address, uint8_t value) {

    if (!is_ram_enabled) {
        std::cerr << "Invalid MBC2 RAM write at address " << std::hex
            << address << " - built-in RAM is currently not accessible."
            << std::endl;
        return;
    }

    if (address >= BUILT_IN_RAM_SIZE)
    {
        std::cerr << "Invalid MBC2 RAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    ram[0][address & (BUILT_IN_RAM_SIZE - 1)] = value & 0xF;
}