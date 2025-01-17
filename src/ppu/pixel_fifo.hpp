#ifndef PIXEL_FIFO_HPP
#define PIXEL_FIFO_HPP

#include <cstdint>

/* Pixel First-In-First-Out
 * Data structure for storing and shifting out pixels in the order they were
 * fetched. */
class PixelFifo {
public:
    PixelFifo() { clear(); };
    ~PixelFifo() {};

    void clear();
    bool is_accepting_pixels() const;
    bool is_shifting_pixels() const;
    void accept_pixels(uint16_t pixels);
    uint8_t shift_pixel();

private:
    uint32_t buffer;    // 16 x 2 bit pixels
    uint8_t pointer;    // 0 points to pixel in bits 31-30
};

#endif