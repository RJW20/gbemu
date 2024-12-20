from opcode import Opcode


def dictionary_initialisations(
    opcodes: list[Opcode],
    cb_opcodes: list[Opcode],
) -> None:
    """Write all opcode dictionary initialisations to the file
    "dictionary.txt"."""

    with open("dictionary.txt", "w+") as f:

        f.write("opcodes = {\n")
        for opcode in opcodes:
            f.write(
                "    "
                f"{{{opcode.address}, &{opcode.variable_name}}},\n"
            )
        f.write("};\n\n")

        f.write("cb_opcodes = {\n")
        for cb_opcode in cb_opcodes:
            f.write(
                "    "
                f"{{{cb_opcode.address}, &{cb_opcode.variable_name}}},\n"
            )
        f.write("};")