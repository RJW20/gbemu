#ifndef DMA_HPP
#define DMA_HPP

#include <cstdint>
#include <string>
#include "ppu/ppu.hpp"

class Mmu;

/* Direct Memory Access (DMA)
 * Enables fast copying of data to the OAM removing responsibility from the
 * main program. */
class Dma {
public:
    Dma(Ppu* ppu) : ppu(ppu) { reset(); }

    void set_mmu(Mmu* _mmu) { mmu = _mmu; }

    void reset();
    void tick();

    uint8_t source_address() const { return _source_address >> 8; }
    void set_source_address(uint8_t value);

    bool transfer_in_progress;

    std::string representation() const;

private:
    Ppu* ppu;
    Mmu* mmu;

    // The 16 bit address to start the transfer from
    uint16_t _source_address;

    bool delay_in_progress;
    uint8_t locked;
    uint8_t delay_m_cycles;
    uint8_t transfer_m_cycles;
    static constexpr uint8_t TOTAL_DELAY_M_CYCLES = 2;
    static constexpr uint8_t TOTAL_TRANSFER_M_CYCLES = 0xA0;
};

#endif