#include <cstdint>
#include <string>
#include <sstream>
#include "dma.hpp"
#include "mmu.hpp"

// Set _source_address to its post boot ROM value. 
void Dma::reset() {
    _source_address = 0xFF;
    transfer_in_progress = false;
}

// Carry out 1 t-cycle.
void Dma::tick() {

    if (!transfer_in_progress) {
        return;
    }

    // Only act every 4 t-cycles
    if (++locked < 4) {
        return;
    }
    locked = 0;

    // Delay for first 2 m-cycles
    if (current_m_cycles < DELAY_M_CYCLES) {
        current_m_cycles++;
        return;
    }

    ppu->write_oam(
        current_m_cycles - DELAY_M_CYCLES,
        mmu->read(_source_address + current_m_cycles - DELAY_M_CYCLES)
    );
    
    if (++current_m_cycles == DELAY_M_CYCLES + TOTAL_TRANSFER_M_CYCLES) {
        transfer_in_progress = false;
    }
}

// Set _source_address to the given value << 8 and start a transfer.
void Dma::set_source_address(uint8_t value) {
    _source_address = value << 8;
    transfer_in_progress = true;
    locked = 0;
    current_m_cycles = 0;
}

// Return a string representation of the DMA.
std::string Dma::representation() const {
    std::ostringstream repr;
    repr << "DMA:" << std::hex
        << " source_address = " << static_cast<int>(source_address());
    return repr.str();
}