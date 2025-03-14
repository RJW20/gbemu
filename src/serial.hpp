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

    uint8_t sb() const { return sb_; }
    uint8_t sc() const { return sc_ | 0x7E; }
    void set_sb(uint8_t value) { sb_ = value; }
    void set_sc(uint8_t value) { sc_ = value & 0x81; }

    std::string representation() const;

private:
    InterruptManager* interrupt_manager;

    uint16_t transfer_counter;  // Counts to 512 (8192 Hz) per byte transfer

    uint8_t sb_;     // Serial data transfer
    uint8_t sc_;     // Serial transfer control

    bool transfer_in_progress() const { return (sc_ >> 7) && (sc_ & 1); }
};

#endif