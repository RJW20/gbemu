import json

from opcode import Opcode
from declaration import declarations
from definition import definitions
from dictionary import dictionary_initialisations


def main() -> None:
    """Load all opcodes from "opcodes.json" and create files containing their
    corresponding function declarations, dictionary definitions and function
    definitions, to be used as part of gbemu/src/cpu."""
    
    # Load Opcodes
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

    declarations(opcodes, cb_opcodes)
    dictionary_initialisations(opcodes, cb_opcodes)
    definitions(opcodes, cb_opcodes)


if __name__ == "__main__":
    main()
