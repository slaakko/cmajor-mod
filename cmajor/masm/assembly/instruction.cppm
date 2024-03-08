// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.instruction;

import std.core;
import util.code.formatter;

export namespace cmajor::masm::assembly {

class Value;

enum class OpCode
{
    ADC, ADCX, ADD, ADOX, AND, ANDN,
    BEXTR, BLCFILL, BLCI, BLCIC, BLCMSK, BLCS, BLSFILL, BLSI, BLSIC, BLSMSK, BLSR, BOUND, BSF, BSR, BSWAP, BT, BTC, BTR, BTS, BZHI,
    CALL, CBW, CWDE, CDQE, CWD, CDQ, CQO, CLC, CLD, CLFLUSH, CLFLUSHOPT, CLZERO, CMC,
    CMOVO, CMOVNO, CMOVB, CMOVC, CMOVNAE, CMOVNB, CMOVNC, CMOVAE, CMOVZ, CMOVE, CMOVNZ, CMOVNE, CMOVBE, CMOVNA, CMOVNBE, CMOVA, CMOVS, CMOVNS, CMOVP, CMOVPE, CMOVNP,
    CMOVPO, CMOVL, CMOVNGE, CMOVNL, CMOVGE, CMOVLE, CMOVNG, CMOVNLE, CMOVG,
    CMP, CMPS, CMPSB, CMPSW, CMPSD, CMPSQ, CMPXCHG, CMPXCHG8B, CMPXCHG16B, CPUID, CRC32,
    DEC, DIV,
    ENTER,
    IDIV, IMUL, IN, INC, INS, INSB, INSW, INSD, INT, INTO,
    JO, JNO, JB, JC, JNAE, JNB, JNC, JAE, JZ, JE, JNZ, JNE, JBE, JNA, JNBE, JA, JS, JNS, JP, JPE, JNP, JPO, JL, JNGE, JNL, JGE, JLE, JNG, JNLE, JG,
    JCXZ, JECXZ, JRCXZ, JMP,
    LAHF, LDS, LES, LFS, LGS, LSS, LEA, LEAVE, LFENCE, LLWPCB, LODS, LODSB, LODSW, LODSD, LODSQ, LOOP, LOOPE, LOOPNE, LOOPNZ, LOOPZ, LWPINS, LWPVAL, LZCNT,
    MCOMMIT, MFENCE, MONITORX, MOV, MOVBE, MOVD, MOVMSKPD, MOVMSKPS, MOVNTI, MOVS, MOVSB, MOVSW, MOVSD, MOVSQ, MOVSX, MOVSXD, MOVZX, MUL, MULX, MWAITX,
    NEG, NOP, NOT, OR, OUT, OUTS, OUTSB, OUTSW, OUTSD,
    PAUSE, PDEP, PEXT, POP, POPA, POPAD, POPCNT, POPF, POPFD, POPFQ, PREFETCH, PREFETCHW, PUSH, PUSHA, PUSHAD, PUSHF, PUSHFD, PUSHFQ,
    RCL, RCR, RDFSBASE, RDGSBASE, RDPID, RDPRU, RDRAND, RDSEED, RET, RETF, ROL, ROR, RORX,
    SAHF, SAL, SHL, SAR, SARX, SBB, SCAS, SCASB, SCASW, SCASD, SCASQ,
    SETO, SETNO, SETB, SETC, SETNAE, SETNB, SETNC, SETAE, SETZ, SETE, SETNZ, SETNE, SETBE, SETNA, SETNBE, SETA, SETS, SETNS, SETP, SETPE, SETNP, SETPO, SETL, SETNGE, SETNL, SETGE,
    SETLE, SETNG, SETNLE, SETG,
    SFENCE, SHLD, SHLX, SHR, SHRD, SHRX, SLWPCB, STC, STD, STOS, STOSB, STOSW, STOSD, STOSQ, SUB,
    T1MSKC, TEST, TZCNT, TZMSK,
    XADD, XCHG, XLAT, XLATB, XOR,
    DB, DW, DD, DQ
};

std::string OpCodeStr(OpCode opCode);

class Instruction
{
public:
    Instruction(OpCode opCode_);
    void SetLabel(const std::string& label_);
    const std::string& Label() const { return label; }
    void AddOperand(Value* operand);
    OpCode GetOpCode() const { return opCode; }
    const std::vector<Value*>& Operands() const { return operands; }
    void Write(util::CodeFormatter& formatter);
private:
    std::string label;
    OpCode opCode;
    std::vector<Value*> operands;
};

Instruction* MakeInst(OpCode opCode, Value* operand);
Instruction* MakeInst(OpCode opCode, Value* left, Value* right);

} // namespace cmajor::masm::assembly
