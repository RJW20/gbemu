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

                case "LD":
                    f.write(ld(opcode))
                case "LDH":
                    f.write(ldh(opcode))
                case "PUSH":
                    f.write(push(opcode))
                case "POP":
                    f.write(pop(opcode))
                case "ADD":
                    f.write(add(opcode))
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
                    f.write(cp(opcode))
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
                case "NOP" | "HALT" | "STOP" | "PREFIX":
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
                case "SLA":
                    f.write(sla(opcode))
                case "SRA":
                    f.write(sra(opcode))
                case "SRL":
                    f.write(srl(opcode))
                case "BIT":
                    f.write(bit(opcode))
                case "SET":
                    f.write(set_(opcode))
                case "RES":
                    f.write(res(opcode))
                case "JP":
                    f.write(jp(opcode))
                case "JR":
                    f.write(jr(opcode))
                case "CALL":
                    f.write(call(opcode))
                case "RST":
                    f.write(rst(opcode))
                case "RET":
                    f.write(ret(opcode))
                case "RETI":
                    f.write(reti(opcode))

            f.write("\n")


def wrap_function_definition() -> Callable[
    [Callable[[Opcode], str]], Callable[[Opcode], str]
]:
    """Decorator to wrap a functions output with the Opcode that it was called
    with's function_start and function_end."""

    def decorator(func: Callable[[Opcode], str]) -> Callable[[Opcode], str]:
        def wrapped(opcode: Opcode) -> str:
            return (
                opcode.function_start +
                func(opcode) +
                opcode.function_end
            )
        return wrapped
    return decorator

def step(operation: str) -> str:
    """Return an operation encased in a push_back to the steps vector."""
    return f"{INDENT}steps.push_back([this] {{ {operation} }});\n"

@wrap_function_definition()
def ld(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(a16)":
            match opcode.operand2:

                case "SP":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                        step("mmu->write(z16, reg.sp & 0xFF);") +
                        step(
                            "\n"
                            f"{INDENT}{INDENT}mmu->write(static_cast<uint16_t>(z16 + 1), reg.sp >> 8);\n"
                            "   "
                        )
                    )
       
                case "A":
                    return(
                        step("z8 = mmu->read(reg.pc++);") +
                        step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                        step("mmu->write(z16, reg.a);")
                    )

        case "(HL+)":
            return step(
                "mmu->write(reg.hl(), reg.a);\n"
                f"{INDENT}{INDENT}reg.set_hl(reg.hl() + 1);"
            )

        case "(HL-)":
            return step(
                "mmu->write(reg.hl(), reg.a);\n"
                f"{INDENT}{INDENT}reg.set_hl(reg.hl() - 1);"
            )

        case "(HL)":
            match opcode.operand2:

                case "d8":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("mmu->write(reg.hl(), z8);")
                    )
       
                case _:
                    return step(f"mmu->write(reg.hl(), reg.{opcode.operand2.lower()});")

        case "(BC)" | "(DE)":
            register = opcode.operand1[1:3].lower()
            return step(f"mmu->write(reg.{register}(), reg.a);")

        case "BC" | "DE" | "HL":
            match opcode.operand2:

                case "SP+r8":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("reg.set_hl(add_signed8(reg.sp, z8));")
                    )
       
                case _:
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                        step(f"reg.set_{opcode.operand1.lower()}(z16);")
                    )


        case "SP":
            match opcode.operand2:
       
                case "d16":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                        step("reg.sp = z16;")
                    )
   
                case "HL":
                    return step("reg.sp = reg.hl();")

        case "A":
            match opcode.operand2:

                case "(a16)":
                    return(
                        step("z8 = mmu->read(reg.pc++);") +
                        step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                        step("reg.a = mmu->read(z16);")
                    )
       
                case "(HL+)":
                    return (
                        step(
                            "z8 = mmu->read(reg.hl());\n"
                            f"{INDENT}{INDENT}reg.set_hl(reg.hl() + 1);"
                        ) +
                        step("reg.a = z8;")
                    )
       
                case "(HL-)":
                    return (
                        step(
                            "z8 = mmu->read(reg.hl());\n"
                            f"{INDENT}{INDENT}reg.set_hl(reg.hl() - 1);"
                        ) +
                        step("reg.a = z8;")
                    )
       
                case "(BC)" | "(DE)" | "(HL)":
                    register = opcode.operand2[1:3].lower()
                    return (
                        step(f"z8 = mmu->read(reg.{register}());") +
                        step("reg.a = z8;")
                    )
       
                case "d8":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step("reg.a = z8;")
                    )
       
                case _:
                    return step(f"reg.a = reg.{opcode.operand2.lower()};")

        case _:
            match opcode.operand2:
       
                case "(HL)":
                    return (
                        step("z8 = mmu->read(reg.hl());") +
                        step(f"reg.{opcode.operand1.lower()} = z8;")
                    )
       
                case "d8":
                    return (
                        step("z8 = mmu->read(reg.pc++);") +
                        step(f"reg.{opcode.operand1.lower()} = z8;")
                    )
       
                case _:
                    to_reg = opcode.operand1.lower()
                    from_reg = opcode.operand2.lower()
                    return step(f"reg.{to_reg} = reg.{from_reg};")

