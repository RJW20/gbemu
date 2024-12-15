import json

from opcode import Opcode


def main() -> None:
    """"""
    
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



if __name__ == "__main__":
    main()
