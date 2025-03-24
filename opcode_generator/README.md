# Opcode Generator
The Python scripts found here take information about all the GameBoy opcodes found in `opcodes.json` and generate C++ code for use in my GameBoy emulator.

## Emulator Configuration

### Opcodes
The goal is to turn `opcodes.json` into two hash tables (non-0xCB and 0xCB) that map an opcode's address to a C++ struct containing all relevant information about it:
```
struct Opcode {
    uint8_t address;            // Hex address of opcode
    std::string name;           // Common name for opcode
    int length;                 // Number of bytes read in the fetch/execution
    int t_cycles;               // t-cycles required to execute
    std::vector<Step> steps;    // Functions carried out during the execution
};
```
where steps is a sequential list of void returning functions that carry out the opcode's operations in a piecewise manner. These hash tables will be members of the Cpu class, and will be entirely instantiated as soon as the emulator is started so that at runtime the Cpu can fetch an opcode address and then have immediate access to the instructions needed to execute it.

### Basis Methods
To enable easy grouping of the opcodes, the Cpu has 27 methods that I have referred to as basis methods as they form the basis of all the opcodes' functionality. The simplest is the add method that adds the given value to register a:
```
void Cpu::add(uint8_t value) {
    int result = reg.a + value;
    reg.flag_z = !(result & 0xFF);
    reg.flag_n = false;
    reg.flag_h = ((reg.a & 0xF) + (value & 0xF) > 0xF); // Bits 3 and 4
    reg.flag_c = (result > 0xFF);
    reg.a = static_cast<uint8_t>(result);
}
```
The opcode structs that are generated can contain calls to these methods in the steps attribute. Note that these basis methods all need to be implemented manually.

### Opcode Generators
The Cpu has methods that generate all of the opcode structs that we can then place inside the relevant hash tables. For example one that utilises that add basis method:
```
Opcode Cpu::opcode_0x80() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.b); });
    return Opcode{0x80, "ADD A, B", 1, 4, steps};
}
```
These generators will be entirely written by the Python scripts.

## Script Output
Running the file `main.py` will invoke all of the functions listed below:
- `declaration.py` defines a function that generates a file `declaration.txt` which contains the method declarations for all of the opcode generators - these can then simply be pasted into the Cpu class declaration.
- `definition.py` defines a function that generates a file `opcodes.cpp` which contains C++ code for every opcode generator method - this can simply be placed in the same directory as the Cpu class declaration header file.
- `dictionary.py` defines a function that generates a file `dictionary.txt` which contains C++ code for populating the Cpu's opcode hash tables - this can be pasted into a method on the Cpu class that is called during its construction.

## Adjusting for another Language
It should be possible to edit these scripts for use in a GameBoy emulator written in another language. This emulator would need to use a very similar configuration to what I have outlined above, and then the following would need to be done:
- An implementation of a similar opcode struct. Note that the information about address, name and length needn't be included, they just give easy debug-ability.
- All 27 basis methods implemented on the Cpu class (or equivalent).
- Edits in `opcodes.py` to fix the `function_start` and `function_end` properties to use the correct language syntax.
- Edits in `definition.py` to the `step` function and to each opcode group generator function to use the correct language syntax.

If you wish to give this a go I would be happy to assist and perhaps update this repo to include the code for your language of choice.