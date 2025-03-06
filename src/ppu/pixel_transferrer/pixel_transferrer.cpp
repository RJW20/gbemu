#include <cstdint>
#include "pixel_transferrer.hpp"
#include "fetcher.hpp"
#include "pixel_fifo.hpp"
#include "../oam_scanner/oam_object.hpp"

// Initialise variables for a new pixel transfer.
void PixelTransferrer::new_pixel_transfer() {
    lx = 0;
    bgwin_fifo.clear();
    object_fifo.clear();
    bgwin_fifo.to_discard = bgwin_pixels_to_discard;
    set_fetcher_source(FetcherSource::BACKGROUND);
    fetcher.wly = 0;
    window_visible_on_scanline = false;
}

/* Carry out 1 PIXEL_TRANSFER t-cycle. 
 * Pushes the current fetcher row buffer to a PixelFifo if possible.
 * Checks the contents at the current (lx, ly_) to keep the fetcher's source
 * accurate.
 * Carries out a fetcher cycle.
 * Shifts a pixel if possible. */
void PixelTransferrer::pixel_transfer_tick() {

    if (fetcher.mode == FetcherMode::PUSH) {
        try_push_fetcher_to_fifo();
    }

    check_fetcher_source();

    fetcher_tick();
    
    if (bgwin_fifo.is_shifting_pixels() && !object_occupies_current_pixel()) {
        shift_pixel();
    }
}

/* Set the fetcher source to the given source. 
 * Restarts the fetcher and sets the appropriate variables in preparation for
 * the new source. */
void PixelTransferrer::set_fetcher_source(FetcherSource new_source) {

    fetcher.source = new_source;
    fetcher.restart();
    switch(fetcher.source) {
        
        case FetcherSource::BACKGROUND:
            fetcher.x = (lx + bgwin_fifo.size()) >> 3;
            break;

        case FetcherSource::WINDOW:
            fetcher.x = ((lx - (wx - 7)) + bgwin_fifo.size()) >> 3;
            window_visible_on_scanline = true;
            break;
    }
}

/* Push the fetcher's current row_buffer onto the corresponding PixelFifo as
 * Pixels (if it has space).
 * Increments fetcher.x or fetcher.current_object_index.
 * Sets the fetcher source to BACKGROUND and restarts it. */
void PixelTransferrer::try_push_fetcher_to_fifo() {

    switch(fetcher.source) {

        case FetcherSource::BACKGROUND:
        case FetcherSource::WINDOW:
            if (!bgwin_fifo.is_accepting_pixels()) {
                return;
            }
            for (int i = 7; i >= 0; i--) {
                const uint8_t colour_id = colour_id_from_row(
                    fetcher.row_low_buffer,
                    fetcher.row_high_buffer,
                    i
                );
                bgwin_fifo.push(Pixel{colour_id});
            }
            fetcher.x++;
            fetcher.restart();
            break;

        case FetcherSource::OBJECT:
            const OamObject& oam_object = scanline_objects.front();
            const PaletteRegister palette_register =
                PaletteRegister(oam_object.pallette + 1);
            const bool priority = oam_object.priority;
            const int cut_off_pixels = oam_object.x < 8 ? 8 - oam_object.x : 0; 
            const int start = oam_object.x_flip ?
                0 + cut_off_pixels : 7 - cut_off_pixels;
            const int end = oam_object.x_flip ? 8 : -1;
            const int step = oam_object.x_flip ? 1 : -1;
            uint8_t position = 0;
            for (int i = start; i != end; i += step) {
                const uint8_t colour_id = colour_id_from_row(
                    fetcher.row_low_buffer,
                    fetcher.row_high_buffer,
                    i
                );
                object_fifo.push(
                    Pixel{colour_id, palette_register, priority},
                    position++
                );
            }
            scanline_objects.pop_front();
            set_fetcher_source(window_covers_current_pixel() ? 
                FetcherSource::WINDOW : FetcherSource::BACKGROUND);
            break;
    }
}

// Return the colour ID at internal tile coordinate 7-i on the given tile row.
uint8_t PixelTransferrer::colour_id_from_row(
    uint8_t row_low,
    uint8_t row_high,
    uint8_t i
) const {
    return ((row_high >> i) & 1) << 1 | ((row_low >> i) & 1);
}

