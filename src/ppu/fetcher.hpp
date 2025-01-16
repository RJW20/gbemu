#ifndef FECTHER_HPP
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
 * - WINDOW - Fetch tile via the window tile map. */
enum class FetcherSource {
    BACKGROUND,
    WINDOW
};

/* Fetcher
 * Struct holding all relevant data about the Fetcher's current state. */
struct Fetcher {
    uint8_t cycles;
    FetcherMode mode;
    FetcherSource source;
    uint8_t x;  // tile position from the left of the screen
    uint8_t y;  // tile position from the top of the screen
    uint8_t current_tile_id;
    uint16_t row_buffer;
};

#endif