#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <array>
#include "cartridge/cartridge.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "serial.hpp"
#include "dma.hpp"
#include "ppu.hpp"

/* Memory Management Unit
 * Simulates an array with the following contents:
 * - 0x0000-0x3FFF - Fixed cartridge ROM bank
 * - 0x4000-0x7FFF - Switchable catridge ROM bank
 * - 0x8000-0x9FFF - Video RAM
 * - 0xA000-0xBFFF - Switchable cartridge RAM bank
 * - 0xC000-0xDFFF - Work RAM
 * - 0xE000-0xFDFF - Echo RAM (ignored)
 * - 0xFE00-0xFE9F - Object attrubute memory
 * - 0xFEA0-0xFEFF - Unusable
 * - 0xFF00-0xFF7F - I/O registers
 * - 0xFF80-0xFFFE - High RAM
 * - 0xFFFF - Interrupt enable flag */
class Mmu {
public:
    Mmu(
        Cartridge* cartridge,
        InterruptManager* interrupt_manager,
        Timer* timer,
        Serial* serial,
        Dma* dma,
        Ppu* ppu
    ) : cartridge(cartridge), interrupt_manager(interrupt_manager),
        timer(timer), serial(serial), dma(dma), ppu(ppu) { reset(); }
    ~Mmu() {};

    void reset();
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    Cartridge* cartridge;
    InterruptManager* interrupt_manager;
    Timer* timer;
    Serial* serial;
    Dma* dma;
    Ppu* ppu;

    static constexpr uint16_t WRAM_SIZE = 0x2000;    // 8 KB
    static constexpr uint16_t HRAM_SIZE = 0x7F;      // 127 B

    std::array<uint8_t, WRAM_SIZE> wram;    // Work RAM
    std::array<uint8_t, HRAM_SIZE> hram;    // High RAM

    // Read/write regions
    static constexpr uint16_t ROM_UPPER = 0x8000;
    static constexpr uint16_t VRAM_UPPER = 0xA000;
    static constexpr uint16_t EXTERNAL_RAM_UPPER = 0xC000;
    static constexpr uint16_t WRAM_UPPER = 0xE000;
    static constexpr uint16_t ECHO_RAM_UPPER = 0xFE00;
    static constexpr uint16_t OAM_UPPER = 0xFEA0;
    static constexpr uint16_t UNUSABLE_MEMORY_UPPER = 0xFF00;
    static constexpr uint16_t IO_REGISTERS_UPPER = 0xFF80;
    static constexpr uint16_t HRAM_UPPER = 0xFFFF;
};

#endif