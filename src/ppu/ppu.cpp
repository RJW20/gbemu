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
 * Set the PPU mode to VBLANK, with OAM_SCAN immediately pending. */
void Ppu::reset() {
    vram.fill(0);
    oam.fill(0);
    pixel_buffer.fill(palette.at(0));

    lcdc_ = 0x91;
    ly_ = 0x99;
    lyc = 0;
    stat_ = 0x85;
    scy = 0;
    scx = 0;
    wy = 0;
    wx = 0;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    fetcher.wly = 0;

    mode = Mode::VBLANK;
    scanline_t_cycles = SCANLINE_T_CYCLES;
    mode_change = true;
    pending_mode = Mode::OAM_SCAN;
}

// Carry out 1 t-cycle.
void Ppu::tick() {

    if (!lcd_enabled()) {
        return;
    }

    if (mode_change) {
        set_mode(pending_mode);
        mode_change = false;
    }

    (this->*mode_tick)();

    Log::debug(*this);
}

/* Set the PPU mode to the given Mode. 
 * Resets the appropriate variables in preparation for the new Mode.
 * Sets the mode_tick pointer to the correct method.
 * Requests a STAT interrupt if necessary. */
void Ppu::set_mode(Mode new_mode) {
    
    mode = new_mode;
    switch (mode) {

        case Mode::OAM_SCAN:
            new_oam_scan();
            mode_tick = &Ppu::oam_scan_tick;
            if ((ly_ == lyc) && lyc_interrupt_requested() ||
                oam_scan_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }
            break;

        case Mode::PIXEL_TRANSFER:
            new_pixel_transfer();
            mode_tick = &Ppu::pixel_transfer_tick;
            break;

        case Mode::HBLANK:
            mode_tick = &Ppu::hblank_tick;
            if (hblank_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }
            break;

        case Mode::VBLANK:
            scanline_t_cycles = 0;
            mode_tick = &Ppu::vblank_tick;
            interrupt_manager->request(InterruptType::VBLANK);
            if (vblank_interrupt_requested() ||
                oam_scan_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }
            break;
    }
}

/* Carry out 1 OAM_SCAN t-cycle.
 * Sorts the identified scanline objects if at the end of OAM_SCAN and sets the
 * pending PPU mode to PIXEL_TRANSFER. */
void Ppu::oam_scan_tick() {

    OamScanner::oam_scan_tick();

    if (++scanline_t_cycles < OAM_T_CYCLES) {
        return;
    }

    std::stable_sort(
        scanline_objects.begin(),
        scanline_objects.end(),
        [](const OamObject& a, const OamObject& b) {
            return a.x < b.x;
        }
    );

    mode_change = true;
    pending_mode = Mode::PIXEL_TRANSFER;
}

/* Carry out 1 PIXEL_TRANSFER t-cycle.
 * Sets the pending PPU mode to HBLANK if a whole scanline has been
 * rendered. */
void Ppu::pixel_transfer_tick() {

    scanline_t_cycles++;

    PixelTransferrer::pixel_transfer_tick();

    if (lx == SCREEN_WIDTH) {
        fetcher.wly += window_visible_on_scanline;
        mode_change = true;
        pending_mode = Mode::HBLANK;
    }
}

/* Carry out 1 HBLANK t-cycle.
 * Sets the pending PPU mode to OAM_SCAN or VBLANK if at the end of a scanline,
 * depending on the value of ly_. */
void Ppu::hblank_tick() {

    if (++scanline_t_cycles < SCANLINE_T_CYCLES) {
        return;
    }

    mode_change = true;
    pending_mode = ly_ < SCREEN_HEIGHT - 1 ? Mode::OAM_SCAN : Mode::VBLANK;
}

/* Carry out 1 VBLANK t-cycle.
 * Increments ly_ if at the start of a scanline and requests a STAT interrupt
 * if necessary.
 * Sets the pending PPU mode to OAM_SCAN if 10 VBLANK scanlines have been
 * completed. */
void Ppu::vblank_tick() {

    if (!scanline_t_cycles && (++ly_ == lyc) && lyc_interrupt_requested()) {
        interrupt_manager->request(InterruptType::STAT);
    }
    
    if (++scanline_t_cycles < SCANLINE_T_CYCLES) {
        return;
    }
    
    if (ly_ < SCREEN_HEIGHT + VBLANK_SCANLINES - 1) {
        scanline_t_cycles = 0;
    }
    else {
        ready_to_render = true;
        fetcher.wly = 0;
        mode_change = true;
        pending_mode = Mode::OAM_SCAN;
    }
}

/* Return ly_. 
 * On the final scanline of VBLANK, for t-cycles 5-456 ly reads as 0. */
uint8_t Ppu::ly() const {
    if (ly_ == SCREEN_HEIGHT + VBLANK_SCANLINES - 1 &&
        mode == Mode::VBLANK && scanline_t_cycles > 4) {
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
        << " LX = " << static_cast<int>(lx)
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