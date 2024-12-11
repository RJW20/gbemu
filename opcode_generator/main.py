import json
from typing import Any

type Opcode = dict[str, Any]


def declaration(opcode: Opcode, prefixed=False) -> str:
    """Return the Cpu function declaration for the given opcode."""

    if not prefixed:
        return f"Opcode Cpu::opcode_{opcode["addr"]} ();\n"
    else:
        return f"Opcode Cpu::opcode_cb_{opcode["addr"]} ();\n"


def main() -> None:
    """"""
    
    with open("opcodes.json", "r") as f:
        all_opcodes = json.load(f)

    opcodes = all_opcodes["unprefixed"]
    cb_opcodes = all_opcodes["cbprefixed"]

    
    with open("declaration.txt", "w+") as f:
        f.write("// Opcode methods\n")
        for opcode in opcodes.values():
            f.write(declaration(opcode))

        f.write("\n// CB opcode methods\n")
        for cb_opcode in cb_opcodes.values():
            f.write(declaration(cb_opcode, prefixed=True))



if __name__ == "__main__":
    main()
