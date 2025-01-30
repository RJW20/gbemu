#include <iostream>
#include <cstdint>
#include "ppu_core.hpp"

/* Return the 8 bit value in VRAM at the given address.
 * Returns 0xFF if the address is out of boudns. */
uint8_t PpuCore::read_vram(uint16_t address) const {

    if (address >= VRAM_SIZE) {
        std::cerr << "Invalid PPU VRAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return vram[address];
}

/* Write the given 8 bit value to the VRAM at the given address.
 * Fails if the address is out of bounds. */
void PpuCore::write_vram(uint16_t address, uint8_t value) {

    if (address >= VRAM_SIZE) {
        std::cerr << "Invalid PPU VRAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    vram[address] = value;
}

/* Return the 8 bit value in OAM at the given address.
 * Returns 0xFF if the address is out of bounds. */
uint8_t PpuCore::read_oam(uint16_t address) const {
    
    if (address >= OAM_SIZE) {
        std::cerr << "Invalid PPU OAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return vram[address];
}

/* Write the given 8 bit value to the OAM at the given address.
 * Fails if the address is out of bounds. */
void PpuCore::write_oam(uint16_t address, uint8_t value) {

    if (address >= OAM_SIZE) {
        std::cerr << "Invalid PPU OAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    oam[address] = value;
}