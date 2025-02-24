#ifndef MBC3_HPP
#define MBC3_HPP

#include <cstdint>
#include <chrono>
#include <fstream>
#include "base_mbc.hpp"

// RealTimeClock
class Rtc {
public:
    Rtc() {}

    void update();
    uint8_t read(uint8_t reg) const;
    void write(uint8_t reg, uint8_t value);

private:
    uint8_t seconds : 6;
    uint8_t minutes : 6;
    uint8_t hours : 5;
    uint16_t days : 9;
    uint8_t halted : 1;
    uint8_t overflowed : 1;
    std::chrono::time_point<std::chrono::steady_clock> previous_update_time =
        std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> halt_time;
};

// Memory Bank Controller version 3.
class Mbc3 : public BaseMbc {
public:
    Mbc3(std::ifstream& rom_file, bool has_external_ram, bool has_rtc) : 
        BaseMbc(rom_file, has_external_ram), has_rtc(has_rtc) { reset(); }

    void reset() override;

    uint8_t read_rom(uint16_t address) const override;
    void write_rom(uint16_t address, uint8_t value) override;
    uint8_t read_ram(uint16_t address) const override;
    void write_ram(uint16_t address, uint8_t value) override;

private:
    // Real time clock
    bool has_rtc : 1;
    Rtc rtc;
    Rtc latched_rtc;

    // Registers
    bool ram_rtc_enabled : 1;       // Controls if RAM and RTC are accessible
    uint8_t rom_bank_number : 7;    // Currently selected ROM bank
    uint8_t ram_bank_number : 4;    // Currently selected RAM bank
    uint8_t latch_clock;            // Controls when to latch time to RTC

    // Write regions
    static constexpr uint16_t IS_RAM_ENABLED_UPPER = 0x2000;
    static constexpr uint16_t RAM_BANK_NUMBER_UPPER = 0x6000;
};

#endif