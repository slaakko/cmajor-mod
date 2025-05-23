// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.machine.machine;

import cmajor.systemx.machine.processor;
import cmajor.systemx.machine.config;

namespace cmajor::systemx::machine {

Machine::Machine() : clock(), scheduler(nullptr), processors(), memory(*this), insts(), exiting(false), hasException(false), started(false)
{
    clock.SetMachine(this);
    int numProcessors = NumProcessors();
    processors.resize(numProcessors);
    for (int i = 0; i < numProcessors; ++i)
    {
        Processor& processor = processors[i];
        processor.SetId(i);
        processor.SetMachine(this);
    }
    instructions.resize(256);
    for (int i = 0; i < 256; ++i)
    {
        SetInstruction(new Instruction(i));
    }
    SetInstruction(new Trap());
    SetInstruction(new Swym());
    SetInstruction(new Ldb());
    SetInstruction(new Ldbi());
    SetInstruction(new Ldw());
    SetInstruction(new Ldwi());
    SetInstruction(new Ldt());
    SetInstruction(new Ldti());
    SetInstruction(new Ldo());
    SetInstruction(new Ldoi());
    SetInstruction(new Ldbu());
    SetInstruction(new Ldbui());
    SetInstruction(new Ldwu());
    SetInstruction(new Ldwui());
    SetInstruction(new Ldtu());
    SetInstruction(new Ldtui());
    SetInstruction(new Ldou());
    SetInstruction(new Ldoui());
    SetInstruction(new Ldht());
    SetInstruction(new Ldhti());
    SetInstruction(new Ldsf());
    SetInstruction(new Ldsfi());
    SetInstruction(new Stb());
    SetInstruction(new Stbi());
    SetInstruction(new Stw());
    SetInstruction(new Stwi());
    SetInstruction(new Stt());
    SetInstruction(new Stti());
    SetInstruction(new Sto());
    SetInstruction(new Stoi());
    SetInstruction(new Stbu());
    SetInstruction(new Stbui());
    SetInstruction(new Stwu());
    SetInstruction(new Stwui());
    SetInstruction(new Sttu());
    SetInstruction(new Sttui());
    SetInstruction(new Stou());
    SetInstruction(new Stoui());
    SetInstruction(new Stht());
    SetInstruction(new Sthti());
    SetInstruction(new Stsf());
    SetInstruction(new Stsfi());
    SetInstruction(new Stco());
    SetInstruction(new Stcoi());
    SetInstruction(new Add());
    SetInstruction(new Addi());
    SetInstruction(new Sub());
    SetInstruction(new Subi());
    SetInstruction(new Mul());
    SetInstruction(new Muli());
    SetInstruction(new Div());
    SetInstruction(new Divi());
    SetInstruction(new Addu());
    SetInstruction(new Addui());
    SetInstruction(new Subu());
    SetInstruction(new Subui());
    SetInstruction(new Mulu());
    SetInstruction(new Mului());
    SetInstruction(new Divu());
    SetInstruction(new Divui());
    SetInstruction(new I2Addu());
    SetInstruction(new I2Addui());
    SetInstruction(new I4Addu());
    SetInstruction(new I4Addui());
    SetInstruction(new I8Addu());
    SetInstruction(new I8Addui());
    SetInstruction(new I16Addu());
    SetInstruction(new I16Addui());
    SetInstruction(new Neg());
    SetInstruction(new Negi());
    SetInstruction(new Negu());
    SetInstruction(new Negui());
    SetInstruction(new Sl());
    SetInstruction(new Sli());
    SetInstruction(new Slu());
    SetInstruction(new Slui());
    SetInstruction(new Sr());
    SetInstruction(new Sri());
    SetInstruction(new Sru());
    SetInstruction(new Srui());
    SetInstruction(new Cmp());
    SetInstruction(new Cmpi());
    SetInstruction(new Cmpu());
    SetInstruction(new Cmpui());
    SetInstruction(new Csn());
    SetInstruction(new Csni());
    SetInstruction(new Csz());
    SetInstruction(new Cszi());
    SetInstruction(new Csp());
    SetInstruction(new Cspi());
    SetInstruction(new Csod());
    SetInstruction(new Csodi());
    SetInstruction(new Csnn());
    SetInstruction(new Csnni());
    SetInstruction(new Csnz());
    SetInstruction(new Csnzi());
    SetInstruction(new Csnp());
    SetInstruction(new Csnpi());
    SetInstruction(new Csev());
    SetInstruction(new Csevi());
    SetInstruction(new Zsn());
    SetInstruction(new Zsni());
    SetInstruction(new Zsz());
    SetInstruction(new Zszi());
    SetInstruction(new Zsp());
    SetInstruction(new Zspi());
    SetInstruction(new Zsod());
    SetInstruction(new Zsodi());
    SetInstruction(new Zsnn());
    SetInstruction(new Zsnni());
    SetInstruction(new Zsnz());
    SetInstruction(new Zsnzi());
    SetInstruction(new Zsnp());
    SetInstruction(new Zsnpi());
    SetInstruction(new Zsev());
    SetInstruction(new Zsevi());
    SetInstruction(new And());
    SetInstruction(new Andi());
    SetInstruction(new Or());
    SetInstruction(new Ori());
    SetInstruction(new Xor());
    SetInstruction(new Xori());
    SetInstruction(new Andn());
    SetInstruction(new Andni());
    SetInstruction(new Orn());
    SetInstruction(new Orni());
    SetInstruction(new Nand());
    SetInstruction(new Nandi());
    SetInstruction(new Nor());
    SetInstruction(new Nori());
    SetInstruction(new Nxor());
    SetInstruction(new Nxori());
    SetInstruction(new Mux());
    SetInstruction(new Muxi());
    SetInstruction(new Sadd());
    SetInstruction(new Saddi());
    SetInstruction(new Bdif());
    SetInstruction(new Bdifi());
    SetInstruction(new Wdif());
    SetInstruction(new Wdifi());
    SetInstruction(new Tdif());
    SetInstruction(new Tdifi());
    SetInstruction(new Odif());
    SetInstruction(new Odifi());
    SetInstruction(new Fadd());
    SetInstruction(new Fsub());
    SetInstruction(new Fmul());
    SetInstruction(new Fdiv());
    SetInstruction(new Frem());
    SetInstruction(new Fsqrt());
    SetInstruction(new Fint());
    SetInstruction(new Fcmp());
    SetInstruction(new Feql());
    SetInstruction(new Fix());
    SetInstruction(new Fixu());
    SetInstruction(new Flot());
    SetInstruction(new Floti());
    SetInstruction(new Flotu());
    SetInstruction(new Flotui());
    SetInstruction(new Sflot());
    SetInstruction(new Sfloti());
    SetInstruction(new Sflotu());
    SetInstruction(new Sflotui());
    SetInstruction(new Seth());
    SetInstruction(new Setmh());
    SetInstruction(new Setml());
    SetInstruction(new Setl());
    SetInstruction(new Inch());
    SetInstruction(new Incmh());
    SetInstruction(new Incml());
    SetInstruction(new Incl());
    SetInstruction(new Orh());
    SetInstruction(new Ormh());
    SetInstruction(new Orml());
    SetInstruction(new Orl());
    SetInstruction(new Andnh());
    SetInstruction(new Andnmh());
    SetInstruction(new Andnml());
    SetInstruction(new Andnl());
    SetInstruction(new Jmp());
    SetInstruction(new Jmpb());
    SetInstruction(new Go());
    SetInstruction(new Goi());
    SetInstruction(new Call());
    SetInstruction(new Calli());
    SetInstruction(new Ret());
    SetInstruction(new Bn());
    SetInstruction(new Bnb());
    SetInstruction(new Bz());
    SetInstruction(new Bzb());
    SetInstruction(new Bp());
    SetInstruction(new Bpb());
    SetInstruction(new Bod());
    SetInstruction(new Bodb());
    SetInstruction(new Bnn());
    SetInstruction(new Bnnb());
    SetInstruction(new Bnz());
    SetInstruction(new Bnzb());
    SetInstruction(new Bnp());
    SetInstruction(new Bnpb());
    SetInstruction(new Bev());
    SetInstruction(new Bevb());
    SetInstruction(new Pbn());
    SetInstruction(new Pbnb());
    SetInstruction(new Pbz());
    SetInstruction(new Pbzb());
    SetInstruction(new Pbp());
    SetInstruction(new Pbpb());
    SetInstruction(new Pbod());
    SetInstruction(new Pbodb());
    SetInstruction(new Pbnn());
    SetInstruction(new Pbnnb());
    SetInstruction(new Pbnz());
    SetInstruction(new Pbnzb());
    SetInstruction(new Pbnp());
    SetInstruction(new Pbnpb());
    SetInstruction(new Pbev());
    SetInstruction(new Pbevb());
    SetInstruction(new Get());
    SetInstruction(new Put());
    SetInstruction(new Geta());
    SetInstruction(new Getab());
    for (int i = 0; i < 256; ++i)
    {
        insts[i] = instructions[i].get();
    }
}

Machine::~Machine()
{
}

void Machine::SetInstruction(Instruction* inst)
{
    instructions[inst->OpCode()].reset(inst);
}

void Machine::Start()
{
    started = true;
    for (auto& processor : processors)
    {
        processor.Start();
    }
    for (auto& processor : processors)
    {
        processor.EnableInterrupts();
    }
    clock.Start();
}

void Machine::Exit()
{
    exiting = true;
    NotifyObservers();
    clock.Stop();
    scheduler->Stop();
    for (auto& processor : processors)
    {
        processor.Stop();
    }
}

void Machine::SetExiting()
{
    exiting = true;
    NotifyObservers();
}

void Machine::SetHasException()
{
    hasException = true;
    NotifyObservers();
}

void Machine::SetException(std::exception_ptr&& exception_)
{
    exception = exception_;
    SetHasException();
}

void Machine::CheckExceptions()
{
    if (hasException)
    {
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    for (auto& processor : processors)
    {
        processor.CheckException();
    }
}

void Machine::AddObserver(MachineObserver* observer)
{
    observers.push_back(observer);
}

void Machine::NotifyObservers()
{
    for (auto& observer : observers)
    {
        observer->MachineStateChanged();
    }
}

bool Machine::HasRunnableKernelProcess()
{
    for (auto& processor : processors)
    {
        if (processor.HasRunnableKernelProcess()) return true;
    }
    return false;
}

MachineObserver::~MachineObserver()
{
}

} // cmajor::systemx::machine
