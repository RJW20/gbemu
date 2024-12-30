#include "gameboy.hpp"

void GameBoy::run() {

    while (true) {
        tick();
    }
}

// Carry out 1 t-cycle.
void GameBoy::tick() {
    timer.tick();
    cpu.tick();
    serial.tick();
}