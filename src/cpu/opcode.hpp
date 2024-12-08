#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

typedef std::function<void()> Step;

class Opcode {
public:
    Opcode(
        uint8_t value,
        std::string name,
        int t_cycles,
        int length,
        std::vector<Step> steps
    ) : value(value), name(name), t_cycles(t_cycles), length(length),
        steps(steps) {};
    ~Opcode() {};

    uint8_t value;              // Hex value of opcode
    std::string name;           // Common name for opcode
    int t_cycles;               // t-cycles required to execute
    int length;                 // The number of steps in the execution
    std::vector<Step> steps;    // Functions carried out during the execution
};

#endif