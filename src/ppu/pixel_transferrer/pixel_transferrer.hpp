#ifndef PIXEL_TRANSFERRER_HPP
#define PIXEL_TRANSFERRER_HPP

#include <cstdint>
#include "../ppu_core.hpp"
#include "fetcher.hpp"
#include "pixel_fifo.hpp"

/* Pixel Transferrer 
 * Fetches tiles from VRAM and pushes RGBA values to its pixel_buffer. */
class PixelTransferrer : virtual public PpuCore {
public:
    PixelTransferrer() {}

    // Pixel buffer
    static constexpr uint8_t SCREEN_WIDTH = 160;
    static constexpr uint8_t SCREEN_HEIGHT = 144;
    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> pixel_buffer;

private:
    PixelFifo bgwin_fifo;
    PixelFifo object_fifo;

    // Helper methods
    void set_fetcher_source(FetcherSource source);
    void try_push_fetcher_to_fifo();
    uint8_t colour_id_from_row(
        uint8_t row_low,
        uint8_t row_high,
        uint8_t x
    ) const;
    void check_fetcher_source();
    bool window_covers_current_pixel() const;
    bool object_occupies_current_pixel();
    void fetcher_tick();
    uint8_t fetch_tile_id() const;
    uint8_t fetch_tile_offset() const;
    uint8_t fetch_tile_row(
        uint8_t tile_id,
        uint8_t tile_offset,
        bool unsigned_addressing,
        bool high
    ) const;
    void shift_pixel();
    uint32_t pixel_to_rgba(Pixel pixel) const;

protected:
    uint8_t lx;
    Fetcher fetcher;
    bool window_visible_on_scanline;    // Flag for incrementing fetcher.wly

    void new_pixel_transfer();
    void pixel_transfer_tick();

    /* Singular global palette containing RGBA colour values, with 2 bit index:
     * 0 - Light-green
     * 1 - Green
     * 2 - Dark-green
     * 3 - Black */
    static constexpr std::array<uint32_t, 4> palette = {
        0xE0F8D0FF, 0x88C070FF, 0x346856FF, 0x081820FF
    };
};

#endif