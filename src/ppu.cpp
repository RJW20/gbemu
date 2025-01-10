#include <iostream>
#include <utility>
#include "ppu.hpp"

/* Resize and clear the memory vectors.
 * Set the registers to their default values. 
 * Set mode to Mode::VBLANK. */
void Ppu::reset() {
    vram.resize(VRAM_SIZE);
    oam.resize(OAM_SIZE);

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

    set_mode(Mode::VBLANK);
}

// Carry out 1 t-cycle.
void Ppu::tick() {

    if (!lcd_enabled()) {
        return;
    }

    current_t_cycles += 1;

    switch(mode) {

        case Mode::OAM_SCAN:
            oam_scan_tick();
            break;

        case Mode::PIXEL_TRANSFER:
            pixel_transfer_tick();
            break;

        case Mode::HBLANK:
            hblank_tick();
            break;

        case Mode::VBLANK:
            vblank_tick();
            break;
    }
}

/* Set the Ppu mode to the given mode. 
 * */
void Ppu::set_mode(Mode new_mode) {
    mode = new_mode;
    current_t_cycles = 0;
}

//
void Ppu::oam_scan_tick() {

    // Request interrupts in here
}

//
void Ppu::pixel_transfer_tick() {

    // Request interrupts in here
}

//
void Ppu::hblank_tick() {

    // Request interrupts in here
}

//
void Ppu::vblank_tick() {

    // Request interrupts in here
}

// Return ly_.
uint8_t Ppu::ly() const {
    return ly_;
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

// Return true if bit 7 of lcdc is set.
bool Ppu::lcd_enabled() const {
    return (lcdc >> 7) & 1;
}

/* Return the address of tile map 2 if bit 6 of lcdc is set otherwise the
 * address of tile map 1. */
uint16_t Ppu::window_tile_map_address() const {
    return (lcdc >> 6) & 1 ? 0x1C00 : 0x1800;
}

// Return true if bit 5 of lcdc is set.
bool Ppu::window_enabled() const {
    return (lcdc >> 5) & 1;
}

// Return true if bit 4 of lcdc is set.
bool Ppu::bgwin_unsigned_addressing() const {
    return (lcdc >> 4) & 1;
}

/* Return the address of tile map 2 if bit 3 of lcdc is set otherwise the
 * address of tile map 1. */
uint16_t Ppu::background_tile_map_address() const {
    return (lcdc >> 3) & 1 ? 0x1C00 : 0x1800;
}

// Return true if bit 2 of lcdc is set.
bool Ppu::double_object_height() const {
    return (lcdc >> 2) & 1;
}

// Return true if bit 1 of lcdc is set.
bool Ppu::object_enabled() const {
    return (lcdc >> 1) & 1;
}

// Return true if bit 0 of lcdc is set.
bool Ppu::bgwin_enabled() const {
    return lcdc & 1;
}

// Return true if bit 6 of stat_ is set.
bool Ppu::lyc_interrupt_requested() const {
    return (stat_ >> 6) & 1;
}

// Return true if bit 5 of stat_ is set.
bool Ppu::oam_scan_interrupt_requested() const {
    return (stat_ >> 5) & 1;
}

// Return true if bit 4 of stat_ is set.
bool Ppu::vblank_interrupt_requested() const {
    return (stat_ >> 4) & 1;
}

// Return true if bit 3 of stat_ is set.
bool Ppu::hblank_interrupt_requested() const {
    return (stat_ >> 3) & 1;
}