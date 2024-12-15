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

        for opcode in opcodes + cb_opcodes:
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
                case "SWAP":
                    f.write(swap(opcode))
                case "DAA":
                    f.write(daa(opcode))
                case "CPL":
                    f.write(cpl(opcode))
                case "CCF":
                    f.write(ccf(opcode))
                case "SCF":
                    f.write(scf(opcode))
                case "NOP" | "HALT" | "STOP":
                    f.write(nop(opcode))
                case "DI":
                    f.write(di(opcode))
                case "EI":
                    f.write(ei(opcode))
                case "RLCA":
                    f.write(rlca(opcode))
                case "RLA":
                    f.write(rla(opcode))
                case "RRCA":
                    f.write(rrca(opcode))
                case "RRA":
                    f.write(rra(opcode))
                case "RLC":
                    f.write(rlc(opcode))
                case "RL":
                    f.write(rl(opcode))
                case "RRC":
                    f.write(rrc(opcode))
                case "RR":
                    f.write(rr(opcode))
                case "BIT":
                    f.write(bit(opcode))
                case "SET":
                    f.write(set_(opcode))
                case "RES":
                    f.write(reset(opcode))
                
            
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("add(z8)")}"
            )
        
        case "d8":
            return (
                f"{step("z8 = mmu->read(reg.pc++)")}"
                f"{step("add(z8)")}"
            )
        
        case "r8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("add_with_carry(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("subtract(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("subtract_with_carry(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("and_(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("or_(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("xor_(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("compare(z8)")}"
            )
        
        case "d8":
            return (
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
            return (
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
            return (
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
    
@wrap_function_definition()
def swap(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = swap(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )
        
        case _:
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.{register} = swap(reg.{register})")}"
            )
        
@wrap_function_definition()
def daa(opcode: Opcode) -> str:

    return f"{step("decimal_adjust_accumulator()")}"

@wrap_function_definition()
def cpl(opcode: Opcode) -> str:

    return f"{step("complement_accumulator()")}"

@wrap_function_definition()
def ccf(opcode: Opcode) -> str:

    return f"{step("complement_carry_flag()")}"

@wrap_function_definition()
def scf(opcode: Opcode) -> str:

    return f"{step("set_carry_flag()")}"

@wrap_function_definition()
def nop(opcode: Opcode) -> str:

    return f"{step("")}"

@wrap_function_definition()
def di(opcode: Opcode) -> str:

    return f"{step("disable_interrupts()")}"

@wrap_function_definition()
def ei(opcode: Opcode) -> str:

    return f"{step("enable_interrupts()")}"

@wrap_function_definition()
def rlca(opcode: Opcode) -> str:

    return (
        f"{step("reg.a = rotate_left_circular(reg.a);\n"
            f"{INDENT}{INDENT}reg.flag_z = false"
        )}"
    )

@wrap_function_definition()
def rla(opcode: Opcode) -> str:

    return (
        f"{step("reg.a = rotate_left(reg.a);\n"
            f"{INDENT}{INDENT}reg.flag_z = false"
        )}"
    )

@wrap_function_definition()
def rrca(opcode: Opcode) -> str:

    return (
        f"{step("reg.a = rotate_right_circular(reg.a);\n"
            f"{INDENT}{INDENT}reg.flag_z = false"
        )}"
    )

@wrap_function_definition()
def rra(opcode: Opcode) -> str:

    return (
        f"{step("reg.a = rotate_right(reg.a);\n"
            f"{INDENT}{INDENT}reg.flag_z = false"
        )}"
    )

@wrap_function_definition()
def rlc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = rotate_left_circular(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )

        case _:
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.{register} = rotate_left_circular(reg.{register})")}"
            )
        
@wrap_function_definition()
def rl(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = rotate_left(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )

        case _:
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.{register} = rotate_left(reg.{register})")}"
            )
        
@wrap_function_definition()
def rrc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = rotate_right_circular(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )

        case _:
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.{register} = rotate_right_circular(reg.{register})")}"
            )
        
@wrap_function_definition()
def rr(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step("z8 = rotate_right(z8)")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )

        case _:
            register = opcode.operand1.lower()
            return (
                f"{step(f"reg.{register} = rotate_right(reg.{register})")}"
            )
        
@wrap_function_definition()
def bit(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step(f"bit(z8, {int(opcode.operand1)})")}"
            )
        
        case _:
            return (
                f"{step(f"bit(reg.{opcode.operand2.lower()}, {int(opcode.operand1)})")}"
            )
        
@wrap_function_definition()
def set_(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step(f"z8 = set(z8, {int(opcode.operand1)})")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )
        
        case _:
            register = opcode.operand2.lower()
            return (
                f"{step(f"reg.{register} = set(reg.{register}, {int(opcode.operand1)})")}"
            )
        
@wrap_function_definition()
def reset(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                f"{step("z8 = mmu->read(reg.hl())")}"
                f"{step(f"z8 = reset(z8, {int(opcode.operand1)})")}"
                f"{step("mmu->write(reg.hl(), z8)")}"
            )
        
        case _:
            register = opcode.operand2.lower()
            return (
                f"{step(f"reg.{register} = reset(reg.{register}, {int(opcode.operand1)})")}"
            )