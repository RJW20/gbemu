#include <cstdint>
#include "ppu_core.hpp"
#include "exceptions.hpp"

/* Return the 8 bit value in VRAM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t PpuCore::read_vram(uint16_t address) const {

    if (address >= VRAM_SIZE) {
        throw MemoryAccessException(
            "PPU VRAM", "out of bounds", address, true
        );
    }

    return vram[address];
}

/* Write the given 8 bit value to the VRAM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
void PpuCore::write_vram(uint16_t address, uint8_t value) {

    if (address >= VRAM_SIZE) {
        throw MemoryAccessException(
            "PPU VRAM", "out of bounds", address, false
        );
    }

    vram[address] = value;
}

/* Return the 8 bit value in OAM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t PpuCore::read_oam(uint16_t address) const {
    
    if (address >= OAM_SIZE) {
        throw MemoryAccessException(
            "PPU OAM", "out of bounds", address, true
        );
    }

    return vram[address];
}

/* Write the given 8 bit value to the OAM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
void PpuCore::write_oam(uint16_t address, uint8_t value) {

    if (address >= OAM_SIZE) {
        throw MemoryAccessException(
            "PPU OAM", "out of bounds", address, false
        );
    }

    oam[address] = value;
}