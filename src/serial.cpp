#include <string>
#include <sstream>
#include "serial.hpp"
#include "interrupt_manager.hpp"

// Set the registers to their post boot ROM values.
void Serial::reset() {
    transfer_in_progress = false;
    sb_ = 0;
    sc_ = 0x7E;
}

// Carry out 1 t-cycle.
void Serial::tick() {

    if (!transfer_in_progress) {
        return;
    }

    const bool current_div_bit = timer->div() & 1;
    if (previous_div_bit && !current_div_bit && ++bits_transferred == 8) {
        transfer_in_progress = false;
        sb_ = 0xFF;
        sc_ &= 0x7F;
        interrupt_manager->request(InterruptType::SERIAL);
    }
    previous_div_bit = current_div_bit;
}

/* Set the first and last bits in sc_ to those in the given value and start a
 * transfer if they're both set. */
void Serial::set_sc(uint8_t value) {
    sc_ = value & 0x81;
    if ((sc_ >> 7) && (sc_ & 1)) {
        transfer_in_progress = true;
        previous_div_bit = timer->div() & 1;
        bits_transferred = 0;
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