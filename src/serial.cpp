#include <string>
#include <sstream>
#include "serial.hpp"
#include "interrupt_manager.hpp"

// Set the registers to their post boot ROM values.
void Serial::reset() {
    sb_ = 0;
    sc_ = 0x7E;
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
        sb_ = 0xFF;
        sc_ &= 0x7F;
    }
}

// Return a string representation of the Serial.
std::string Serial::representation() const {
    std::ostringstream repr;
    repr << "Serial:" << std::hex
        << " SB = " << static_cast<int>(sb())
        << " SC = " << static_cast<int>(sc());
    return repr.str();
}