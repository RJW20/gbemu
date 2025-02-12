#include <cstdint>
#include <fstream>
#include "no_mbc.hpp"
#include "../../exceptions.hpp"

/* Return the 8 bit value in the ROM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t NoMbc::read_rom(uint16_t address) const {

    if (address < ROM_BANK_SIZE) {
        return rom[0][address];
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        return rom[1][address & (ROM_BANK_SIZE - 1)];
    }
    
    else {
        throw MemoryAccessException(
            "NoMBC ROM", "out of bounds", address, true
        );
    }
}

// Throws a MemoryAccessException.
void NoMbc::write_rom(uint16_t address, uint8_t value) {
    throw MemoryAccessException(
        "NoMBC ROM", "there is no Memory Bank Controller", address, false
    );
}

// Throws a MemoryAccessException.
uint8_t NoMbc::read_ram(uint16_t address) const {
    throw MemoryAccessException(
        "NoMBC RAM", "there are no external RAM banks", address, true
    );
}

// Throws a MemoryAccessException.
void NoMbc::write_ram(uint16_t address, uint8_t value) {
    throw MemoryAccessException(
        "NoMBC RAM", "there are no external RAM banks", address, false
    );
}