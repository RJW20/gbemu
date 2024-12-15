from typing import Callable

from opcode import Opcode, INDENT


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