#include <cstdint>
#include <chrono>
#include "mbc3.hpp"
#include "../../exceptions.hpp"

/* Update the registers to include the time that has passed since the last
 * update.
 * The elapsed time is added to the seconds register, and a tick is sent up the
 * chain to the next register only if it takes the register through its maximum
 * (e.g. 60 for seconds and minutes, 24 for hours).
 * If a register has a value greater than or equal to its maximum (via a write)
 * then it will overflow its number of bits as appropriate without sending a
 * tick to the next register. */
void Rtc::update() {

    if (halted) {
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    uint64_t elapsed_seconds =
        std::chrono::duration<double>(now - previous_update_time).count();
    if (!elapsed_seconds) {
        return;
    }
    uint64_t elapsed_minutes = 0;
    uint64_t elapsed_hours = 0;
    uint64_t elapsed_days = 0;

    if (seconds > 59) {
        const uint8_t count_to_overflow = 64 - seconds;
        if (elapsed_seconds > count_to_overflow - 1) {
            seconds = 0;
            elapsed_seconds -= count_to_overflow;
        }
    }
    else {
        const uint8_t count_to_tick = 60 - seconds;
        if (elapsed_seconds > count_to_tick - 1) {
            seconds = 0;
            elapsed_seconds -= count_to_tick;
            elapsed_minutes = 1;
        }
    }
    seconds += elapsed_seconds % 60;
    elapsed_minutes += elapsed_seconds / 60;

    if (minutes > 59) {
        const uint8_t count_to_overflow = 64 - minutes;
        if (elapsed_minutes > count_to_overflow - 1) {
            minutes = 0;
            elapsed_minutes -= count_to_overflow;
        }
    }
    else {
        const uint8_t count_to_tick = 60 - minutes;
        if (elapsed_minutes > count_to_tick - 1) {
            minutes = 0;
            elapsed_minutes -= count_to_tick;
            elapsed_hours = 1;
        }
    }
    minutes += elapsed_minutes % 60;
    elapsed_hours += elapsed_minutes / 60;

    if (hours > 23) {
        const uint8_t count_to_overflow = 32 - hours;
        if (elapsed_hours > count_to_overflow - 1) {
            hours = 0;
            elapsed_hours -= count_to_overflow;
        }
    }
    else {
        const uint8_t count_to_tick = 24 - hours;
        if (elapsed_hours > count_to_tick - 1) {
            hours = 0;
            elapsed_hours -= count_to_tick;
            elapsed_days = 1;
        }
    }
    hours += elapsed_hours % 24;
    elapsed_days += elapsed_hours / 24;

    const uint16_t count_to_overflow = 512 - days;
    if (elapsed_days > count_to_overflow - 1) {
        days = 0;
        overflowed = true;
        //halt_time = now;
    }
    else {
        days += elapsed_days;
    }

    previous_update_time = now;
}

/* Read the value from the corresponding RTC register.
 * Throws a MemoryAccessException if the given register is invalid. */
uint8_t Rtc::read(uint8_t reg) const {
    switch(reg) {
        case 0x8:
            return seconds;
        case 0x9:
            return minutes;
        case 0xA:
            return hours;
        case 0xB:
            return days & 0xFF;
        case 0xC:
            return (overflowed << 7) | (halted << 6) | (days >> 8);
        default:
            throw MemoryAccessException(
                "RTC", "invalid RTC register", reg, true
            );
    }
}

/* Write the value to the corresponding RTC register. 
 * Adjusts previous_update_time as necessary if the written value changes the
 * halted or overflow status of the RTC. 
 * Throws a MemoryAccessException if the given register is invalid. */
void Rtc::write(uint8_t reg, uint8_t value) {
    
    update();
    
    switch (reg) {
        case 0x8:
            seconds = value & 0x3F;
            break;
        case 0x9:
            minutes = value & 0x3F;
            break;
        case 0xA:
            hours = value & 0x1F;
            break;
        case 0xB:
            days = (days & 0x100) | value;
            break;
        case 0x0C: {
            const bool previous_halted = (halted);
            days = ((value & 0x1) << 8) | (days & 0xFF);
            halted = value >> 6;
            overflowed = value >> 7;
            if (!previous_halted) {
                if (halted) {
                    halt_time = std::chrono::steady_clock::now();
                }
            }
            else {
                if (!(halted)) {
                    previous_update_time += std::chrono::steady_clock::now() - halt_time;
                }
            }
            break;
        }
        default:
            throw MemoryAccessException(
                "RTC", "invalid RTC register", reg, false
            );
    }
}

// Clear RAM and set registers to their default power-on values.
void Mbc3::reset() {
    BaseMbc::reset();
    ram_rtc_enabled = false;
    rom_bank_number = 1;
    ram_bank_number = 0;
    latch_clock = 0xFF;
}

/* Return the 8 bit value in the currently selected ROM bank at the given
 * address.
 * Throws a MemoryAccessException if the address is out of bounds. */
uint8_t Mbc3::read_rom(uint16_t address) const {

    if (address < ROM_BANK_SIZE) {
        return rom[0][address];
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        return rom[rom_bank_number][address & (ROM_BANK_SIZE - 1)];
    }
    
    else {
        throw MemoryAccessException(
            "MBC3 ROM", "out of bounds", address, true
        );
    }
}

/* Modify the registers depending on the region the given address falls in and
 * the given value as follows:
 * - 0x0000-0x1FFF - ram_rtc_enabled
 * Set true if the trailing 4 bits of value are 0xA, otherwise false.
 * - 0x2000-0x3FFF - rom_bank_number
 * Set to the trailing 7 bits of value. If set to 0 is incremented to 1.
 * - 0x4000-0x5FFF - ram_bank_number
 * Set to the trailing 4 bits of value.
 * - 0x6000-0x7FFF - latch_clock
 * Set to value, and update the rtc if its last value was 0 and the new one is
 * 1.
 * Throws a MemoryAccessException if the address is out of bounds. */
void Mbc3::write_rom(uint16_t address, uint8_t value) {

    if (address < IS_RAM_ENABLED_UPPER) {
        ram_rtc_enabled = ((value & 0xF) == 0xA);
    }

    else if (address < ROM_BANK_SIZE) {
        rom_bank_number = value & 0x7F;
        rom_bank_number = rom_bank_number ? rom_bank_number : 1;
    }

    else if (address < RAM_BANK_NUMBER_UPPER) {
        ram_bank_number = value & 0xF;
    }

    else if (address < 2 * ROM_BANK_SIZE) {
        if (!latch_clock && value == 1) {
            rtc.update();
            latched_rtc = rtc;
        }
        latch_clock = value;
    }
    
    else {
        throw MemoryAccessException(
            "MBC3 ROM", "out of bounds", address, false
        );
    }
}

/* Return the 8 bit value in the currently selected RAM bank at the given
 * address, or the value in the currently selected RTC register.
 * Throws a MemoryAccessException if RAM/RTC are not enabled, if the address is
 * out of bounds, if trying to access RAM but there is none or if trying to
 * access the RTC and there isn't one. */
uint8_t Mbc3::read_ram(uint16_t address) const {

    if (!ram_rtc_enabled) {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", 
            "external RAM/RTC registers are currently not accessible",
            address, true
        );
    }

    if (address >= RAM_BANK_SIZE) {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", "out of bounds", address, true
        );
    }

    if (ram_bank_number < 4) {
        if (!ram_size) {
            throw MemoryAccessException(
                "MBC3 RAM", "there are no external RAM banks", address, true
            );
        }
        return ram[ram_bank_number][address & (RAM_BANK_SIZE - 1)];
    }
    else if (ram_bank_number > 7 && ram_bank_number < 0xD) {
        if (!has_rtc) {
            throw MemoryAccessException(
                "MBC3 RTC", "there is no Real Time Clock", address, true
            );
        }
        return latched_rtc.read(ram_bank_number);
    }
    else {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", "invalid RAM bank number/RTC register select",
            address, true
        );
    }
}

