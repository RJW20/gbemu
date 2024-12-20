#include "serial.hpp"

// Reset all components to zero.
void Serial::reset() {
    sb = 0;
    sc = 0;
    transfer_counter = 0;
}

// Carry out 1 t-cycle.
void Serial::tick() {

    if (!transfer_in_progress()) {
        return;
    }

    if (++transfer_counter == 512) { // 512 t-cycles per byte transfer 
        transfer_counter = 0;
        interrupt_manager->request(InterruptType::Serial_);
    }
}

// Return true if the 7th bit in sc is 1.
bool Serial::transfer_in_progress() const{
    return sc >> 7;
}