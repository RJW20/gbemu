#include <cstdint>
#include <string>
#include <sstream>
#include "dma.hpp"
#include "mmu.hpp"

// Set source_address_ to its post boot ROM value. 
void Dma::reset() {
    source_address_ = 0xFF;
    transfer_in_progress = false;
    locked = 0;
    delay_m_cycles = 0;
    transfer_m_cycles = 0;
}

// Carry out 1 t-cycle.
void Dma::tick() {

    if (!delay_in_progress && !transfer_in_progress) {
        return;
    }

    // Only act every 4 t-cycles
    if (++locked < 4) {
        return;
    }
    locked = 0;

    if (delay_in_progress && ++delay_m_cycles == TOTAL_DELAY_M_CYCLES) {
        transfer_in_progress = true;
        delay_in_progress = false;
        transfer_m_cycles = 0;
        return;
    }

    if (transfer_in_progress) {
        ppu->write_oam(
            transfer_m_cycles,
            mmu->read(source_address_ + transfer_m_cycles)
        );
        
        if (++transfer_m_cycles == TOTAL_TRANSFER_M_CYCLES) {
            transfer_in_progress = false;
        }
    }
}

// Set source_address_ to the given value << 8 and start a transfer.
void Dma::set_source_address(uint8_t value) {
    source_address_ = value << 8;
    delay_in_progress = true;
    locked = 0;
    delay_m_cycles = 0;
}

// Return a string representation of the DMA.
std::string Dma::representation() const {
    std::ostringstream repr;
    repr << "DMA:" << std::hex
        << " source_address = " << static_cast<int>(source_address());
    return repr.str();
}