@wrap_function_definition()
def ldh(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(a8)":
            return(
                step("z8 = mmu->read(reg.pc++);") +
                step(
                    "\n"
                    f"{INDENT}{INDENT}mmu->write(static_cast<uint16_t>(0xFF00 + z8), reg.a);\n"
                    "   "
                )
            )

        case "(C)":
            return(
                step(
                    "\n"
                    f"{INDENT}{INDENT}mmu->write(static_cast<uint16_t>(0xFF00 + reg.c), reg.a);\n"
                    "   "
                )
            )

        case "A":
            match opcode.operand2:

                case "(a8)":
                    return(
                        step("z8 = mmu->read(reg.pc++);") +
                        step(
                            "\n"
                            f"{INDENT}{INDENT}z8 = mmu->read(static_cast<uint16_t>(0xFF00 + z8));\n"
                            "   "
                        ) +
                        step("reg.a = z8;")
                    )
       
                case "(C)":
                    return(
                        step(
                            "\n"
                            f"{INDENT}{INDENT}z8 = mmu->read(static_cast<uint16_t>(0xFF00 + reg.c));\n"
                            "   "
                        ) +
                        step("reg.a = z8;")
                    )
       
@wrap_function_definition()
def push(opcode: Opcode) -> str:

    if opcode.operand1 == "AF":

        return (
            step("reg.sp--;") +
            step("mmu->write(reg.sp--, reg.a);") +
            step("mmu->write(reg.sp, reg.f());")
        )

    return (
        step("reg.sp--;") +
        step(f"mmu->write(reg.sp--, reg.{opcode.operand1.lower()[0]});") +
        step(f"mmu->write(reg.sp, reg.{opcode.operand1.lower()[1]});")
    )

@wrap_function_definition()
def pop(opcode: Opcode) -> str:

    return (
        step("z8 = mmu->read(reg.sp++);") +
        step("z16 = (mmu->read(reg.sp++) << 8) | z8;") +
        step(f"reg.set_{opcode.operand1.lower()}(z16);")
    )

@wrap_function_definition()
def add(opcode: Opcode) -> str:

    if opcode.operand1 != "HL":

        match opcode.operand2:

            case "(HL)":
                return (
                    step("z8 = mmu->read(reg.hl());") +
                    step("add(z8);")
                )
   
            case "d8":
                return (
                    step("z8 = mmu->read(reg.pc++);") +
                    step("add(z8);")
                )
   
            case "r8":
                return (
                    step("z8 = mmu->read(reg.pc++);") +
                    step("z16 = add_signed8(reg.sp, z8);") +
                    step("reg.sp = z16;")
                )

            case _:
                return (
                    step(f"add(reg.{opcode.operand2.lower()});")
                )
   
    if opcode.operand2 != "SP":
        return step(f"add_hl(reg.{opcode.operand2.lower()}());")

    return step("add_hl(reg.sp);")

@wrap_function_definition()
def adc(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("add_with_carry(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("add_with_carry(z8);")
            )

        case _:
            return (
                step(f"add_with_carry(reg.{opcode.operand2.lower()});")
            )

@wrap_function_definition()
def sub(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("subtract(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("subtract(z8);")
            )

        case _:
            return (
                step(f"subtract(reg.{opcode.operand1.lower()});")
            )

@wrap_function_definition()
def sbc(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("subtract_with_carry(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("subtract_with_carry(z8);")
            )

        case _:
            return (
                step(f"subtract_with_carry(reg.{opcode.operand2.lower()});")
            )

@wrap_function_definition()
def and_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("and_(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("and_(z8);")
            )

        case _:
            return (
                step(f"and_(reg.{opcode.operand1.lower()});")
            )

@wrap_function_definition()
def or_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("or_(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("or_(z8);")
            )

        case _:
            return (
                step(f"or_(reg.{opcode.operand1.lower()});")
            )

@wrap_function_definition()
def xor_(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("xor_(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("xor_(z8);")
            )

        case _:
            return (
                step(f"xor_(reg.{opcode.operand1.lower()});")
            )

@wrap_function_definition()
def cp(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("compare(z8);")
            )

        case "d8":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("compare(z8);")
            )

        case _:
            return (
                step(f"compare(reg.{opcode.operand1.lower()});")
            )

@wrap_function_definition()
def inc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), increment(z8));")
            )

        case "BC" | "DE" | "HL":
            register = opcode.operand1.lower()
            return (
                step(f"reg.set_{register}(reg.{register}() + 1);")
            )

        case "SP":
            return (
                step("reg.sp++;")
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = increment(reg.{register});")
            )

@wrap_function_definition()
def dec(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), decrement(z8));")
            )

        case "BC" | "DE" | "HL":
            register = opcode.operand1.lower()
            return (
                step(f"reg.set_{register}(reg.{register}() - 1);")
            )

        case "SP":
            return (
                step("reg.sp--;")
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = decrement(reg.{register});")
            )

