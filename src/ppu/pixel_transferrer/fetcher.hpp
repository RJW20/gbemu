#ifndef FETCHER_HPP
#define FETCHER_HPP

#include <cstdint>

/* FetcherMode
 * Enum containing 4 modes the Fetcher can be in:
 * - TILE_ID - Fetch the current_tile_id from the current tile map at x, y. 
 * - TILE_ROW_LOW - Fetch the leading byte of a row of the tile with
 * current_tile_id.
 * - TILE_ROW_HIGH - Fetch the trailing byte of a row of the tile with
 * current_tile_id.
 * - PUSH - Attempt to push the row_buffer into the FIFO as pixels. */
enum class FetcherMode {
    TILE_ID,
    TILE_ROW_LOW,
    TILE_ROW_HIGH,
    PUSH
};

/* FetcherSource 
 * Enum containing 3 sources the Fetcher can read tile data from. 
 * - BACKGROUND - Fetch tiles via the background tile map. 
 * - WINDOW - Fetch tiles via the window tile map. 
 * - OBJECT - Fetch tiles via OAM entry. */
enum class FetcherSource {
    BACKGROUND,
    WINDOW,
    OBJECT
};

/* Fetcher
 * Struct holding all relevant data about the Fetcher's current state. */
struct Fetcher {
    uint8_t cycles;
    FetcherMode mode;
    FetcherSource source;
    uint8_t x;              // tile X on {source} tile map
    uint8_t wly;            // window line counter
    uint8_t current_object_index;   // index for scanline objects
    uint8_t current_tile_id;
    uint8_t current_tile_offset;    // internal tile y coordinate
    uint8_t row_low_buffer;
    uint8_t row_high_buffer;

    void restart() { cycles = 0; mode = FetcherMode::TILE_ID; }
};

#endif