#include <fstream>
#include "serial.hpp"
#include "interrupt_manager.hpp"

#include <iostream>

// Set the registers to their post boot ROM values.
void Serial::reset() {
    sb = 0;
    sc = 0x7E;
    transfer_counter = 0;
}

// Carry out 1 t-cycle.
void Serial::tick() {

    if (!transfer_in_progress()) {
        return;
    }

    if (++transfer_counter == 512) { // 512 t-cycles per byte transfer 
        transfer_counter = 0;
        interrupt_manager->request(InterruptType::SERIAL);
    }
}

// Output a string representation of the Serial to the given ostream.
std::ostream& operator<<(std::ostream& os, const Serial& serial) {
    os << "Serial:" << std::hex
        << " SB = " << static_cast<int>(serial.sb)
        << " SC = " << static_cast<int>(serial.sc);
    return os;
}