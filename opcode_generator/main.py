import json

from opcode import Opcode
from definitions import add


def main() -> None:
    """Load all opcodes from "opcodes.json" and create files containing their
    corresponding function declarations, dictionary definitions and function
    definitions, to be used as part of gbemu/src/cpu."""
    
    with open("opcodes.json", "r") as f:
        all_opcodes = json.load(f)

    opcodes = [
        Opcode(opcode_dict, False) for opcode_dict in
        all_opcodes["unprefixed"].values()
    ]
    cb_opcodes = [
        Opcode(opcode_dict, True) for opcode_dict in
        all_opcodes["cbprefixed"].values()
    ]

    # Declarations
    with open("declaration.txt", "w+") as f:
        f.write("// Opcode methods\n")
        for opcode in opcodes:
            f.write(f"Opcode {opcode.function_handle}();\n")

        f.write("\n// CB opcode methods\n")
        for cb_opcode in cb_opcodes:
            f.write(f"Opcode {cb_opcode.function_handle}();\n")

    # Dictionary initialisation
    with open("dictionary.txt", "w+") as f:

        f.write("opcodes = {\n")
        for opcode in opcodes:
            f.write(
                "    "
                f"{{{opcode.address}, {opcode.function_handle}()}},\n"
            )
        f.write("};\n\n")

        f.write("cb_opcodes = {\n")
        for cb_opcode in cb_opcodes:
            f.write(
                "    "
                f"{{{cb_opcode.address}, {cb_opcode.function_handle}()}},\n"
            )
        f.write("};")
        
    # Definitions
    with open("opcodes.cpp", "w+") as f:

        f.write("// This file is auto-generated.\n")
        f.write("// Do not modify this file directly.\n\n")

        f.write("#include <vector>\n")
        f.write("#include \"cpu.hpp\"\n")
        f.write("#include \"opcode.hpp\"\n\n")

        for opcode in opcodes:
            match opcode.mnemonic:

                case "ADD":
                    if opcode.operand1 != "HL":
                        f.write(add(opcode))
            
            f.write("\n")


if __name__ == "__main__":
    main()
