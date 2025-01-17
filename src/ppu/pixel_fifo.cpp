#include <cstdint>
#include "pixel_fifo.hpp"

// Reset the PixelFifo to empty.
void PixelFifo::clear() {
    buffer = 0;
    pointer = 0;
}

/* Place the given pixels into the buffer starting at the position given by the
 * pointer. */
void PixelFifo::accept_pixels(uint16_t pixels) {
    buffer &= (pixels << (8 - pointer) * 2);
    pointer += 8;
}

// Remove and return the next pixel.
uint8_t PixelFifo::shift_pixel() {
    uint8_t pixel = buffer >> 30;
    buffer <<= 2;
    pointer -= 1;
    return pixel;
}

// Return true if the PixelFifo has 8 or fewer pixels in it.
bool BackgroundPixelFifo::is_accepting_pixels() const {
    return pointer <= 8;
}

// Return true if the PixelFifo has more than 8 pixels in it.
bool BackgroundPixelFifo::is_shifting_pixels() const {
    return pointer > 8;
}

// Return true if not currently storing any pixels.
bool ObjectPixelFifo::is_empty() const {
    return !pointer;
} 