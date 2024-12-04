#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char** argv) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Hello world with SDL2: " << argc << std::endl;

    return 0;
}