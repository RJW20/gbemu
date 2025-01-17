#include <iostream>
#include <cstdint>
#include <utility>
#include "ppu.hpp"
#include "../interrupt_manager.hpp"
#include "fetcher.hpp"

/* Clear VRAM, OAM and the pixel_buffer.
 * Set the registers to their power-on values. 
 * Set mode to VBLANK. */
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

    set_mode(Mode::OAM_SCAN);
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
 * Resets the appropriate variables in preparation for the new mode. */
void Ppu::set_mode(Mode new_mode) {
    
    mode = new_mode;
    switch (mode) {

        case Mode::OAM_SCAN:
            current_t_cycles = 0;
            oam_scan_index = 0;
            scanline_object_indexes.resize(0);
            pixels_to_discard = scx & 0x7;
            window_possible_on_scanline = (_ly >= wy);
            window_visible_on_scanline = false;
            break;

        case Mode::PIXEL_TRANSFER:
            current_t_cycles = 0;
            lx = 0;
            set_fetcher_source(FetcherSource::BACKGROUND);
            bgwin_fifo.clear();
            object_fifo.clear();
            break;

        case Mode::VBLANK:
            current_t_cycles = 0;
            fetcher.wly = 0;
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

    switch(fetcher.source) {

        case FetcherSource::BACKGROUND:
            if (window_enabled() && window_possible_on_scanline &&
                lx >= wx - 7) {
                set_fetcher_source(FetcherSource::WINDOW);
                bgwin_fifo.clear();
            }
            break;

        case FetcherSource::WINDOW:
            if (!window_enabled()) {
                set_fetcher_source(FetcherSource::BACKGROUND);
                bgwin_fifo.clear();
            }
            break;
    }

    // Push fetcher.row_buffer pixels to fifo if space ---- turn into method
    if (fetcher.mode == FetcherMode::PUSH &&
        bgwin_fifo.is_accepting_pixels()) {

        // Interleave the row_buffer bits to make colour IDs
        uint16_t pixel_row = 0;
        for (int i = 0; i < 8; i++) {
            uint16_t lsb = (fetcher.row_buffer >> (i + 8)) & 1;
            uint16_t msb = (fetcher.row_buffer >> i) & 1;
            pixel_row |= (lsb << (2 * i));
            pixel_row |= (msb << (2 * i + 1));
        }

        bgwin_fifo.accept_pixels(pixel_row);
        fetcher.x++;
        restart_fetcher();
    }

    fetcher_tick();
    
    push_pixel();
}

/* Set the fetcher source to the given source. 
 * Resets the appropriate variables in preparation for the new source. */
void Ppu::set_fetcher_source(FetcherSource new_source) {

    fetcher.source = new_source;
    restart_fetcher();
    switch(fetcher.source) {
        
        case FetcherSource::BACKGROUND:
            fetcher.x = lx >> 3;
            break;

        case FetcherSource::WINDOW:
            fetcher.x = (lx - (wx - 7)) >> 3;
            window_visible_on_scanline = true;
            break;        
    }
}

/* Prepare the fetcher to fetch a new tile.
 * Sets the fetcher mode to TILE_ID and resets the fetcher cycles to 0. */
void Ppu::restart_fetcher() {
    fetcher.mode = FetcherMode::TILE_ID;
    fetcher.cycles = 0;
}

// Carry out 1 fetch t-cycle.
void Ppu::fetcher_tick() {

    switch(fetcher.mode) {

        case FetcherMode::TILE_ID:

            if (fetcher.cycles++ & 0x1) { // Get tile_id in cycle 2

                switch(fetcher.source) {

                    case FetcherSource::BACKGROUND:
                        fetcher.current_tile_id = fetch_background_tile_id();
                        break;

                    case FetcherSource::WINDOW:
                        fetcher.current_tile_id = fetch_window_tile_id();
                        break;
                }

                fetcher.mode = FetcherMode::TILE_ROW_LOW;
            }
            break;

        case FetcherMode::TILE_ROW_LOW:
            
            if (fetcher.cycles++ & 0x1) { // Get row_buffer 1st byte in cycle 4

                uint8_t tile_row;
                switch(fetcher.source) {

                    case FetcherSource::BACKGROUND:
                        tile_row = (scy + _ly) & 0x7;
                        break;

                    case FetcherSource::WINDOW:
                        tile_row = fetcher.wly & 0x7;
                        break;
                }

                fetcher.row_buffer = fetch_tile_row(
                    fetcher.current_tile_id,
                    tile_row, 
                    true
                ) << 8;
                fetcher.mode = FetcherMode::TILE_ROW_HIGH;
            }
            break;

        case FetcherMode::TILE_ROW_HIGH:
            
            if (fetcher.cycles++ & 0x1) { // Get row_buffer 2nd byte in cycle 6

                uint8_t tile_row;
                switch(fetcher.source) {

                    case FetcherSource::BACKGROUND:
                        tile_row = (scy + _ly) & 0x7;
                        break;

                    case FetcherSource::WINDOW:
                        tile_row = fetcher.wly & 0x7;
                        break;
                }

                fetcher.row_buffer &= fetch_tile_row(
                    fetcher.current_tile_id,
                    tile_row,
                    false
                );
                fetcher.mode = FetcherMode::PUSH;
            }
            break;
    }
}

/* Push a pixel to the pixel_buffer if the FIFO is full enough or discard it if
 * necessary.
 * Increments lx and sets mode to HBLANK if reached end of screen. */
void Ppu::push_pixel() {

    if (!bgwin_fifo.is_shifting_pixels()) {
        return;
    }

    if (pixels_to_discard) {
        bgwin_fifo.shift_pixel();
        pixels_to_discard -= 1;
        return;
    }

    pixel_buffer[_ly * SCREEN_WIDTH + lx] = palette.at(bgwin_fifo.shift_pixel());
    if (++lx == SCREEN_WIDTH) {
        set_mode(Mode::HBLANK);
    }
}

/* Return the tile ID of the tile at the current X, Y position on the
 * background map.
 * Coordinates larger than 31 will wrap around. */
uint8_t Ppu::fetch_background_tile_id() const {
    uint8_t tile_x = ((scx >> 3) + fetcher.x) & 0x1F;
    uint8_t tile_y = ((scy + _ly) >> 3) & 0x1F; // = ((scy + _ly) & 0xFF) >> 3
    return vram[background_tile_map_address() + (tile_y << 5) + tile_x];
}

/* Return the tile ID of the tile at the current X, Y position on the window
 * map. */
uint8_t Ppu::fetch_window_tile_id() const {
    uint8_t tile_y = fetcher.wly >> 3;
    return vram[window_tile_map_address() + (tile_y << 5) + fetcher.x];
}

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
            fetcher.wly += window_visible_on_scanline;
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
            _ly = 0;
            set_mode(Mode::OAM_SCAN);
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