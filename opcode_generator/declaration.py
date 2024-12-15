from opcode import Opcode


def declarations(opcodes: list[Opcode], cb_opcodes: list[Opcode]) -> None:
    """Write all opcode function declarations to the file "declaration.txt"."""

    with open("declaration.txt", "w+") as f:

        f.write("// Opcode methods\n")
        for opcode in opcodes:
            f.write(f"Opcode {opcode.function_handle}();\n")

        f.write("\n// CB opcode methods\n")
        for cb_opcode in cb_opcodes:
            f.write(f"Opcode {cb_opcode.function_handle}();\n")