@wrap_function_definition()
def swap(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), swap(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = swap(reg.{register});")
            )

@wrap_function_definition()
def daa(opcode: Opcode) -> str:

    return step("decimal_adjust_accumulator();")

@wrap_function_definition()
def cpl(opcode: Opcode) -> str:

    return step("complement_accumulator();")

@wrap_function_definition()
def ccf(opcode: Opcode) -> str:

    return step("complement_carry_flag();")

@wrap_function_definition()
def scf(opcode: Opcode) -> str:

    return step("set_carry_flag();")

@wrap_function_definition()
def nop(opcode: Opcode) -> str:

    return step("")

@wrap_function_definition()
def di(opcode: Opcode) -> str:

    return step(
        "interrupt_manager->disable_interrupts();\n"
        f"{INDENT}{INDENT}interrupt_enable_scheduled = false; "
        f"interrupt_enable_delay = 0;"
    )

@wrap_function_definition()
def ei(opcode: Opcode) -> str:

    return step("interrupt_enable_scheduled = true;")

@wrap_function_definition()
def rlca(opcode: Opcode) -> str:

    return step(
        "reg.a = rotate_left_circular(reg.a);\n"
        f"{INDENT}{INDENT}reg.flag_z = false;"
    )

@wrap_function_definition()
def rla(opcode: Opcode) -> str:

    return step(
        "reg.a = rotate_left(reg.a);\n"
        f"{INDENT}{INDENT}reg.flag_z = false;"
    )

