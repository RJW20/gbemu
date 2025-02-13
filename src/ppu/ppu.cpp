#include <cstdint>
#include <algorithm>
#include <utility>
#include <string>
#include <sstream>
#include "ppu.hpp"
#include "oam_scanner/oam_object.hpp"
#include "../interrupt_manager.hpp"
#include "../exceptions.hpp"

/* Clear VRAM, OAM and the pixel_buffer.
 * Set the registers to their post boot ROM values. 
 * Set mode to VBLANK. */
void Ppu::reset() {
    vram.fill(0);
    oam.fill(0);
    pixel_buffer.fill(palette.at(0));

    lcdc_ = 0x91;
    ly_ = 0;
    lyc = 0;
    stat_ = 0x80;
    scy = 0;
    scx = 0;
    wy = 0;
    wx = 0;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    set_mode(Mode::OAM_SCAN);
}

// Carry out 1 t-cycle.
void Ppu::tick() {

    if (!lcd_enabled()) {
        return;
    }

    current_t_cycles++;

    switch(mode) {

        case Mode::OAM_SCAN:
            oam_scan_tick();

            if (lyc_interrupt_requested() && (ly_ == lyc) ||
                oam_scan_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (current_t_cycles == OAM_T_CYCLES) {
                std::stable_sort(
                    scanline_objects.begin(),
                    scanline_objects.end(),
                    [](const OamObject& a, const OamObject& b) {
                        return a.x < b.x;
                    }
                );
                set_mode(Mode::PIXEL_TRANSFER);
            }
            break;

        case Mode::PIXEL_TRANSFER:
            pixel_transfer_tick();

            if (lyc_interrupt_requested() && (ly_ == lyc)) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (lx == SCREEN_WIDTH) {
                fetcher.wly += window_visible_on_scanline;
                set_mode(Mode::HBLANK);
            }
            break;

        case Mode::HBLANK:
            if (lyc_interrupt_requested() && (ly_ == lyc) ||
                hblank_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (current_t_cycles == SCANLINE_T_CYCLES) {
                if (++ly_== SCREEN_HEIGHT) {
                    set_mode(Mode::VBLANK);
                }
                else {
                    set_mode(Mode::OAM_SCAN);
                }
            }
            break;

        case Mode::VBLANK:
            if (current_t_cycles == 1 && ly_ == SCREEN_HEIGHT) {
                interrupt_manager->request(InterruptType::VBLANK);
            }

            if (lyc_interrupt_requested() && (ly_ == lyc) ||
                vblank_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (current_t_cycles == SCANLINE_T_CYCLES) {
                if (++ly_ == SCREEN_HEIGHT + VBLANK_SCANLINES) {
                    ly_ = 0;
                    set_mode(Mode::OAM_SCAN);
                }
                else {
                    current_t_cycles = 0;
                }
            }
            break;
    }
}

/* Set the Ppu mode to the given Mode. 
 * Resets the appropriate variables in preparation for the new Mode. */
void Ppu::set_mode(Mode new_mode) {
    
    mode = new_mode;
    switch (mode) {

        case Mode::OAM_SCAN:
            new_oam_scan();
            break;

        case Mode::PIXEL_TRANSFER:
            new_pixel_transfer();
            break;

        case Mode::VBLANK:
            current_t_cycles = 0;
            fetcher.wly = 0;
            break;
    }
}

/* Return the LCD status.
 * Bits 7-3 are the leading 5 bits of stat_.
 * Bit 2 is set if ly_ = lyc or clear otherwise.
 * Bits 1-0 represent the current mode. */
uint8_t Ppu::stat() const {
    return (stat_ & 0xF8) | ((ly_ == lyc) << 2) | std::to_underlying(mode);
}

// Set lcdc_ to the given value and disable the LCD if needed.
void Ppu::set_lcdc(uint8_t value) {
    lcdc_ = value;
    if (!lcd_enabled()) {
        ly_ = 0;
        set_mode(Mode::OAM_SCAN);
    }
}

// Throws a MemoryAccessException. */
void Ppu::set_ly(uint8_t value) const {
    throw MemoryAccessException(
        "PPU register", "LY is read-only", 0xFF44, false
    );
}

// Return a string representation of the PPU.
std::string Ppu::representation() const {
    std::ostringstream repr;
    repr << "PPU: " << std::hex
        << " LCDC = " << static_cast<int>(lcdc())
        << " LY = " << static_cast<int>(ly())
        << " LYC = " << static_cast<int>(lyc)
        << " STAT = " << static_cast<int>(stat())
        << " SCY = " << static_cast<int>(scy)
        << " SCX = " << static_cast<int>(scx)
        << " WY = " << static_cast<int>(wy)
        << " WX = " << static_cast<int>(wx)
        << " BGP = " << static_cast<int>(bgp)
        << " obp0 = " << static_cast<int>(obp0)
        << " obp1 = " << static_cast<int>(obp1);
    return repr.str();
}