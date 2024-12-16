cmake_minimum_required(VERSION 3.10)
project(gbemu)

set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
    src/main.cpp
    src/gameboy.cpp
    src/cartridge/cartridge.cpp
    src/interrupt_manager.cpp
    src/timer.cpp
    src/mmu.cpp
    src/cpu/cpu.cpp
    src/cpu/register.cpp
    src/cpu/basis.cpp
    src/cpu/opcodes.cpp
)

# SDL2 (for graphics, sound, and input)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(LIBS ${SDL2_LIBRARIES})

# Create executable
add_executable(gbemu ${SOURCES})
target_link_libraries(gbemu ${LIBS})