#include <cstdint>
#include <string>
#include <sstream>
#include <fstream>
#include "apu.hpp"
#include "exceptions.hpp"

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
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t Apu::read_wave_ram(const uint8_t& address) const {

    if (address >= 0x10) {
        throw MemoryAccessException(
            "APU wave RAM", "out of bounds", address, true
        );
    }

    return wave_ram[address];
}

/* Write the given 8 bit value to the wave RAM at the given address.
 * Throws a MemoryAccessException if the address is out of bounds. */
void Apu::write_wave_ram(const uint8_t& address, const uint8_t& value) {

    if (address >= 0x10) {
        throw MemoryAccessException(
            "APU wave RAM", "out of bounds", address, false
        );
    }

    wave_ram[address] = value;
}

// Return a string representation of the APU.
std::string Apu::representation() const {
    std::ostringstream repr;
    repr << "APU:" << std::hex
        << " NR10 = " << static_cast<int>(nr10())
        << " NR11 = " << static_cast<int>(nr11())
        << " NR12 = " << static_cast<int>(nr12())
        << " NR13 = " << static_cast<int>(nr13())
        << " NR14 = " << static_cast<int>(nr14())
        << " NR21 = " << static_cast<int>(nr21())
        << " NR22 = " << static_cast<int>(nr22())
        << " NR23 = " << static_cast<int>(nr23())
        << " NR24 = " << static_cast<int>(nr24())
        << " NR30 = " << static_cast<int>(nr30())
        << " NR31 = " << static_cast<int>(nr31())
        << " NR32 = " << static_cast<int>(nr32())
        << " NR33 = " << static_cast<int>(nr33())
        << " NR34 = " << static_cast<int>(nr34())
        << " NR41 = " << static_cast<int>(nr41())
        << " NR42 = " << static_cast<int>(nr42())
        << " NR43 = " << static_cast<int>(nr43())
        << " NR44 = " << static_cast<int>(nr44())
        << " NR50 = " << static_cast<int>(nr50())
        << " NR51 = " << static_cast<int>(nr51())
        << " NR52 = " << static_cast<int>(nr52());
    return repr.str();
}

// Output a string representation of the APU to the given ostream.
std::ostream& operator<<(std::ostream& os, const Apu& apu) {
    os << apu.representation();
    return os;
}