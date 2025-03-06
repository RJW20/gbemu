#include <cstdint>
#include <string>
#include <sstream>
#include "logger.hpp"
#include "mmu.hpp"
#include "exceptions.hpp"

// Clear WRAM and HRAM.
void Mmu::reset() {
    wram.fill(0);
    hram.fill(0);
}

/* Return the 8 bit value stored at the given address.
 * Returns 0xFF if the value cannot be read. */
uint8_t Mmu::read(uint16_t address) const {

    uint8_t value;
    try {
        value = read_value(address);
    }
    catch (MemoryAccessException& e) {
        Log::warning([e]{ return e.what(); });
        value = 0xFF;
    }

    Log::debug(
        [this, address, value] {
            return read_write_message(address, value, true);
        }
    );
    return value;
}

/* Write the given 8 bit value to the given address.
 * Does nothing if the address cannot be written to. */
void Mmu::write(uint16_t address, uint8_t value) {
    try {
        write_value(address, value);
        Log::debug(
            [this, address, value] {
                return read_write_message(address, value, false);
            }
        );
    }
    catch (MemoryAccessException& e) {
        Log::warning([e]{ return e.what(); });
    }
}

/* Return the 8 bit value stored at the given address.
 * Directs the address to its corresponding component.
 * Throws a MemoryAccessException if the address has no corresponding
 * component. */
uint8_t Mmu::read_value(uint16_t address) const {

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
        return wram[address - WRAM_UPPER];
    }

    else if (address < OAM_UPPER) {
        if (dma->transfer_in_progress) {
            throw MemoryAccessException(
                "OAM", "not readable during DMA", address, true
            );
        }
        return ppu->read_oam(address - ECHO_RAM_UPPER);
    }

    else if (address < UNUSABLE_MEMORY_UPPER) {
        throw MemoryAccessException(
            "MMU", "this section of RAM is unusable", address, true
        );
    }

    else if (address < IO_REGISTERS_UPPER) {
        switch (address) {
            case 0xFF00:
                return joypad->joyp();
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
                return interrupt_manager->_if();
            case 0xFF10:
                return apu->nr10();
            case 0xFF11:
                return apu->nr11();
            case 0xFF12:
                return apu->nr12();
            case 0xFF13:
                return apu->nr13();
            case 0xFF14:
                return apu->nr14();
            case 0xFF16:
                return apu->nr21();
            case 0xFF17:
                return apu->nr22();
            case 0xFF18:
                return apu->nr23();
            case 0xFF19:
                return apu->nr24();
            case 0xFF1A:
                return apu->nr30();
            case 0xFF1B:
                return apu->nr31();
            case 0xFF1C:
                return apu->nr32();
            case 0xFF1D:
                return apu->nr33();
            case 0xFF1E:
                return apu->nr34();
            case 0xFF20:
                return apu->nr41();
            case 0xFF21:
                return apu->nr42();
            case 0xFF22:
                return apu->nr43();
            case 0xFF23:
                return apu->nr44();
            case 0xFF24:
                return apu->nr50();
            case 0xFF25:
                return apu->nr51();
            case 0xFF26:
                return apu->nr52();
            case 0xFF30:
            case 0xFF31:
            case 0xFF32:
            case 0xFF33:
            case 0xFF34:
            case 0xFF35:
            case 0xFF36:
            case 0xFF37:
            case 0xFF38:
            case 0xFF39:
            case 0xFF3A:
            case 0xFF3B:
            case 0xFF3C:
            case 0xFF3D:
            case 0xFF3E:
            case 0xFF3F:
                return apu->read_wave_ram(address - 0xFF30);
            case 0xFF40:
                return ppu->lcdc();
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
                throw MemoryAccessException(
                    "MMU I/O register", "this register does not exist",
                    address, true
                );
        }
    }

    else if (address < HRAM_UPPER) {
        return hram[address - IO_REGISTERS_UPPER];
    }

    else {
        return interrupt_manager->ie();
    }
}

/* Write the given 8 bit value to the given address.
 * Directs the address to its corresponding component.
 * Throws a MemoryAccessException if the address has no corresponding
 * component. */