@wrap_function_definition()
def rrca(opcode: Opcode) -> str:

    return step(
        "reg.a = rotate_right_circular(reg.a);\n"
        f"{INDENT}{INDENT}reg.flag_z = false;"
    )

@wrap_function_definition()
def rra(opcode: Opcode) -> str:

    return step(
        "reg.a = rotate_right(reg.a);\n"
        f"{INDENT}{INDENT}reg.flag_z = false;"
    )

@wrap_function_definition()
def rlc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step(
                    "\n"
                    f"{INDENT}{INDENT}mmu->write(reg.hl(), rotate_left_circular(z8));\n"
                    "   "
                )
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = rotate_left_circular(reg.{register});")
            )

@wrap_function_definition()
def rl(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), rotate_left(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = rotate_left(reg.{register});")
            )

@wrap_function_definition()
def rrc(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step(
                    "\n"
                    f"{INDENT}{INDENT}mmu->write(reg.hl(), rotate_right_circular(z8));\n"
                    "   "
                )
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = rotate_right_circular(reg.{register});")
            )

@wrap_function_definition()
def rr(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), rotate_right(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return (
                step(f"reg.{register} = rotate_right(reg.{register});")
            )

@wrap_function_definition()
def sla(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), shift_left_arithmetic(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return step(f"reg.{register} = shift_left_arithmetic(reg.{register});")

@wrap_function_definition()
def sra(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), shift_right_arithmetic(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return step(f"reg.{register} = shift_right_arithmetic(reg.{register});")

@wrap_function_definition()
def srl(opcode: Opcode) -> str:

    match opcode.operand1:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step("mmu->write(reg.hl(), shift_right_logical(z8));")
            )

        case _:
            register = opcode.operand1.lower()
            return step(f"reg.{register} = shift_right_logical(reg.{register});")

@wrap_function_definition()
def bit(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step(f"bit(z8, {int(opcode.operand1)});")
            )

        case _:
            return (
                step(f"bit(reg.{opcode.operand2.lower()}, {int(opcode.operand1)});")
            )

@wrap_function_definition()
def set_(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step(f"mmu->write(reg.hl(), set(z8, {int(opcode.operand1)}));")
            )

        case _:
            register = opcode.operand2.lower()
            return (
                step(f"reg.{register} = set(reg.{register}, {int(opcode.operand1)});")
            )

@wrap_function_definition()
def res(opcode: Opcode) -> str:

    match opcode.operand2:

        case "(HL)":
            return (
                step("z8 = mmu->read(reg.hl());") +
                step(f"mmu->write(reg.hl(), reset(z8, {int(opcode.operand1)}));")
            )

        case _:
            register = opcode.operand2.lower()
            return (
                step(f"reg.{register} = reset(reg.{register}, {int(opcode.operand1)});")
            )

@wrap_function_definition()
def jp(opcode: Opcode) -> str:

    match opcode.operand1:

        case "a16":
            return (
                step("z8 = mmu->read(reg.pc++);") +
                step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
                step("reg.pc = z16;")
            )

        case "HL":
            return step("reg.pc = reg.hl();")

        case _:

            flag_condition = ""
            match opcode.operand1:
                case "NZ":
                    flag_condition = "reg.flag_z"
                case "Z":
                    flag_condition = "!reg.flag_z"
                case "NC":
                    flag_condition = "reg.flag_c"
                case "C":
                    flag_condition = "!reg.flag_c"

            return (
                step("z8 = mmu->read(reg.pc++);") +
                step(
                    "z16 = (mmu->read(reg.pc++) << 8) | z8;\n"
                    f"{INDENT}{INDENT}if ({flag_condition}) early_exit = true;"
                ) +
                step("reg.pc = z16;")
            )

@wrap_function_definition()
def jr(opcode: Opcode) -> str:

    if opcode.operand1 == "r8":

        return (
            step("z8 = mmu->read(reg.pc++);") +
            step(
                "\n"
                f"{INDENT}{INDENT}reg.pc = static_cast<uint16_t>(reg.pc + static_cast<int8_t>(z8));\n"
                "   "
            )
        )

    flag_condition = ""
    match opcode.operand1:
        case "NZ":
            flag_condition = "reg.flag_z"
        case "Z":
            flag_condition = "!reg.flag_z"
        case "NC":
            flag_condition = "reg.flag_c"
        case "C":
            flag_condition = "!reg.flag_c"

    return (
        step(
            "z8 = mmu->read(reg.pc++);\n"
            f"{INDENT}{INDENT}if ({flag_condition}) early_exit = true;"
        ) +
        step(
            "\n"
            f"{INDENT}{INDENT}reg.pc = static_cast<uint16_t>(reg.pc + static_cast<int8_t>(z8));\n"
            "   "
        )
    )

@wrap_function_definition()
def call(opcode: Opcode) -> str:

    if opcode.operand1 == "a16":

        return (
            step("z8 = mmu->read(reg.pc++);") +
            step("z16 = (mmu->read(reg.pc++) << 8) | z8;") +
            step("reg.sp--;") +
            step("mmu->write(reg.sp--, reg.pc >> 8);") +
            step(
                "mmu->write(reg.sp, reg.pc & 0xFF);\n"
                f"{INDENT}{INDENT}reg.pc = z16;"
            )
        )

    flag_condition = ""
    match opcode.operand1:
        case "NZ":
            flag_condition = "reg.flag_z"
        case "Z":
            flag_condition = "!reg.flag_z"
        case "NC":
            flag_condition = "reg.flag_c"
        case "C":
            flag_condition = "!reg.flag_c"

    return (
        step("z8 = mmu->read(reg.pc++);") +
        step(
            "z16 = (mmu->read(reg.pc++) << 8) | z8;\n"
            f"{INDENT}{INDENT}if ({flag_condition}) early_exit = true;"
        ) +
        step("reg.sp--;") +
        step("mmu->write(reg.sp--, reg.pc >> 8);") +
        step(
            "mmu->write(reg.sp, reg.pc & 0xFF);\n"
            f"{INDENT}{INDENT}reg.pc = z16;"
        )
    )

@wrap_function_definition()
def rst(opcode: Opcode) -> str:

    return (
        step("reg.sp--;") +
        step("mmu->write(reg.sp--, reg.pc >> 8);") +
        step(
            "mmu->write(reg.sp, reg.pc & 0xFF);\n"
            f"{INDENT}{INDENT}reg.pc = 0x{opcode.operand1[:2]};"
        )
    )

@wrap_function_definition()
def ret(opcode: Opcode) -> str:

    if opcode.operand1 is None:

        return (
            step("z8 = mmu->read(reg.sp++);") +
            step("z16 = (mmu->read(reg.sp++) << 8) | z8;") +
            step("reg.pc = z16;")
        )

    flag_condition = ""
    match opcode.operand1:
        case "NZ":
            flag_condition = "reg.flag_z"
        case "Z":
            flag_condition = "!reg.flag_z"
        case "NC":
            flag_condition = "reg.flag_c"
        case "C":
            flag_condition = "!reg.flag_c"

    return (
        step(f"if ({flag_condition}) early_exit = true;") +
        step("z8 = mmu->read(reg.sp++);") +
        step("z16 = (mmu->read(reg.sp++) << 8) | z8;") +
        step("reg.pc = z16;")
    )

@wrap_function_definition()
def reti(opcode: Opcode) -> str:

    return (
        step("z8 = mmu->read(reg.sp++);") +
        step("z16 = (mmu->read(reg.sp++) << 8) | z8;") +
        step(
            "reg.pc = z16;\n"
            f"{INDENT}{INDENT}interrupt_manager->enable_interrupts();"
        )
    )