// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.instruction;

import cmajor.systemx.machine.opcode;
import std.core;

export namespace cmajor::systemx::machine {

class Processor;
class Formatter;

class Instruction
{
public:
    Instruction(uint8_t opCode_);
    virtual ~Instruction();
    uint8_t OpCode() const { return opCode; }
    virtual void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z);
    virtual void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z);
    bool IsJumpInstruction() const { return IsJumpInst(opCode); }
private:
    uint8_t opCode;
};

class Trap : public Instruction
{
public:
    Trap();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};


class Swym : public Instruction
{
public:
    Swym();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldb : public Instruction
{
public:
    Ldb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldbi : public Instruction
{
public:
    Ldbi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldw : public Instruction
{
public:
    Ldw();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldwi : public Instruction
{
public:
    Ldwi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldt : public Instruction
{
public:
    Ldt();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldti : public Instruction
{
public:
    Ldti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldo : public Instruction
{
public:
    Ldo();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldoi : public Instruction
{
public:
    Ldoi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldbu : public Instruction
{
public:
    Ldbu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldbui : public Instruction
{
public:
    Ldbui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldwu : public Instruction
{
public:
    Ldwu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldwui : public Instruction
{
public:
    Ldwui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldtu : public Instruction
{
public:
    Ldtu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldtui : public Instruction
{
public:
    Ldtui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldou : public Instruction
{
public:
    Ldou();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldoui : public Instruction
{
public:
    Ldoui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldht : public Instruction
{
public:
    Ldht();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldhti : public Instruction
{
public:
    Ldhti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldsf : public Instruction
{
public:
    Ldsf();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ldsfi : public Instruction
{
public:
    Ldsfi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stb : public Instruction
{
public:
    Stb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stbi : public Instruction
{
public:
    Stbi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stw : public Instruction
{
public:
    Stw();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stwi : public Instruction
{
public:
    Stwi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stt : public Instruction
{
public:
    Stt();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stti : public Instruction
{
public:
    Stti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sto : public Instruction
{
public:
    Sto();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stoi : public Instruction
{
public:
    Stoi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stbu : public Instruction
{
public:
    Stbu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stbui : public Instruction
{
public:
    Stbui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stwu : public Instruction
{
public:
    Stwu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stwui : public Instruction
{
public:
    Stwui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sttu : public Instruction
{
public:
    Sttu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sttui : public Instruction
{
public:
    Sttui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stou : public Instruction
{
public:
    Stou();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stoui : public Instruction
{
public:
    Stoui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stht : public Instruction
{
public:
    Stht();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sthti : public Instruction
{
public:
    Sthti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stsf : public Instruction
{
public:
    Stsf();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stsfi : public Instruction
{
public:
    Stsfi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stco : public Instruction
{
public:
    Stco();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Stcoi : public Instruction
{
public:
    Stcoi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Add : public Instruction
{
public:
    Add();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Addi : public Instruction
{
public:
    Addi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sub : public Instruction
{
public:
    Sub();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Subi : public Instruction
{
public:
    Subi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Mul : public Instruction
{
public:
    Mul();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Muli : public Instruction
{
public:
    Muli();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Div : public Instruction
{
public:
    Div();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Divi : public Instruction
{
public:
    Divi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Addu : public Instruction
{
public:
    Addu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Addui : public Instruction
{
public:
    Addui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Subu : public Instruction
{
public:
    Subu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Subui : public Instruction
{
public:
    Subui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Mulu : public Instruction
{
public:
    Mulu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Mului : public Instruction
{
public:
    Mului();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Divu : public Instruction
{
public:
    Divu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Divui : public Instruction
{
public:
    Divui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I2Addu : public Instruction
{
public:
    I2Addu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I2Addui : public Instruction
{
public:
    I2Addui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I4Addu : public Instruction
{
public:
    I4Addu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I4Addui : public Instruction
{
public:
    I4Addui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I8Addu : public Instruction
{
public:
    I8Addu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I8Addui : public Instruction
{
public:
    I8Addui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I16Addu : public Instruction
{
public:
    I16Addu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class I16Addui : public Instruction
{
public:
    I16Addui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Neg : public Instruction
{
public:
    Neg();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Negi : public Instruction
{
public:
    Negi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Negu : public Instruction
{
public:
    Negu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Negui : public Instruction
{
public:
    Negui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sl : public Instruction
{
public:
    Sl();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sli : public Instruction
{
public:
    Sli();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Slu : public Instruction
{
public:
    Slu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Slui : public Instruction
{
public:
    Slui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sr : public Instruction
{
public:
    Sr();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sri : public Instruction
{
public:
    Sri();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sru : public Instruction
{
public:
    Sru();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Srui : public Instruction
{
public:
    Srui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cmp : public Instruction
{
public:
    Cmp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cmpi : public Instruction
{
public:
    Cmpi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cmpu : public Instruction
{
public:
    Cmpu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cmpui : public Instruction
{
public:
    Cmpui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csn : public Instruction
{
public:
    Csn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csni : public Instruction
{
public:
    Csni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csz : public Instruction
{
public:
    Csz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cszi : public Instruction
{
public:
    Cszi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csp : public Instruction
{
public:
    Csp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Cspi : public Instruction
{
public:
    Cspi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csod : public Instruction
{
public:
    Csod();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csodi : public Instruction
{
public:
    Csodi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnn : public Instruction
{
public:
    Csnn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnni : public Instruction
{
public:
    Csnni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnz : public Instruction
{
public:
    Csnz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnzi : public Instruction
{
public:
    Csnzi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnp : public Instruction
{
public:
    Csnp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csnpi : public Instruction
{
public:
    Csnpi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csev : public Instruction
{
public:
    Csev();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Csevi : public Instruction
{
public:
    Csevi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsn : public Instruction
{
public:
    Zsn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsni : public Instruction
{
public:
    Zsni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsz : public Instruction
{
public:
    Zsz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zszi : public Instruction
{
public:
    Zszi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsp : public Instruction
{
public:
    Zsp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zspi : public Instruction
{
public:
    Zspi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsod : public Instruction
{
public:
    Zsod();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsodi : public Instruction
{
public:
    Zsodi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnn : public Instruction
{
public:
    Zsnn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnni : public Instruction
{
public:
    Zsnni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnz : public Instruction
{
public:
    Zsnz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnzi : public Instruction
{
public:
    Zsnzi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnp : public Instruction
{
public:
    Zsnp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsnpi : public Instruction
{
public:
    Zsnpi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsev : public Instruction
{
public:
    Zsev();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Zsevi : public Instruction
{
public:
    Zsevi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class And : public Instruction
{
public:
    And();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andi : public Instruction
{
public:
    Andi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Or : public Instruction
{
public:
    Or();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ori : public Instruction
{
public:
    Ori();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Xor : public Instruction
{
public:
    Xor();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Xori : public Instruction
{
public:
    Xori();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andn : public Instruction
{
public:
    Andn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andni : public Instruction
{
public:
    Andni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Orn : public Instruction
{
public:
    Orn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Orni : public Instruction
{
public:
    Orni();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nand : public Instruction
{
public:
    Nand();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nandi : public Instruction
{
public:
    Nandi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nor : public Instruction
{
public:
    Nor();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nori : public Instruction
{
public:
    Nori();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nxor : public Instruction
{
public:
    Nxor();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Nxori : public Instruction
{
public:
    Nxori();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Mux : public Instruction
{
public:
    Mux();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Muxi : public Instruction
{
public:
    Muxi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sadd : public Instruction
{
public:
    Sadd();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Saddi : public Instruction
{
public:
    Saddi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bdif : public Instruction
{
public:
    Bdif();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bdifi : public Instruction
{
public:
    Bdifi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Wdif : public Instruction
{
public:
    Wdif();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Wdifi : public Instruction
{
public:
    Wdifi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Tdif : public Instruction
{
public:
    Tdif();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Tdifi : public Instruction
{
public:
    Tdifi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Odif : public Instruction
{
public:
    Odif();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Odifi : public Instruction
{
public:
    Odifi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fadd : public Instruction
{
public:
    Fadd();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fsub : public Instruction
{
public:
    Fsub();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fmul : public Instruction
{
public:
    Fmul();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fdiv : public Instruction
{
public:
    Fdiv();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Frem : public Instruction
{
public:
    Frem();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fsqrt : public Instruction
{
public:
    Fsqrt();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fint : public Instruction
{
public:
    Fint();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fcmp : public Instruction
{
public:
    Fcmp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Feql : public Instruction
{
public:
    Feql();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fix : public Instruction
{
public:
    Fix();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Fixu : public Instruction
{
public:
    Fixu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Flot : public Instruction
{
public:
    Flot();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Floti : public Instruction
{
public:
    Floti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Flotu : public Instruction
{
public:
    Flotu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Flotui : public Instruction
{
public:
    Flotui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sflot : public Instruction
{
public:
    Sflot();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sfloti : public Instruction
{
public:
    Sfloti();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sflotu : public Instruction
{
public:
    Sflotu();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Sflotui : public Instruction
{
public:
    Sflotui();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Seth : public Instruction
{
public:
    Seth();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Setmh : public Instruction
{
public:
    Setmh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Setml : public Instruction
{
public:
    Setml();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Setl : public Instruction
{
public:
    Setl();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Inch : public Instruction
{
public:
    Inch();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Incmh : public Instruction
{
public:
    Incmh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Incml : public Instruction
{
public:
    Incml();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Incl : public Instruction
{
public:
    Incl();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Orh : public Instruction
{
public:
    Orh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ormh : public Instruction
{
public:
    Ormh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Orml : public Instruction
{
public:
    Orml();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Orl : public Instruction
{
public:
    Orl();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andnh : public Instruction
{
public:
    Andnh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andnmh : public Instruction
{
public:
    Andnmh();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andnml : public Instruction
{
public:
    Andnml();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Andnl : public Instruction
{
public:
    Andnl();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Jmp : public Instruction
{
public:
    Jmp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Jmpb : public Instruction
{
public:
    Jmpb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Go : public Instruction
{
public:
    Go();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Goi : public Instruction
{
public:
    Goi();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Call : public Instruction
{
public:
    Call();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Calli : public Instruction
{
public:
    Calli();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Ret : public Instruction
{
public:
    Ret();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bn : public Instruction
{
public:
    Bn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnb : public Instruction
{
public:
    Bnb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bz : public Instruction
{
public:
    Bz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bzb : public Instruction
{
public:
    Bzb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bp : public Instruction
{
public:
    Bp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bpb : public Instruction
{
public:
    Bpb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bod : public Instruction
{
public:
    Bod();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bodb : public Instruction
{
public:
    Bodb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnn : public Instruction
{
public:
    Bnn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnnb : public Instruction
{
public:
    Bnnb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnz : public Instruction
{
public:
    Bnz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnzb : public Instruction
{
public:
    Bnzb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnp : public Instruction
{
public:
    Bnp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bnpb : public Instruction
{
public:
    Bnpb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bev : public Instruction
{
public:
    Bev();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Bevb : public Instruction
{
public:
    Bevb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbn : public Instruction
{
public:
    Pbn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnb : public Instruction
{
public:
    Pbnb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbz : public Instruction
{
public:
    Pbz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbzb : public Instruction
{
public:
    Pbzb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbp : public Instruction
{
public:
    Pbp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbpb : public Instruction
{
public:
    Pbpb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbod : public Instruction
{
public:
    Pbod();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbodb : public Instruction
{
public:
    Pbodb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnn : public Instruction
{
public:
    Pbnn();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnnb : public Instruction
{
public:
    Pbnnb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnz : public Instruction
{
public:
    Pbnz();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnzb : public Instruction
{
public:
    Pbnzb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnp : public Instruction
{
public:
    Pbnp();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbnpb : public Instruction
{
public:
    Pbnpb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbev : public Instruction
{
public:
    Pbev();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Pbevb : public Instruction
{
public:
    Pbevb();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Get : public Instruction
{
public:
    Get();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Put : public Instruction
{
public:
    Put();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Geta : public Instruction
{
public:
    Geta();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

class Getab : public Instruction
{
public:
    Getab();
    void Execute(Processor& processor, uint8_t x, uint8_t y, uint8_t z) override;
    void Format(Formatter& formatter, uint8_t x, uint8_t y, uint8_t z) override;
};

} // namespace cmajor::systemx::machine
