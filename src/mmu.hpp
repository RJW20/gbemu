#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <vector>
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "cartridge/cartridge.hpp"

/* Memory Management Unit
 * Simulates an array with the following contents:
 * - 0x0000-0x3FFF - Fixed cartridge ROM bank
 * - 0x4000-0x7FFF - Switchable catridge ROM bank
 * - 0x8000-0x9FFF - Video RAM
 * - 0xA000-0xBFFF - Switchable cartridge RAM bank
 * - 0xC000-0xDFFF - Work RAM
 * - 0xE000-0xFDFF - Echo RAM (ignored)
 * - 0xFE00-0xFE9F - OAM
 * - 0xFEA0-0xFEFF - Unusable
 * - 0xFF00-0xFF7F - I/O registers
 * - 0xFF80-0xFFFE - High RAM
 * - 0xFFFF - Interrupt enable flag */
class Mmu {
public:
    Mmu(
        Cartridge* cartridge,
        InterruptManager* interrupt_manager,
        Timer* timer
    );
    ~Mmu() {};

    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    Cartridge* cartridge;
    std::vector<uint8_t> wram;              // Work RAM
    InterruptManager* interrupt_manager;
    Timer* timer;
    std::vector<uint8_t> hram;              // High RAM
};

#endif