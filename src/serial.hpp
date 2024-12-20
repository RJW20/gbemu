#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <cstdint>
#include "interrupt_manager.hpp"

/* Emulates the I/O port for connection between 2 GameBoys. 
 * Assumes the GameBoy is never connected to another GameBoy. */
class Serial {
public:
    Serial(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) {reset();};
    ~Serial() {};

    uint8_t sb;     // Serial data transfer
    uint8_t sc;     // Serial transfer control

    void reset();
    void tick();

private:
    InterruptManager* interrupt_manager;

    uint16_t transfer_counter;  // Counts to 512 (8192 Hz) per byte transfer

    bool transfer_in_progress() const;
};

#endif SERIAL_HPP