#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

typedef std::function<void()> Step;

/* Opcode
 * Struct containing all relevant information about an Opcode.
 * Should be passed lambdas from the CPU as steps. */
struct Opcode {
    Opcode(
        uint8_t address,
        std::string name,
        int length,
        int t_cycles,
        std::vector<Step> steps
    ) : address(address), name(name), length(length), t_cycles(t_cycles),
        steps(steps) {}

    uint8_t address;            // Hex address of opcode
    std::string name;           // Common name for opcode
    int length;                 // Number of bytes read in the fetch/execution
    int t_cycles;               // t-cycles required to execute
    std::vector<Step> steps;    // Functions carried out during the execution
};

#endif