/* Check what's occupying (lx, ly_) and the current state of the PixelFifos to
 * ensure the Fetcher's source is correct. */
void PixelTransferrer::check_fetcher_source() {

    switch(fetcher.source) {

        case FetcherSource::BACKGROUND:
            if (window_covers_current_pixel()) {
                bgwin_fifo.clear();
                bgwin_fifo.to_discard = 0;
                set_fetcher_source(FetcherSource::WINDOW);
            }
            else if (bgwin_fifo.is_shifting_pixels() &&
                object_occupies_current_pixel()) {
                set_fetcher_source(FetcherSource::OBJECT);
            }
            break;

        case FetcherSource::WINDOW:
            if (!window_enabled()) {
                bgwin_fifo.clear();
                bgwin_fifo.to_discard = bgwin_pixels_to_discard;
                set_fetcher_source(FetcherSource::BACKGROUND);
            }
            else if (bgwin_fifo.is_shifting_pixels() &&
                object_occupies_current_pixel()) {
                set_fetcher_source(FetcherSource::OBJECT);
            }
            break;

        case FetcherSource::OBJECT:
            if (!objects_enabled()) {
                object_fifo.clear();
                set_fetcher_source(window_covers_current_pixel() ? 
                    FetcherSource::WINDOW : FetcherSource::BACKGROUND);
            }
            break;
    }
}

// Return true if the window is enabled and covers the pixel at (lx, ly_).
bool PixelTransferrer::window_covers_current_pixel() const {
    return window_present_on_scanline && window_enabled() && lx >= wx - 7;
}

/* Return true if objects are enabled and one occupies the pixel at (lx,ly_).
 * If there are leading scanline_objects that were passed over when objects
 * weren't enabled they will be removed here. */
bool PixelTransferrer::object_occupies_current_pixel() {
    if (!objects_enabled() || !scanline_objects.size()) {
        return false;
    }
    while (scanline_objects.front().x < lx) {
        scanline_objects.pop_front();
    }
    const OamObject& oam_object = scanline_objects.front();
    return oam_object.x - 8 <= lx && oam_object.x > lx;
}

// Carry out 1 fetcher t-cycle.
void PixelTransferrer::fetcher_tick() {

    if (!(fetcher.cycles++ & 0x1)) { // Act on 2nd cycle of each FetcherMode
        return;
    }

    switch(fetcher.mode) {
        case FetcherMode::TILE_ID:
            fetcher.current_tile_id = fetch_tile_id();
            fetcher.mode = FetcherMode::TILE_ROW_LOW;
            break;

        case FetcherMode::TILE_ROW_LOW: {
            fetcher.current_tile_offset = fetch_tile_offset();
            const bool unsigned_addressing = 
                fetcher.source == FetcherSource::OBJECT ?
                true : bgwin_unsigned_addressing();
            fetcher.row_low_buffer = fetch_tile_row(
                fetcher.current_tile_id,
                fetcher.current_tile_offset,
                unsigned_addressing,
                false
            );
            fetcher.mode = FetcherMode::TILE_ROW_HIGH;
            break;
        }

        case FetcherMode::TILE_ROW_HIGH: {
            const bool unsigned_addressing = 
                fetcher.source == FetcherSource::OBJECT ?
                true : bgwin_unsigned_addressing();
            fetcher.row_high_buffer = fetch_tile_row(
                fetcher.current_tile_id,
                fetcher.current_tile_offset,
                unsigned_addressing,
                true
            );
            fetcher.mode = FetcherMode::PUSH;
            break;
        }
    }
}

/* Return the tile ID of the tile that (lx, ly_) occupies.
 * Depends on the current fetcher.source:
 * - BACKGROUND - Return the tile ID of the tile at the current X, Y position
 * on the current background tile-map, coordinates larger than 31 wrap around.
 * - WINDOW - Return the tile ID of the tile at the current X, Y position on
 * the current window tile-map. 
 * - OBJECT - Return the tile ID of the current OamObject, or incremented by 1
 * if ly_ places us in the possible second (underneath) tile of the OamObject.
 */
