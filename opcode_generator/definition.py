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
                    else:
                        f.write(add_hl(opcode))

                case "ADC":
                    f.write(adc(opcode))

                case "SUB":
                    f.write(sub(opcode))

                case "SBC":
                    f.write(sbc(opcode))

                case "AND":
                    f.write(and_(opcode))

                case "OR":
                    f.write(or_(opcode))

                case "XOR":
                    f.write(xor_(opcode))

                case "CP":
                    f.write(compare(opcode))

                case "INC":
                    f.write(inc(opcode))

                case "DEC":
                    f.write(dec(opcode))
            
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
                f"{step("reg.sp = z16")}"
                f"{step("")}"
            )

        case _:
            return (
                f"{step(f"add(reg.{opcode.operand2.lower()})")}"
            )
        
@wrap_function_definition()
def adc(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("add_with_carry(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("add_with_carry(z8)")}"
            )

        case _:
            return (
                f"{step(f"add_with_carry(reg.{opcode.operand2.lower()})")}"
            )

@wrap_function_definition()
def sub(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("subtract(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("subtract(z8)")}"
            )

        case _:
            return (
                f"{step(f"subtract(reg.{opcode.operand1.lower()})")}"
            )
        
@wrap_function_definition()
def sbc(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("subtract_with_carry(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("subtract_with_carry(z8)")}"
            )

        case _:
            return (
                f"{step(f"subtract_with_carry(reg.{opcode.operand2.lower()})")}"
            )
        
@wrap_function_definition()
def and_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("and_(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("and_(z8)")}"
            )

        case _:
            return (
                f"{step(f"and_(reg.{opcode.operand1.lower()})")}"
            )

@wrap_function_definition()
def or_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("or_(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("or_(z8)")}"
            )

        case _:
            return (
                f"{step(f"or_(reg.{opcode.operand1.lower()})")}"
            )
        
@wrap_function_definition()
def xor_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("xor_(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("xor_(z8)")}"
            )

        case _:
            return (
                f"{step(f"xor_(reg.{opcode.operand1.lower()})")}"
            )
        
@wrap_function_definition()
def compare(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("compare(z8)")}"
            )
        
        case "d8":
            return(
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("compare(z8)")}"
            )

        case _:
            return (
                f"{step(f"compare(reg.{opcode.operand1.lower()})")}"
            )
        
@wrap_function_definition()
def inc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = increment(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )
        
        case "BC" | "DE" | "HL":
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.set_{register}(reg.{register}() + 1)")}"
            )
        
        case "SP":
            return (
                f"{step("reg.sp++")}"
            )

        case _:
            return (
                f"{step(f"increment(reg.{opcode.operand1.lower()})")}"
            )

@wrap_function_definition()
def dec(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return(
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = decrement(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )
        
        case "BC" | "DE" | "HL":
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.set_{register}(reg.{register}() - 1)")}"
            )

        case "SP":
            return (
                f"{step("reg.sp--")}"
            )

        case _:
            return (
                f"{step(f"decrement(reg.{opcode.operand1.lower()})")}"
            )
        
@wrap_function_definition()
def add_hl(opcode: Opcode) -> str:

    if opcode.operand2 != "SP":
        return (
            f"{step(f"add_hl(reg.{opcode.operand2.lower()}())")}"
        )

    else:
        return (
            f"{step("add_hl(reg.sp)")}"
            f"{step("")}"
        )