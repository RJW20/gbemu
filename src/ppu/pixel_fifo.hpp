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
    void accept_pixels(uint16_t pixels);
    uint8_t shift_pixel();

protected:
    uint8_t pointer;    // 0 points to pixel in bits 31-30

private:
    uint32_t buffer;    // 16 x 2 bit pixels
};

/* Background PixelFIFO
 * Implements methods to check for accepting and shift pixels. */
class BackgroundPixelFifo : public PixelFifo {
public:
    BackgroundPixelFifo() : PixelFifo() {};
    ~BackgroundPixelFifo() {};

    bool is_accepting_pixels() const;
    bool is_shifting_pixels() const;
};

/* Object PixelFIFO
 * Implements method to check for emptiness. */
class ObjectPixelFifo : public PixelFifo {
public:
    ObjectPixelFifo() : PixelFifo() {};
    ~ObjectPixelFifo() {};

    bool is_empty() const;
};

#endif