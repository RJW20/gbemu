# gbemu
A GameBoy emulator written in C++, using the SDL2 library for graphics.

## Usage
### Requirements
1. **C++ Compiler**: Support for C++17 or later.
2. **SDL2 Library**: For graphics rendering.

### Building the Emulator
Make sure the SDL2 package is available somewhere on your system and run: 
```
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=<path_to_sdl2_package>
make
```
Place SDL2.dll from the package in the build folder before running.

### Running the Emulator
`./gbemu <ROM file>`

### Controller Mapping
- START = Enter
- SELECT = Backspace
- B = Z
- A = X
- UP, RIGHT, DOWN, LEFT = arrow keys
- Speed-up = Spacebar (hold)

## Implementation Details

### Timing
All of the components in the GameBoy need to work in synchronization - as the internal timer ticks, each component needs to carry out the correct amount of work. There are 2 main ways to emulate this:
- **CPU driven**: The CPU fetches an opcode, carries out the relevant operations and then notifies all other components how many cycles it took so that they can catch up.
- **Timer driven**: Each timer tick, every component advances one cycle.

The first method is far easier to implement, and would also result in a faster emulator. However it is not very accurate, and passing many of the tests mentioned below would be impossible. I have therefore chosen to take the timer driven approach, and this is why all my [opcodes](https://github.com/RJW20/gbemu/blob/main/src/cpu/opcodes.cpp) are implemented in steps.

### Absent Features
- The APU does not function other than to hold the relevant registers. This does not impact any other functionality.
- Some bugs I have chosen not to implement, like the OAM corruption bug and STAT blocking.
- OAM and VRAM have inaccessibility windows, but the CPU is currently free to access them at all times.
- No save functionality.

## Opcodes
There are just over 500 opcodes, and typing them all out is not only boring and error-prone, but it is also highly inefficient as many of them can be grouped together. I have included my Python scripts that I used to automatically generate the C++ code for all of them [here](https://github.com/RJW20/gbemu/tree/main/opcode_generator).

## Tests
- [Blargg](https://github.com/retrio/gb-test-roms) - Passes all except the OAM bug tests (as not implemented).
- [rtc3test](https://github.com/aaaaaa123456789/rtc3test) - Used to get the Real Time Clock working for MBC3.

## References
- [PanDocs](https://gbdev.io/pandocs/About.html) - Implementations for everything except the CPU mostly utilised the information found here. Note that for the PPU, lots of specific details are missing or glossed over.
- [CPU opcodes](https://meganesu.github.io/generate-gb-opcodes/) - A useful overview of all the CPU opcodes.
- [CPU instruction timings](https://gekkio.fi/files/gb-docs/gbctr.pdf) - Information on the general grouping of the CPU opcodes and importantly their timings.