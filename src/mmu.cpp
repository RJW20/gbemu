#include <iostream>
#include <cstdint>
#include "mmu.hpp"

/* Set wram and hram to their respective sizes and fill them with zeros. */
void Mmu::reset() {
    wram.resize(0x2000, 0);
    hram.resize(0x7F, 0);
}

/* Return the 8 bit value stored at the given address.
 * Directs the address to its corresponding component. */
uint8_t Mmu::read(uint16_t address) const {

    if (address < 0x8000) {
        return cartridge->read_rom(address);
    }

    else if (0x8000 <= address && address < 0xA000) {
        std::cerr << "Failed to read from address: " << std::hex << address <<
            ". Video RAM currently not implemented." << std::endl;
        return 0xFF;
    }

    else if (0xA000 <= address && address < 0xC000) {
        return cartridge->read_ram(address - 0xA000);
    }
    
    else if (0xC000 <= address && address < 0xE000) {
        return wram[address - 0xC000];
    }

    else if (0xE000 <= address && address < 0xFE00) {
        return wram[address - 0xE000];
    }

    else if (0xFE00 <= address && address < 0xFEA0) {
        std::cerr << "Failed to read from address: " << std::hex << address <<
            ". OAM currently not implemented." << std::endl;
        return 0xFF;
    }

    else if (0xFEA0 <= address && address < 0xFEFF) {
        std::cerr << "Failed to read from address: " << std::hex << address <<
            ". This section of RAM is unusable." << std::endl;
        return 0xFF;
    }

    else if (0xFF80 <= address && address < 0xFFFE) {
        return hram[address - 0xFF80];
    }

    else {
        switch (address) {
            case 0xFF01:
                return serial->sb;
            case 0xFF02:
                return serial->sc;
            case 0xFF04:
                return timer->div();
            case 0xFF05:
                return timer->tima;
            case 0xFF06:
                return timer->tma;
            case 0xFF07:
                return timer->tac;
            case 0xFF0F:
                return interrupt_manager->ix;
            case 0xFFFF:
                return interrupt_manager->ie;
            default:
                std::cerr << "Failed to read from address: " << std::hex <<
                    address << ". I/O registers not fully implemented." <<
                    std::endl;
                return 0xFF;
        }
    }
}

/* Write the given 8 bit value to the given address.
 * Directs the address to its corresponding component. */
void Mmu::write(uint16_t address, uint8_t value) {

    if (address < 0x8000) {
        cartridge->write_rom(address, value);
    }

    else if (0x8000 <= address && address < 0xA000) {
        std::cerr << "Failed to write to address: " << std::hex << address <<
            ". Video RAM currently not implemented." << std::endl;
    }

    else if (0xA000 <= address && address < 0xC000) {
        cartridge->write_ram(address - 0xA000, value);
    }
    
    else if (0xC000 <= address && address < 0xE000) {
        wram[address - 0xC000] = value;
    }

    else if (0xE000 <= address && address < 0xFE00) {
        std::cerr << "Failed to write to address: " << std::hex << address <<
            ". Echo RAM should not be accessed." << std::endl;
    }

    else if (0xFE00 <= address && address < 0xFEA0) {
        std::cerr << "Failed to write to address: " << std::hex << address <<
            ". OAM currently not implemented." << std::endl;
    }

    else if (0xFEA0 <= address && address < 0xFEFF) {
        std::cerr << "Failed to write to address: " << std::hex << address <<
            ". This section of RAM is unusable." << std::endl;
    }

    else if (0xFF80 <= address && address < 0xFFFE) {
        hram[address - 0xFF80] = value;
    }

    else {
        switch (address) {
            case 0xFF01:
                serial->sb = value;
                break;
            case 0xFF02:
                serial->sc = value;
                break;
            case 0xFF04:
                timer->set_div();
                break;
            case 0xFF05:
                timer->set_tima(value);
                break;
            case 0xFF06:
                timer->set_tma(value);
                break;
            case 0xFF07:
                timer->set_tac(value);
                break;
            case 0xFF0F:
                interrupt_manager->ix = value;
                break;
            case 0xFFFF:
                interrupt_manager->ie = value;
                break;
            default:
                std::cerr << "Failed to write to address: " << std::hex <<
                    address << ". I/O registers not fully implemented." <<
                    std::endl;
        }
    }
}