/* Write the given 8 bit value to the given address in the currently selected
 * RAM bank, or to the currently selected RTC register.
 * Throws a MemoryAccessException if RAM/RTC are not enabled, if the address is
 * out of bounds, if trying to access RAM but there is none or if trying to
 * access the RTC and there isn't one. */
void Mbc3::write_ram(uint16_t address, uint8_t value) {

    if (!ram_rtc_enabled) {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", 
            "external RAM/RTC registers are currently not accessible",
            address, false
        );
    }

    if (address >= RAM_BANK_SIZE) {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", "out of bounds", address, false
        );
    }

    if (ram_bank_number < 4) {
        if (!ram_size) {
            throw MemoryAccessException(
                "MBC3 RAM", "there are no external RAM banks", address, false
            );
        }
        ram[ram_bank_number][address & (RAM_BANK_SIZE - 1)] = value;
    }
    else if (ram_bank_number > 7 && ram_bank_number < 0xD) {
        if (!has_rtc) {
            throw MemoryAccessException(
                "MBC3 RTC", "there is no Real Time Clock", address, false
            );
        }
        rtc.write(ram_bank_number, value);
    }
    else {
        throw MemoryAccessException(
            "MBC3 RAM/RTC", "invalid RAM bank number/RTC register select",
            address, false
        );
    }
}