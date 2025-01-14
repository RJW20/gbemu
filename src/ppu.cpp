#include <iostream>
#include <cstdint>
#include <utility>
#include "ppu.hpp"
#include "interrupt_manager.hpp"

/* Clear VRAM, OAM and the pixel_buffer.
 * Set the registers to their power-on values. 
 * Set mode to Mode::VBLANK. */
void Ppu::reset() {
    vram.fill(0);
    oam.fill(0);
    pixel_buffer.fill(0);            //pixel_buffer.fill(palette.at(0));

    lcdc = 0x91;
    _ly = 0;
    lyc = 0;
    _stat_ = 0x80;
    scy = 0;
    scx = 0;
    wy = 0;
    wx = 0;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    set_mode(Mode::OAM_SCAN);       // set_mode(Mode::VBLANK) and set current_t_cycles high?
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
 * Resets all variables in preparation for the new mode. */
void Ppu::set_mode(Mode new_mode) {
    
    mode = new_mode;
    switch (mode) {

        case Mode::OAM_SCAN:
            current_t_cycles = 0;
            //oam_scan_index = 0;
            //scanline_object_indexes.resize(0);
            pixels_to_discard = scx & 0x7;
            break;

        case Mode::PIXEL_TRANSFER:
            current_t_cycles = 0;
            //pixel_fetcher_source = PixelFetcherSource::BACKGROUND;
            reset_fetcher();
            lx = 0;
            current_scanline_tile = 0;
            bgwin_fifo = 0;
            //sprite_fifo = 0;
            bgwin_fifo_pointer = 0;
            //sprite_fifo_pointer = 0;
            break;

        case Mode::VBLANK:
            current_t_cycles = 0;
            break;
    }
}

/* Carry out 1 OAM_SCAN t-cycle.
 * Reads 2 bytes from the OAM, . 
 * */
void Ppu::oam_scan_tick() {

    // Reads objects as off screen if DMA is active

    // Fully identify an object every 2nd call

    if (lyc_interrupt_requested() && (_ly == lyc) ||
        oam_scan_interrupt_requested()) {
        interrupt_manager->request(InterruptType::STAT);
    }

    if (++current_t_cycles == OAM_T_CYCLES) {
        set_mode(Mode::PIXEL_TRANSFER);
    }
}

/* Carry out 1 PIXEL_TRANSFER t-cycle. 
 * */
void Ppu::pixel_transfer_tick() {

    current_t_cycles++;
    fetcher_cycles++;
    
    // Push tile_row pixels to fifo if space
    if (pixel_fetcher_mode == PixelFetcherMode::PUSH &&
        bgwin_fifo_pointer < 9) {

        // Interleave the tile_row bits to make colour IDs
        uint16_t pixel_row = 0;
        for (int i = 0; i < 8; i++) {
            uint16_t lsb = (tile_row >> (i + 8)) & 1;
            uint16_t msb = (tile_row >> i) & 1;
            pixel_row |= (lsb << (2 * i));
            pixel_row |= (msb << (2 * i + 1));
        }

        // Push to fifo
        bgwin_fifo &= (pixel_row << (8 - bgwin_fifo_pointer) * 2);
        bgwin_fifo_pointer += 8;

        // Discard pixels if first tile
        if (!current_scanline_tile++) {
            bgwin_fifo <<= (2 * pixels_to_discard);
            bgwin_fifo_pointer -= pixels_to_discard;
        }

        reset_fetcher();
    }

    switch(pixel_fetcher_mode) {

        case PixelFetcherMode::TILE_ID:
            // Get tile_id during 2nd fetcher_cycle
            if (fetcher_cycles & 0x1) {
                tile_id = fetch_background_tile_id(current_scanline_tile++);
                pixel_fetcher_mode = PixelFetcherMode::TILE_ROW_LOW;
            }
            break;

        case PixelFetcherMode::TILE_ROW_LOW:
            // Get leading byte of tile_row during 4th fetcher_cycle
            if (fetcher_cycles & 0x1) {
                uint8_t row = (scy + _ly) & 0x7;
                tile_row = fetch_tile_row(tile_id, row, true) << 8;
                pixel_fetcher_mode = PixelFetcherMode::TILE_ROW_HIGH;
            }
            break;

        case PixelFetcherMode::TILE_ROW_HIGH:
            // Get trailing byte of tile_row during 6th fetcher_cycle
            if (fetcher_cycles & 0x1) {
                uint8_t row = (scy + _ly) & 0x7;
                tile_row = tile_row & (tile_id, row, false);
                pixel_fetcher_mode = PixelFetcherMode::PUSH;
            }
            break;
    }

    // Push a pixel to pixel_buffer if fifo full enough
    if (bgwin_fifo_pointer > 8) {

        pixel_buffer[_ly * SCREEN_WIDTH + lx] = palette.at(bgwin_fifo >> 30);
        bgwin_fifo <<= 2;
        bgwin_fifo_pointer -= 1;

        // Change to HBLANK if reached end of scanline
        if (++lx == SCREEN_WIDTH) {
            set_mode(Mode::HBLANK);
        }
    }
}

/* Reset the pixel fetcher mode to TILE_ID. 
 * Resets fetcher_cycles.0. */
void Ppu::reset_fetcher() {
    pixel_fetcher_mode = PixelFetcherMode::TILE_ID;
    fetcher_cycles = 0;
}

/* Return the tile ID of the background tile at (scx/8+x_offset,(scy+_ly)/8).
 * Coordinates larger than 31 will wrap around. */
uint8_t Ppu::fetch_background_tile_id(uint8_t x_offset) const {
    uint8_t tile_x = (scx & 0xF8) + x_offset;
    uint8_t tile_y = (scy + _ly) & 0xF8;
    return vram[background_tile_map_address() + (tile_y << 5) + tile_x];
}

/* Return the tile ID of the window tile at (x-(wx-7),_ly-wy). 
uint8_t Ppu::fetch_window_tile_id() {
    uint8_t tile_x = (x - (wx - 7)) / 8;
    uint8_t tile_y = (_ly - wy) / 8;
    return vram[window_tile_map_address() + (tile_y << 5) + tile_x];
}
*/

/* Fetch half of the given row of the tile with given tile_id.
 * Fetches the first byte if the given boolean is true. */
uint8_t Ppu::fetch_tile_row(uint8_t tile_id, uint8_t row, bool low) const {
    uint16_t start_of_tile_address = bgwin_unsigned_addressing() ? 
        tile_id * 16 : 0x1000 + static_cast<int8_t>(tile_id) * 16;
    return low ? vram[start_of_tile_address + 2 * row] :
        vram[start_of_tile_address + 2 * row + 1];
}

/* Carry out 1 HBLANK t-cycle.
 * Checks for relevant STAT interrupts.
 * Increments _ly and sets mode to VBLANK or OAM_SCAN at the end of the
 * scanline. */
void Ppu::hblank_tick() {

    if (lyc_interrupt_requested() && (_ly == lyc) ||
        hblank_interrupt_requested()) {
        interrupt_manager->request(InterruptType::STAT);
    }

    if (++current_t_cycles == SCANLINE_T_CYCLES) {
        if (++_ly== SCREEN_HEIGHT) {
            set_mode(Mode::VBLANK);
        }
        else {
            set_mode(Mode::OAM_SCAN);
        }
    }
}

/* Carry out 1 VBLANK t-cycle.
 * Requests VBLANK interrupt if the first t-cycle.
 * Checks for relevant STAT interrupts.
 * Increments _ly and sets to OAM_SCAN at the end of the 10 scanlines. */
void Ppu::vblank_tick() {

    if (current_t_cycles == 0 && _ly == SCREEN_HEIGHT) {
        interrupt_manager->request(InterruptType::VBLANK);
    }

    if (lyc_interrupt_requested() && (_ly == lyc) ||
        vblank_interrupt_requested()) {
        interrupt_manager->request(InterruptType::STAT);
    }

    if (++current_t_cycles == SCANLINE_T_CYCLES) {
        if (++_ly == SCREEN_HEIGHT + VBLANK_SCANLINES) {
            set_mode(Mode::OAM_SCAN);
            _ly = 0;
        }
        else {
            current_t_cycles = 0;
        }
    }
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

// Return true if bit 6 of _stat_ is set.
bool Ppu::lyc_interrupt_requested() const {
    return (_stat_ >> 6) & 1;
}

// Return true if bit 5 of _stat_ is set.
bool Ppu::oam_scan_interrupt_requested() const {
    return (_stat_ >> 5) & 1;
}

// Return true if bit 4 of _stat_ is set.
bool Ppu::vblank_interrupt_requested() const {
    return (_stat_ >> 4) & 1;
}

// Return true if bit 3 of _stat_ is set.
bool Ppu::hblank_interrupt_requested() const {
    return (_stat_ >> 3) & 1;
}

// Return _ly.
uint8_t Ppu::ly() const {
    return _ly;
}

/* Return the LCD status.
 * Bits 7-3 are the leading 5 bits of _stat_.
 * Bit 2 is set if _ly = lyc or clear otherwise.
 * Bits 1-0 represent the current mode. */
uint8_t Ppu::stat() const {
    return (_stat_ & 0xF8) | ((_ly == lyc) << 2) | std::to_underlying(mode);
}

/* Set bits 7-3 of _stat_ to the leading 5 bits of the given value.
 * Outputs to std::cerr if the trailing 3 bits in the given value are non-
 * zero. */
void Ppu::set_stat(uint8_t value) {
    if (value & 0x7) {
        std::cerr << "The trailing 3 bits in STAT must always be zero, "
            << "ignoring them in value: " << std::hex << (uint16_t) value
            << std::endl;
    }
    _stat_ = value & 0xF8;
}

/* Return the 8 bit value in VRAM at the given address.
 * Returns 0xFF if the address is out of boudns. */
uint8_t Ppu::read_vram(uint16_t address) const {

    if (address >= VRAM_SIZE) {
        std::cerr << "Invalid PPU VRAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return vram[address];
}

/* Write the given 8 bit value to the VRAM at the given address.
 * Fails if the address is out of bounds. */
void Ppu::write_vram(uint16_t address, uint8_t value) {

    if (address >= VRAM_SIZE) {
        std::cerr << "Invalid PPU VRAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    vram[address] = value;
}

/* Return the 8 bit value in OAM at the given address.
 * Returns 0xFF if the address is out of bounds. */
uint8_t Ppu::read_oam(uint16_t address) const {
    
    if (address >= OAM_SIZE) {
        std::cerr << "Invalid PPU OAM read at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return 0xFF;
    }

    return vram[address];
}

/* Write the given 8 bit value to the OAM at the given address.
 * Fails if the address is out of bounds. */
void Ppu::write_oam(uint16_t address, uint8_t value) {

    if (address >= OAM_SIZE) {
        std::cerr << "Invalid PPU OAM write at address " << std::hex
            << address << " - out of bounds." << std::endl;
        return;
    }

    oam[address] = value;
}