uint8_t PixelTransferrer::fetch_tile_id() const {

    switch(fetcher.source) {

        case FetcherSource::BACKGROUND: {
            const uint8_t tile_x = ((scx >> 3) + fetcher.x) & 0x1F;
            const uint8_t tile_y = ((scy + ly_) >> 3) & 0x1F;
            return vram[
                background_tile_map_address() + (tile_y << 5) + tile_x
            ];
        }

        case FetcherSource::WINDOW: {
            const uint8_t tile_y = fetcher.wly >> 3;
            return vram[window_tile_map_address() + (tile_y << 5) + fetcher.x];
        }

        case FetcherSource::OBJECT:
            const OamObject& oam_object = scanline_objects.front();
            const uint8_t tile_offset = ly_ - oam_object.y + 16;
            return oam_object.tile_id + (tile_offset >= 8);
    }

    __builtin_unreachable();
}

// Return the local y-coordinate on the current tile.
uint8_t PixelTransferrer::fetch_tile_offset() const {
    switch(fetcher.source) {
        case FetcherSource::BACKGROUND:
            return (scy + ly_) & 0x7;
        case FetcherSource::WINDOW:
            return fetcher.wly & 0x7;
        case FetcherSource::OBJECT:
            const OamObject& oam_object = scanline_objects.front();
            const uint8_t tile_offset = (ly_ - oam_object.y + 16) & 0x7;
            return oam_object.y_flip ? 8 - tile_offset : tile_offset;
    }

    __builtin_unreachable();
}

/* Fetch half of the row of the tile with given tile_id at the given
 * tile_offset.
 * Uses unsigned addressing from VRAM[0] if unsigned_addressing is true, or
 * signed addressing about VRAM[0x1000] otherwise.
 * Fetches the first byte if high is false, or the second byte otherwise. */
uint8_t PixelTransferrer::fetch_tile_row(
    uint8_t tile_id,
    uint8_t tile_offset,
    bool unsigned_addressing,
    bool high
) const {
    const uint16_t tile_address = unsigned_addressing ? 
        tile_id * 16 : 0x1000 + static_cast<int8_t>(tile_id) * 16;
    return vram[tile_address + 2 * tile_offset + high];
}

/* Push a pixel to the pixel_buffer or discard it if necessary.
 * Mixes the pixels in the background/window and object PixelFifos if the
 * object PixelFifo is non-empty.
 * Increments lx. */
void PixelTransferrer::shift_pixel() {

    if (bgwin_fifo.to_discard) {
        bgwin_fifo.discard();
        return;
    }

    Pixel bgwin_pixel = bgwin_fifo.shift();
    if (!bgwin_enabled()) {
        bgwin_pixel = Pixel{0};
    }
    Pixel shifted_pixel;
    if (object_fifo.is_empty()) {
        shifted_pixel = bgwin_pixel;
    }
    else {
        const Pixel object_pixel = object_fifo.shift();
        if (!object_pixel.colour_id) {
            shifted_pixel = bgwin_pixel;
        }
        else if (object_pixel.priority && bgwin_pixel.colour_id) {
            shifted_pixel = bgwin_pixel;
        }
        else {
            shifted_pixel = object_pixel;
        }
    }
    pixel_buffer[ly_ * SCREEN_WIDTH + lx++] = pixel_to_rgba(shifted_pixel);
}

/* Return the RGBA colour in the palette at the index obtained using the given
 * Pixel's colour_id in its (corrsponding) palette_register via the mapping:
 * Colour ID - 2 bit index
 * - 3 - bits 7-6
 * - 2 - bits 5-4
 * - 1 - bits 3-2
 * - 0 - bits 1-0 */
uint32_t PixelTransferrer::pixel_to_rgba(Pixel pixel) const {
    switch(pixel.palette_register) {
        case PaletteRegister::BGP:
            return palette.at((bgp >> (pixel.colour_id * 2)) & 0x3);
        case PaletteRegister::OBP0:
            return palette.at((obp0 >> (pixel.colour_id * 2)) & 0x3);
        case PaletteRegister::OBP1:
            return palette.at((obp1 >> (pixel.colour_id * 2)) & 0x3);
    }

    __builtin_unreachable();
}