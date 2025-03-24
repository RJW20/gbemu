from opcode import Opcode


def dictionary_initialisations(
    opcodes: list[Opcode],
    cb_opcodes: list[Opcode],
) -> None:
    """Write all opcode dictionary initialisations to the file
    "dictionary.txt"."""

    with open("dictionary.txt", "w+") as f:

        for opcode in opcodes:
            f.write(f"opcodes[{opcode.address}] = {opcode.name}();\n")

        f.write("\n")

        for cb_opcode in cb_opcodes:
            f.write(f"cb_opcodes[{cb_opcode.address}] = {cb_opcode.name}();\n")