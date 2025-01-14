#include "gameboy.hpp"

void GameBoy::run() {

    while (true) {
        tick();
        if (++ticks == SIXTY_FPS) {
            screen.render();
            ticks = 0;
        }
    }
}

// Carry out 1 t-cycle.
void GameBoy::tick() {
    timer.tick();
    cpu.tick();
    serial.tick();
    dma.tick();
    ppu.tick();
}