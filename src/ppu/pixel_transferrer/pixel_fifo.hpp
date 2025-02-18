#ifndef PIXEL_FIFO_HPP
#define PIXEL_FIFO_HPP

#include <cstdint>
#include <array>
#include "../../logger.hpp"

/* PaletteRegister
 * Enum containing information about which palette register a Pixel will derive
 * its colour from. */
enum class PaletteRegister : uint8_t {
    BGP = 0,
    OBP0 = 1,
    OBP1 = 2
};

/* Pixel 
 * Struct containing all information about an entry in a PixelFifo. */
struct Pixel {
    uint8_t colour_id : 2;
    PaletteRegister palette_register : 2;
    bool priority : 1;
};

/* Pixel First-In-First-Out
 * Data structure for storing Pixels.
 * Pixels are shifted out in the order they were pushed. */
class PixelFifo {
public:
    PixelFifo() { clear(); }

    void clear() { front_pointer = 0; end_pointer = 0; size_ = 0; }
    Pixel shift() { size_--; return data[front_pointer++]; }

protected:
    std::array<Pixel, 0x10> data;
    uint8_t front_pointer : 4;
    uint8_t end_pointer : 4;
    uint8_t size_;
};

// PixelFifo for background or window Pixels.
class BGWinFifo : public PixelFifo {
public:
    BGWinFifo() : PixelFifo() {}

    uint8_t to_discard;     // Count to shift off before pushing to LCD

    bool is_accepting_pixels() const { return size_ <= 8; }
    bool is_shifting_pixels() const { return size_ > 8; }
    uint8_t size() const { return size_; }
    void push(Pixel pixel) { data[end_pointer++] = pixel; size_++; }
    void discard() {size_--; front_pointer++; to_discard--; }
};

// PixelFifo for object Pixels.
class ObjectFifo : public PixelFifo {
public:
    ObjectFifo() : PixelFifo() {}

    bool is_empty() const { return !size_; }
    void push(Pixel pixel, uint8_t position) {
        if (position < size_) {
            const uint8_t index = (front_pointer + position) & 0xF;
            if (!data[index].colour_id) {
                data[index] = pixel;
            }
        }
        else if (position == size_) {
            data[end_pointer++] = pixel;
            size_++;
        }
        else {
            Log::warning(
                "Cannot push pixel to ObjectFifo beyond its current end point."
            );
        }
    }
};

#endif