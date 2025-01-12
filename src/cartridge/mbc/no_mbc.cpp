#include <iostream>
#include <cstdint>
#include <fstream>
#include "no_mbc.hpp"
#include "base_mbc.hpp"

/* Return the 8 bit value in the ROM at the given address.
 * Returns 0xFF if the address is invalid. */
uint8_t NoMbc::read_rom(uint16_t address) const {

    if (address < ROM_BANK_SIZE) {
        return rom[0][address];
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        return rom[1][address & (ROM_BANK_SIZE - 1)];
    }
    
    else {
        std::cerr << "Invalid NoMBC ROM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }
}

// Does nothing.
void NoMbc::write_rom(uint16_t address, uint8_t value) {
    std::cerr << "Invalid NoMBC ROM write at address" << std::hex 
        << address << "." << std::endl;
}

// Return 0xFF.
uint8_t NoMbc::read_ram(uint16_t address) const {
    std::cerr << "Invalid NoMBC RAM read at address " << std::hex
        << address << " - there are no external RAM banks." << std::endl;
    return 0xFF;
}

// Does nothing.
void NoMbc::write_ram(uint16_t address, uint8_t value) {
    std::cerr << "Invalid NoMBC RAM write at address " << std::hex
        << address << " - there are no external RAM banks." << std::endl;
}