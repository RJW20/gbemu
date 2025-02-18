#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <cstdint>
#include <string>
#include "interrupt_manager.hpp"

/* Serial
 * Emulates the I/O port for connection between 2 GameBoys. 
 * Assumes the GameBoy is never connected to another GameBoy. */
class Serial {
public:
    Serial(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) { reset(); }

    void reset();
    void tick();

    uint8_t sb;     // Serial data transfer
    uint8_t sc;     // Serial transfer control

    std::string representation() const;

private:
    InterruptManager* interrupt_manager;

    uint16_t transfer_counter;  // Counts to 512 (8192 Hz) per byte transfer

    bool transfer_in_progress() const { return (sc >> 7) && (sc & 1); }
};

#endif