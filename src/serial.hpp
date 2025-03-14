#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <cstdint>
#include <string>
#include "interrupt_manager.hpp"
#include "timer.hpp"

/* Serial
 * Emulates the I/O ports for connection between 2 GameBoys.
 * Transfers can only happen using this GameBoy as the master.
 * The 8-bit transfers are based off the falling edge of bit 8 of the timer's
 * system counter (transferring 1 bit every ~8192Hz).
 * Only ever recieves 0xFF (as never connected to another GameBoy). */
class Serial {
public:
    Serial(InterruptManager* interrupt_manager, Timer* timer) :
        interrupt_manager(interrupt_manager), timer(timer) { reset(); }

    void reset();
    void tick();

    uint8_t sb() const { return sb_; }
    uint8_t sc() const { return sc_ | 0x7E; }
    void set_sb(uint8_t value) { sb_ = value; }
    void set_sc(uint8_t value);

    std::string representation() const;

private:
    InterruptManager* interrupt_manager;
    Timer* timer;

    bool transfer_in_progress : 1;
    bool previous_div_bit : 1;
    uint8_t bits_transferred : 4;

    uint8_t sb_;     // Serial data transfer
    uint8_t sc_;     // Serial transfer control
};

#endif