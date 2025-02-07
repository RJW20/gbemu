#include <iostream>
#include <cstdint>
#include "apu.hpp"

// Set the registers to their post boot ROM values and clear the wave RAM.
void Apu::reset() {
    nr10_ = 0x80;
    nr11_ = 0xBF;
    nr12_ = 0xF3;
    nr13_ = 0xFF;
    nr14_ = 0xBF;
    nr21_ = 0x3F;
    nr22_ = 0;
    nr23_ = 0xFF;
    nr24_ = 0xBF;
    nr30_ = 0x7F;
    nr31_ = 0xFF;
    nr32_ = 0x9F;
    nr33_ = 0xFF;
    nr34_ = 0xBF;
    nr41_ = 0xFF;
    nr42_ = 0;
    nr43_ = 0;
    nr44_ = 0xBF;
    nr50_ = 0x77;
    nr51_ = 0xF3;
    nr52_ = 0xF1;

    wave_ram.fill(0);
}

/* Return the 8 bit value in wave RAM at the given address.
 * Returns 0xFF if the address is out of boudns. */
uint8_t Apu::read_wave_ram(uint16_t address) const {

    if (address >= 0x10) {
        std::cerr << "Invalid APU wave RAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return wave_ram[address];
}

/* Write the given 8 bit value to the wave RAM at the given address.
 * Fails if the address is out of bounds. */
void Apu::write_wave_ram(uint16_t address, uint8_t value) {

    if (address >= 0x10) {
        std::cerr << "Invalid APU wave RAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    wave_ram[address] = value;
}