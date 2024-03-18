// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.instruction;

import util.text.util;
import cmajor.masm.assembly.value;

namespace cmajor::masm::assembly {

const char* opCodeStr[] =
{
    "adc", "adcx", "add", "adox", "and", "andn",
    "bextr", "blcfill", "blci", "blcic", "blcmsk", "blcs", "blsfill", "blsi", "blstc", "blsmsk", "blsr", "bound", "bsf", "bsr", "bswap", "bt", "btc", "btr", "bts", "bzhi",
    "call", "cbw", "cwde", "cdqe", "cwd", "cdq", "cqo", "clc", "cld", "clflush", "clflushopt", "clzero", "cmc",
    "cmovo", "cmovno", "cmovb", "cmovc", "cmovnae", "cmovnb", "cmovnc", "cmovae", "cmovz", "cmove", "cmovnz",
    "cmovne", "cmovbe", "cmovna", "cmovnbe", "cmova", "cmovs", "cmovns", "cmovp", "cmovpe", "cmovnp",
    "cmovpo", "cmovl", "cmovnge", "cmovnl", "cmovge", "cmovle", "cmovng", "cmovnle", "cmovg",
    "cmp", "cmps", "cmpsb", "cmpsw", "cmpsd", "cmpsq", "cmpxchg", "cmpxchg8b", "cmpxchg16b", "cpuid", "crc32",
    "dec", "div",
    "enter",
    "idiv", "imul", "in", "inc", "ins", "insb", "insw", "insd", "int", "into",
    "jo", "jno", "jb", "jc", "jnae", "jnb", "jnc", "jae", "jz", "je", "jnz", "jne", "jbe", "jna", "jnbe", "ja", "js", "jns",
    "jp", "jpe", "jnp", "jpo", "jl", "jnge", "jnl", "jge", "jle", "jng", "jnle", "jg",
    "jcxz", "jecxz", "jrcxz", "jmp",
    "lahf", "lds", "les", "lfs", "lgs", "lss", "lea", "leave", "lfence", "llwpcb", "lods", "lodsb", "lodsw", "lodsd", "lodsq",
    "loop", "loope", "loopne", "loopnz", "loopz", "lwpins", "lwpval", "lzcnt",
    "mcommit", "mfence", "monitorx", "mov", "movbe", "movd", "movmskpd", "movmskps", "movnti", "movs", "movsb", "movsw", "movsd", "movsq",
    "movsx", "movsxd", "movzx", "mul", "mulx", "mwaitx",
    "neg", "nop", "not", "or", "out", "outs", "outsb", "outsw", "outsd",
    "pause", "pdep", "pext", "pop", "popa", "popad", "popcnt", "popf", "popfd", "popfq", "prefetch", "prefetchw", "push", "pusha", "pushad", "pushf", "pushfd", "pushfq",
    "rcl", "rcr", "rdfsbase", "rdgsbase", "rdpid", "rdpru", "rdrand", "rdseed", "ret", "retf", "rol", "ror", "rorx",
    "sahf", "sal", "shl", "sar", "sarx", "sbb", "scas", "scasb", "scasw", "scasd", "scasq",
    "seto", "setno", "setb", "setc", "setnae", "setnb", "setnc", "setae", "setz", "sete", "setnz", "setne", "setbe", "setna", "setnbe", "seta", "sets", "setns", "setp",
    "setpe", "setnp", "setpo", "setl", "setnge", "setnl", "setge",
    "setle", "setng", "setnle", "setg",
    "sfence", "shld", "shlx", "shr", "shrd", "shrx", "slwpcb", "stc", "std", "stos", "stosb", "stosw", "stosd", "stosq", "sub",
    "t1mskc", "test", "tzcnt", "tzmsk",
    "xadd", "xchg", "xlat", "xlatb", "xor", 
    // floating-point support:
    "movdqa", "movq", "movss", "addsd", "addss", "subsd", "subss", "mulsd", "mulss", "divsd", "divss", "ucomisd", "ucomiss", "comisd", "comiss",
    "xorpd", "xorps", "cvtsi2sd", "cvtsi2ss", "cvttsd2si", "cvttss2si", "cvtss2sd", "cvtsd2ss",
    // data definition support:
    "DB", "DW", "DD", "DQ", "REAL4", "REAL8"
};

std::string OpCodeStr(OpCode opCode)
{
    return opCodeStr[int(opCode)];
}

Instruction::Instruction(OpCode opCode_) : opCode(opCode_), nocolon(false)
{
}

void Instruction::SetLabel(const std::string& label_)
{
    label = label_;
}

void Instruction::AddOperand(Value* operand)
{
    operands.push_back(operand);
}

void Instruction::Write(util::CodeFormatter& formatter)
{
    if (!label.empty())
    {
        formatter.DecIndent();
        std::string c(":");
        if (nocolon)
        {
            c = " ";
        }
        formatter.Write(util::Format(label + c, 8, util::FormatWidth::min, util::FormatJustify::left));
    }
    formatter.Write(util::Format(OpCodeStr(opCode), 16));
    bool first = true;
    for (auto& operand : operands)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        formatter.Write(operand->ToString());
    }
    formatter.WriteLine();
    if (!label.empty())
    {
        formatter.IncIndent();
    }
}

int Instruction::Length() const
{
    int length = 0;
    if (!label.empty())
    {
        std::string c = ":";
        length = util::Format(label + c, 8, util::FormatWidth::min, util::FormatJustify::left).length();
    }
    else
    {
        length += 8;
    }
    length += util::Format(OpCodeStr(opCode), 16).length();
    bool first = true;
    for (auto& operand : operands)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            length += 2;
        }
        length += operand->Length();
    }
    return length;
}

Instruction* MakeInst(OpCode opCode, Value* operand)
{
    Instruction* inst = new Instruction(opCode);
    inst->AddOperand(operand);
    return inst;
}

Instruction* MakeInst(OpCode opCode, Value* left, Value* right)
{
    Instruction* inst = new Instruction(opCode);
    inst->AddOperand(left);
    inst->AddOperand(right);
    return inst;
}

} // namespace cmajor::masm::assembly
