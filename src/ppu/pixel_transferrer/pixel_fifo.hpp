#ifndef PIXEL_FIFO_HPP
#define PIXEL_FIFO_HPP

#include <cstdint>
#include <array>

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

    uint8_t to_discard;             // Count to shift off before pushing to LCD
    uint8_t shift_until_discard;    // Count to shift to LCD before discarding

    void clear() { front_pointer = 0; end_pointer = 0; size_ = 0; }
    bool is_accepting_pixels() const { return size_ <= 8; }
    bool is_shifting_pixels() const { return size_ > 8; }
    bool is_empty() const { return !size_; }
    uint8_t size() { return size_; }
    void push(Pixel pixel) { data[end_pointer++] = pixel; size_++; }
    void discard() {size_--; front_pointer++; to_discard--; }
    Pixel shift() { 
        size_--;
        if (shift_until_discard) {
            shift_until_discard--;
        }
        return data[front_pointer++];
    }

private:
    std::array<Pixel, 0x10> data;
    uint8_t front_pointer : 4;
    uint8_t end_pointer : 4;
    uint8_t size_;
};

#endif