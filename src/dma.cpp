#include <cstdint>
#include "dma.hpp"
#include "mmu.hpp"

// Set the mmu pointer.
void Dma::set_mmu(Mmu* _mmu) {
    mmu = _mmu;
}

// Set _source_address to it's power-on value. 
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

    mmu->write(
        OAM_ADDRESS + current_m_cycles,
        mmu->read(_source_address + current_m_cycles)
    );
    
    if (++current_m_cycles == TOTAL_TRANSFER_M_CYCLES) {
        transfer_in_progress = false;
    }

}

// Return _source_address >> 8.
uint8_t Dma::source_address() const {
    return _source_address >> 8;
}

// Set _source_address to the given value << 8 and start a transfer.
void Dma::set_source_address(uint8_t value) {
    _source_address = value << 8;
    transfer_in_progress = true;
    locked = 0;
    current_m_cycles = 0;
}