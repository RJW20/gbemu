from opcode import Opcode


def declarations(opcodes: list[Opcode], cb_opcodes: list[Opcode]) -> None:
    """Write all opcode function declarations to the file "declaration.txt"."""

    with open("declaration.txt", "w+") as f:

        f.write("// Opcode generators\n")
        for opcode in opcodes:
            f.write(f"Opcode {opcode.function_handle}();\n")

        f.write("\n// CB opcode generators \n")
        for cb_opcode in cb_opcodes:
            f.write(f"Opcode {cb_opcode.function_handle}();\n")

        f.write("\n// Opcode variables\n")
        for opcode in opcodes:
            f.write(
                f"Opcode {opcode.variable_name} = {opcode.function_handle}();\n"
            )

        f.write("\n// CB opcode variables\n")
        for cb_opcode in cb_opcodes:
            f.write(
                f"Opcode {cb_opcode.variable_name} = "
                f"{cb_opcode.function_handle}();\n"
            )