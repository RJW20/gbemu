#ifndef DMA_HPP
#define DMA_HPP

#include <cstdint>
#include <string>

class Mmu;

/* Direct Memory Access (DMA)
 * Enables fast copying of data to the OAM removing responsibility from the
 * main program. */
class Dma {
public:
    Dma() { reset(); }

    void set_mmu(Mmu* _mmu);

    void reset();
    void tick();

    bool transfer_in_progress;

    uint8_t source_address() const;
    void set_source_address(uint8_t value);

    std::string representation() const;

private:
    Mmu* mmu;

    uint8_t locked;
    uint8_t current_m_cycles;
    static constexpr uint16_t TOTAL_TRANSFER_M_CYCLES = 160;

    // The 16 bit address to start the transfer from
    uint8_t _source_address;
    static constexpr uint16_t OAM_ADDRESS = 0xFE00;
};

#endif