#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "mmu.hpp"

class GameBoy {
public:
    GameBoy() {};
    ~GameBoy() {};

private:
    InterruptManager interrupt_manager;
    Timer timer = Timer(&interrupt_manager);
    Mmu mmu;
};

#endif