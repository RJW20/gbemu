from typing import Callable

from opcode import Opcode, INDENT


def definitions(opcodes: list[Opcode], cb_opcodes: list[Opcode]) -> None:
    """Write all opcode function definitions to the file "opcodes.cpp"."""
    
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


def wrap_function_definition() -> Callable[
    [Callable[[Opcode], str]], Callable[[Opcode], str]
]:
    """Decorator to wrap a functions output with the Opcode that it was called
    with's function_start and function_end."""

    def decorator(func: Callable[[Opcode], str]) -> Callable[[Opcode], str]:
        def wrapped(opcode: Opcode) -> str:
            return (
                f"{opcode.function_start}"
                f"{func(opcode)}"
                f"{opcode.function_end}"
            )
        return wrapped
    return decorator

def step(operation: str) -> str:
    """Return an operation encased in a push_back to the steps vector."""
    return f"{INDENT}steps.push_back([this]() {{ {operation}; }});\n"

@wrap_function_definition()
def add(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("add(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("add(z8)")}"
            )
        
        case "r8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("z16 = add_signed8(reg.sp, z8)")}"
                f"{step("")}"
                f"{step("reg.sp = z16")}"
            )

        case _:
            return (
                f"{step(f"add(reg.{opcode.operand2.lower()})")}"
            )