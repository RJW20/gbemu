#include <cstdint>
#include <algorithm>
#include <utility>
#include <string>
#include <sstream>
#include "../logger.hpp"
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
    stat_ = 0x85;
    scy = 0;
    scx = 0;
    wy = 0;
    wx = 0;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    set_mode(Mode::VBLANK);
}

// Carry out 1 t-cycle.
void Ppu::tick() {

    if (!lcd_enabled()) {
        return;
    }

    switch(mode) {

        case Mode::OAM_SCAN:
            if (!mode_t_cycles++) {
                if (oam_scan_interrupt_requested() ||
                    (ly_ == lyc) && lyc_interrupt_requested()) {
                    interrupt_manager->request(InterruptType::STAT);
                }
            }

            oam_scan_tick();

            if (mode_t_cycles == OAM_T_CYCLES) {
                std::stable_sort(
                    scanline_objects.begin(),
                    scanline_objects.end(),
                    [](const OamObject& a, const OamObject& b) {
                        return a.x < b.x;
                    }
                );
                set_mode(Mode::PIXEL_TRANSFER);
            }

            scanline_t_cycles++;
            break;

        case Mode::PIXEL_TRANSFER:
            mode_t_cycles++;

            pixel_transfer_tick();

            if (lx == SCREEN_WIDTH) {
                fetcher.wly += window_visible_on_scanline;
                set_mode(Mode::HBLANK);
            }

            scanline_t_cycles++;
            break;

        case Mode::HBLANK:
            if (!mode_t_cycles++ && hblank_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (++scanline_t_cycles == SCANLINE_T_CYCLES) {
                if (++ly_ == SCREEN_HEIGHT) {
                    set_mode(Mode::VBLANK); 
                }
                else {
                    set_mode(Mode::OAM_SCAN);
                }
            }
            break;

        case Mode::VBLANK:
            if (!mode_t_cycles++) {
                interrupt_manager->request(InterruptType::VBLANK);
                if (vblank_interrupt_requested()) {
                    interrupt_manager->request(InterruptType::STAT);
                }
            }

            if (!scanline_t_cycles++ &&
                (ly_ == lyc) && lyc_interrupt_requested()) {
                    interrupt_manager->request(InterruptType::STAT);
                }
            else if (scanline_t_cycles == SCANLINE_T_CYCLES) {
                // !ly_ since first scanline after the boot ROM is VBLANK with ly_ = 0
                if (!ly_ || ++ly_ == SCREEN_HEIGHT + VBLANK_SCANLINES) {
                    set_mode(Mode::OAM_SCAN);
                    ly_ = 0;
                }
                else {
                    scanline_t_cycles = 0;
                }
            }
            break;
    }

    Log::debug(*this);
}

/* Set the Ppu mode to the given Mode. 
 * Resets the appropriate variables in preparation for the new Mode.
 * Requests a STAT interrupt for the new Mode if necessary. */
void Ppu::set_mode(Mode new_mode) {
    
    mode = new_mode;
    mode_t_cycles = 0;
    switch (mode) {

        case Mode::OAM_SCAN:
            new_oam_scan();
            break;

        case Mode::PIXEL_TRANSFER:
            new_pixel_transfer();
            break;

        case Mode::VBLANK:
            scanline_t_cycles = 0;
            fetcher.wly = 0;
            break;
    }
}

/* Return ly_. 
 * On the final scanline of VBLANK, for t-cycles 4-456 ly reads as 0. */
uint8_t Ppu::ly() const {
    if (ly_ == SCREEN_HEIGHT + VBLANK_SCANLINES - 1 &&
        mode == Mode::VBLANK && scanline_t_cycles > 3) {
            return 0;
        }
    return ly_;
}

/* Return the LCD status.
 * Bit 7 is always 1.
 * Bits 6-3 are those of stat_.
 * Bit 2 is set if ly_ = lyc or clear otherwise.
 * Bits 1-0 represent the current mode. */
uint8_t Ppu::stat() const {
    return 0x80 | (stat_ & 0x78) | ((ly_ == lyc) << 2) |
        std::to_underlying(mode);
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
    repr << "PPU:" << " ticks = " << scanline_t_cycles << std::hex
        << " LCDC = " << static_cast<int>(lcdc())
        << " LY = " << static_cast<int>(ly())
        << " LYC = " << static_cast<int>(lyc)
        << " STAT = " << static_cast<int>(stat())
        << " SCY = " << static_cast<int>(scy)
        << " SCX = " << static_cast<int>(scx)
        << " WY = " << static_cast<int>(wy)
        << " WX = " << static_cast<int>(wx)
        << " BGP = " << static_cast<int>(bgp)
        << " OBP0 = " << static_cast<int>(obp0)
        << " OBP1 = " << static_cast<int>(obp1);
    return repr.str();
}