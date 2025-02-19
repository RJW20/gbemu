from typing import Any

INDENT = "    "


class Opcode:
    """Class for an opcode."""

    def __init__(self, opcode_dict: dict[str, Any], prefixed: bool) -> None:

        self.prefixed = prefixed
    
        try:
            self.address = opcode_dict["addr"]
            self.mnemonic = opcode_dict["mnemonic"]
            self.length = opcode_dict["length"]
            self.cycles = opcode_dict["cycles"]
        except KeyError as e:
            print(f"Every opcode must have the attribute {e.args[0]}.")

        try:
            self.operand1 = opcode_dict["operand1"]
        except KeyError:
            self.operand1 = None

        try:
            self.operand2 = opcode_dict["operand2"]
        except KeyError:
            self.operand2 = None

    @property
    def name(self) -> str:
        """Return this opcodes name."""

        if not self.prefixed:
            return f"opcode_{self.address}"
        else:
            return f"opcode_cb_{self.address}"
    
    @property
    def full_name(self) -> str:
        """Return this opcodes full name."""

        if self.operand1 is None:
            return self.mnemonic
        elif self.operand2 is None:
            return f"{self.mnemonic} {self.operand1}"
        else:
            return f"{self.mnemonic} {self.operand1}, {self.operand2}"

    @property
    def function_start(self) -> str:
        """Return the first 2 lines of this opcodes function definition."""

        return (
            f"// {self.full_name}\n"
            f"Opcode Cpu::{self.name}() {{\n"
            f"{INDENT}std::vector<Step> steps;\n"
        )
    
    @property
    def function_end(self) -> str:
        """Return the last 3 lines of this opcodes function definition."""

        return (
            f"{INDENT}return Opcode{{{self.address}, \"{self.full_name}\", "
            f"{self.length}, {self.cycles}, steps}};\n}}\n"
        )