void Mmu::write_value(uint16_t address, const uint8_t value) {

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
        wram[address - WRAM_UPPER] = value;
    }

    else if (address < OAM_UPPER) {
        if (dma->transfer_in_progress) {
            throw MemoryAccessException(
                "OAM", "not writable during DMA", address, false
            );
        }
        ppu->write_oam(address - ECHO_RAM_UPPER, value);
    }

    else if (address < UNUSABLE_MEMORY_UPPER) {
        throw MemoryAccessException(
            "MMU", "this section of RAM is unusable", address, false
        );
    }

    else if (address < IO_REGISTERS_UPPER) {
        switch (address) {
            case 0xFF00:
                joypad->set_joyp(value);
                break;
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
                interrupt_manager->set_if(value);
                break;
            case 0xFF10:
                apu->set_nr10(value);
                break;
            case 0xFF11:
                apu->set_nr11(value);
                break;
            case 0xFF12:
                apu->set_nr12(value);
                break;
            case 0xFF13:
                apu->set_nr13(value);
                break;
            case 0xFF14:
                apu->set_nr14(value);
                break;
            case 0xFF16:
                apu->set_nr21(value);
                break;
            case 0xFF17:
                apu->set_nr22(value);
                break;
            case 0xFF18:
                apu->set_nr23(value);
                break;
            case 0xFF19:
                apu->set_nr24(value);
                break;
            case 0xFF1A:
                apu->set_nr30(value);
                break;
            case 0xFF1B:
                apu->set_nr31(value);
                break;
            case 0xFF1C:
                apu->set_nr32(value);
                break;
            case 0xFF1D:
                apu->set_nr33(value);
                break;
            case 0xFF1E:
                apu->set_nr34(value);
                break;
            case 0xFF20:
                apu->set_nr41(value);
                break;
            case 0xFF21:
                apu->set_nr42(value);
                break;
            case 0xFF22:
                apu->set_nr43(value);
                break;
            case 0xFF23:
                apu->set_nr44(value);
                break;
            case 0xFF24:
                apu->set_nr50(value);
                break;
            case 0xFF25:
                apu->set_nr51(value);
                break;
            case 0xFF26:
                apu->set_nr52(value);
                break;
            case 0xFF30:
            case 0xFF31:
            case 0xFF32:
            case 0xFF33:
            case 0xFF34:
            case 0xFF35:
            case 0xFF36:
            case 0xFF37:
            case 0xFF38:
            case 0xFF39:
            case 0xFF3A:
            case 0xFF3B:
            case 0xFF3C:
            case 0xFF3D:
            case 0xFF3E:
            case 0xFF3F:
                apu->write_wave_ram(address - 0xFF30, value);
                break;
            case 0xFF40:
                ppu->set_lcdc(value);
                break;
            case 0xFF41:
                ppu->set_stat(value);
                break;
            case 0xFF42:
                ppu->scy = value;
                break;
            case 0xFF43:
                ppu->scx = value;
                break;
            case 0xFF44:
                ppu->set_ly(value);
                break;
            case 0xFF45:
                ppu->lyc = value;
                break;
            case 0xFF46:
                dma->set_source_address(value);
                break;
            case 0xFF47:
                ppu->bgp = value;
                break;
            case 0xFF48:
                ppu->obp0 = value;
                break;
            case 0xFF49:
                ppu->obp1 = value;
                break;
            case 0xFF4A:
                ppu->wy = value;
                break;
            case 0xFF4B:
                ppu->wx = value;
                break;
            default:
                throw MemoryAccessException(
                    "MMU I/O register", "this register does not exist",
                    address, false
                );
        }
    }

    else if (address < HRAM_UPPER) {
        hram[address - IO_REGISTERS_UPPER] = value;
    }

    else {
        interrupt_manager->set_ie(value);
    }
}

// Generate an MMU read or write debug message.
std::string Mmu::read_write_message(
    uint16_t address,
    uint8_t value,
    bool read
) const {
    std::ostringstream msg;
    msg << "MMU: ";
    if (read) {
        msg << "Read " << std::hex << static_cast<int>(value) << " from "
            << address;
    }
    else {
        msg << "Write " << std::hex << static_cast<int>(value) << " to "
            << address;
    }
    return msg.str();
}