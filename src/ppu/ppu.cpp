#include <iostream>
#include <cstdint>
#include <algorithm>
#include <utility>
#include "ppu.hpp"
#include "oam_scanner/oam_object.hpp"
#include "../interrupt_manager.hpp"

/* Clear VRAM, OAM and the pixel_buffer.
 * Set the registers to their power-on values. 
 * Set mode to VBLANK. */
void Ppu::reset() {
    vram.fill(0);
    oam.fill(0);
    pixel_buffer.fill(0);            //pixel_buffer.fill(palette.at(0));

    lcdc = 0x91;
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

            if (++current_t_cycles == SCANLINE_T_CYCLES) {
                if (++ly_== SCREEN_HEIGHT) {
                    set_mode(Mode::VBLANK);
                }
                else {
                    set_mode(Mode::OAM_SCAN);
                }
            }
            break;

        case Mode::VBLANK:
            if (current_t_cycles == 0 && ly_ == SCREEN_HEIGHT) {
                interrupt_manager->request(InterruptType::VBLANK);
            }

            if (lyc_interrupt_requested() && (ly_ == lyc) ||
                vblank_interrupt_requested()) {
                interrupt_manager->request(InterruptType::STAT);
            }

            if (++current_t_cycles == SCANLINE_T_CYCLES) {
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

/* Set the Ppu mode to the given mode. 
 * Resets the appropriate variables in preparation for the new mode. */
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

/* Set bits 7-3 of stat_ to the leading 5 bits of the given value.
 * Outputs to std::cerr if the trailing 3 bits in the given value are non-
 * zero. */
void Ppu::set_stat(uint8_t value) {
    if (value & 0x7) {
        std::cerr << "The trailing 3 bits in STAT must always be zero, "
            << "ignoring them in value: " << std::hex << (uint16_t) value
            << std::endl;
    }
    stat_ = value & 0xF8;
}