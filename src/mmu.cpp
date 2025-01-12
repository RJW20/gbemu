#include <iostream>
#include <cstdint>
#include "mmu.hpp"

// Clear WRAM and HRAM.
void Mmu::reset() {
    wram.fill(0);
    hram.fill(0);
}

/* Return the 8 bit value stored at the given address.
 * Directs the address to its corresponding component. */
uint8_t Mmu::read(uint16_t address) const {

    if (address < ROM_UPPER) {
        return cartridge->read_rom(address);
    }

    else if (address < VRAM_UPPER) {
        return ppu->read_vram(address - ROM_UPPER);
    }

    else if (address < EXTERNAL_RAM_UPPER) {
        return cartridge->read_ram(address - VRAM_UPPER);
    }
    
    else if (address < WRAM_UPPER) {
        return wram[address - EXTERNAL_RAM_UPPER];
    }

    else if (address < ECHO_RAM_UPPER) {
        return wram[address - EXTERNAL_RAM_UPPER];
    }

    else if (address < OAM_UPPER) {
        return ppu->read_oam(address - ECHO_RAM_UPPER);
    }

    else if (address < UNUSABLE_MEMORY_UPPER) {
        std::cerr << "Invalid MMU read at address " << std::hex << address
            << " - this section of RAM is unusable." << std::endl;
        return 0xFF;
    }

    else if (address < IO_REGISTERS_UPPER) {
        switch (address) {
            case 0xFF01:
                return serial->sb;
            case 0xFF02:
                return serial->sc;
            case 0xFF04:
                return timer->div();
            case 0xFF05:
                return timer->tima();
            case 0xFF06:
                return timer->tma();
            case 0xFF07:
                return timer->tac();
            case 0xFF0F:
                return interrupt_manager->ix;
            case 0xFF40:
                return ppu->lcdc;
            case 0xFF41:
                return ppu->stat();
            case 0xFF42:
                return ppu->scy;
            case 0xFF43:
                return ppu->scx;
            case 0xFF44:
                return ppu->ly();
            case 0xFF45:
                return ppu->lyc;
            case 0xFF46:
                return dma->source_address();
            case 0xFF47:
                return ppu->bgp;
            case 0xFF48:
                return ppu->obp0;
            case 0xFF49:
                return ppu->obp1;
            case 0xFF4A:
                return ppu->wy;
            case 0xFF4B:
                return ppu->wx;
            default:
                std::cerr << "Invalid MMU read at address " << std::hex
                    << address << " - this I/O register does not exist."
                    << std::endl;
                return 0xFF;
        }
    }

    else if (address < HRAM_UPPER) {
        return hram[address - IO_REGISTERS_UPPER];
    }

    else {
        return interrupt_manager->ie;
    }
}

/* Write the given 8 bit value to the given address.
 * Directs the address to its corresponding component. */
void Mmu::write(uint16_t address, uint8_t value) {

    if (address < ROM_UPPER) {
        cartridge->write_rom(address, value);
    }

    else if (address < VRAM_UPPER) {
        ppu->write_vram(address - ROM_UPPER, value);
    }

    else if (address < EXTERNAL_RAM_UPPER) {
        cartridge->write_ram(address - VRAM_UPPER, value);
    }
    
    else if (address < WRAM_UPPER) {
        wram[address - EXTERNAL_RAM_UPPER] = value;
    }

    else if (address < ECHO_RAM_UPPER) {
        std::cerr << "Invalid MMU write at address " << std::hex << address
            << " - echo RAM cannot be written to." << std::endl;
    }

    else if (address < OAM_UPPER) {
        ppu->write_oam(address - ECHO_RAM_UPPER, value);
    }

    else if (address < UNUSABLE_MEMORY_UPPER) {
        std::cerr << "Invalid MMU write at address " << std::hex << address
            << " - this section of RAM is unusable." << std::endl;
    }

    else if (address < IO_REGISTERS_UPPER) {
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
            case 0xFF40:
                ppu->lcdc = value;
            case 0xFF41:
                ppu->set_stat(value);
            case 0xFF42:
                ppu->scy = value;
            case 0xFF43:
                ppu->scx = value;
            case 0xFF44:
                std::cerr << "Invalid MMU write at address: " << std::hex
                    << address << "- LY register is read-only."
                    << std::endl;
            case 0xFF45:
                ppu->lyc = value;
            case 0xFF46:
                dma->set_source_address(value);
            case 0xFF47:
                ppu->bgp = value;
            case 0xFF48:
                ppu->obp0 = value;
            case 0xFF49:
                ppu->obp1 = value;
            case 0xFF4A:
                ppu->wy = value;
            case 0xFF4B:
                ppu->wx = value;
            default:
                std::cerr << "Invalid MMU write at address " << std::hex
                    << address << " - this I/O register does not exist."
                    << std::endl;
        }
    }

    else if (address < HRAM_UPPER) {
        hram[address - IO_REGISTERS_UPPER] = value;
    }

    else {
        interrupt_manager->ie = value;
    }
}