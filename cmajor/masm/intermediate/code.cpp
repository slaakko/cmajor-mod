// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.masm.intermediate.code;

import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.visitor;
import cmajor.masm.intermediate.error;
import cmajor.masm.intermediate.util;
import util;

namespace cmajor::masm::intermediate {

const char* opCodeStr[] =
{
    "store", "arg", "jmp", "branch", "call", "ret", "switch",
    "not", "neg", "signextend", "zeroextend", "fpextend", "truncate", "bitcast", "inttofloat", "floattoint", "inttoptr", "ptrtoint",
    "add", "sub", "mul", "div", "mod", "and", "or", "xor", "shl", "shr", "equal", "less",
    "param", "local", "load", "elemaddr", "ptroffset", "ptrdiff", "call", "nop"
};

RegValue::RegValue(const soul::ast::Span& span_, Type* type_, int32_t reg_) : Value(span_, ValueKind::regValue, type_), reg(reg_), inst(nullptr)
{
}

RegValue::~RegValue()
{
    if (inst && inst->IsValueInstruction())
    {
        ValueInstruction* valueInst = static_cast<ValueInstruction*>(inst);
        valueInst->ResetResult();
    }
}

std::string RegValue::ToString() const
{
    return "$" + std::to_string(inst->RegValueIndex());
}

Value* RegValue::Clone(CloneContext& cloneContext) const
{
    RegValue* regValue = cloneContext.CurrentFunction()->GetRegValue(reg);
    if (regValue)
    {
        return regValue;
    }
    RegValue* clone = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), reg, cloneContext.GetContext());
    Instruction* mappedInst = cloneContext.GetMappedInstruction(inst);
    if (mappedInst)
    {
        clone->SetInst(mappedInst);
    }
    else
    {
        cloneContext.AddUnmappedInstruction(inst, clone);
    }
    return clone;
}

void AddUser(Instruction* user, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                regValue->Inst()->AddUser(user);
            }
        }
    }
}

void RemoveUser(Instruction* user, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                regValue->Inst()->RemoveUser(user);
            }
        }
    }
}

void AddToUsesVec(std::vector<Instruction*>& uses, Value* value)
{
    if (value)
    {
        if (value->IsRegValue())
        {
            RegValue* regValue = static_cast<RegValue*>(value);
            if (regValue->Inst())
            {
                uses.push_back(regValue->Inst());
            }
        }
    }
}

Instruction::Instruction(const soul::ast::Span& span_, Type* type_, OpCode opCode_) : 
    Value(span_, ValueKind::instruction, type_), opCode(opCode_), index(-1), regValueIndex(-1), assemblyIndex(-1), leader(false)
{
}

void Instruction::Check()
{
    for (Instruction* user : users)
    {
        if (user->opCode < OpCode::store || user->opCode > OpCode::nop)
        {
            Assert(false, "invalid instruction");
        }
    }
}

std::string Instruction::Name() const
{
    return opCodeStr[static_cast<int>(opCode)];
}

bool Instruction::IsLeader() const
{
    if (leader) return true;
    BasicBlock* basicBlock = Parent();
    return this == basicBlock->FirstInstruction();
}

bool Instruction::IsTerminator() const
{
    switch (opCode)
    {
        case OpCode::jmp:
        case OpCode::branch:
        case OpCode::ret:
        case OpCode::switch_:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Instruction::IsValueInstruction() const
{
    switch (opCode)
    {
        case OpCode::not_:
        case OpCode::neg:
        case OpCode::signextend:
        case OpCode::zeroextend:
        case OpCode::fpextend:
        case OpCode::truncate:
        case OpCode::bitcast:
        case OpCode::inttofloat:
        case OpCode::floattoint:
        case OpCode::inttoptr:
        case OpCode::ptrtoint:
        case OpCode::add:
        case OpCode::sub:
        case OpCode::mul:
        case OpCode::div_:
        case OpCode::mod:
        case OpCode::and_:
        case OpCode::or_:
        case OpCode::xor_:
        case OpCode::shl:
        case OpCode::shr:
        case OpCode::equal:
        case OpCode::less:
        case OpCode::param:
        case OpCode::local:
        case OpCode::load:
        case OpCode::elemaddr:
        case OpCode::ptroffset:
        case OpCode::ptrdiff:
        case OpCode::function_call:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Instruction::IsUnaryInstruction() const
{
    switch (opCode)
    {
        case OpCode::not_:
        case OpCode::neg:
        case OpCode::signextend:
        case OpCode::zeroextend:
        case OpCode::fpextend:
        case OpCode::truncate:
        case OpCode::bitcast:
        case OpCode::inttofloat:
        case OpCode::floattoint:
        case OpCode::inttoptr:
        case OpCode::ptrtoint:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Instruction::IsBinaryInstruction() const
{
    switch (opCode)
    {
        case OpCode::add:
        case OpCode::sub:
        case OpCode::mul:
        case OpCode::div_:
        case OpCode::mod:
        case OpCode::and_:
        case OpCode::or_:
        case OpCode::xor_:
        case OpCode::shl:
        case OpCode::shr:
        case OpCode::equal:
        case OpCode::less:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Instruction::RequiresLocalRegister() const
{
    switch (opCode)
    {
        case OpCode::arg:
        case OpCode::procedure_call:
        case OpCode::not_:
        case OpCode::neg:
        case OpCode::signextend:
        case OpCode::zeroextend:
        case OpCode::fpextend:
        case OpCode::truncate:
        case OpCode::bitcast:
        case OpCode::inttofloat:
        case OpCode::floattoint:
        case OpCode::inttoptr:
        case OpCode::ptrtoint:
        case OpCode::add:
        case OpCode::sub:
        case OpCode::mul:
        case OpCode::div_:
        case OpCode::mod:
        case OpCode::and_:
        case OpCode::or_:
        case OpCode::xor_:
        case OpCode::shl:
        case OpCode::shr:
        case OpCode::equal:
        case OpCode::less:
        case OpCode::param:
        case OpCode::load:
        case OpCode::elemaddr:
        case OpCode::ptroffset:
        case OpCode::ptrdiff:
        case OpCode::function_call:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

std::vector<BasicBlock*> Instruction::Successors() const
{
    std::vector<BasicBlock*> successors;
    switch (opCode)
    {
    case OpCode::jmp:
    {
        const JmpInstruction* jmp = static_cast<const JmpInstruction*>(this);
        AddPtrToSet(jmp->TargetBasicBlock(), successors);
        break;
    }
    case OpCode::branch:
    {
        const BranchInstruction* branch = static_cast<const BranchInstruction*>(this);
        AddPtrToSet(branch->TrueTargetBasicBlock(), successors);
        AddPtrToSet(branch->FalseTargetBasicBlock(), successors);
        break;
    }
    case OpCode::switch_:
    {
        const SwitchInstruction* switch_ = static_cast<const SwitchInstruction*>(this);
        AddPtrToSet(switch_->DefaultTargetBlock(), successors);
        for (const CaseTarget& caseTarget : switch_->CaseTargets())
        {
            AddPtrToSet(caseTarget.targetBlock, successors);
        }
        break;
    }
    }
    return successors;
}

BasicBlock* Instruction::Parent() const
{
    return static_cast<BasicBlock*>(GetContainer()->Parent());
}

void Instruction::AddUser(Instruction* user)
{
    AddPtrToSet(user, users);
}

void Instruction::RemoveUser(Instruction* user)
{
    RemovePtrFromSet(user, users);
}

void Instruction::AddToUses()
{
}

void Instruction::ReplaceUsesWith(Value* value)
{
    Function* fn = Parent()->Parent();
    RegValue* use = fn->GetRegValue(RegValueIndex());
    if (use)
    {
        std::vector<Instruction*> copiedUsers = users;
        for (const auto& user : copiedUsers)
        {
            user->ReplaceValue(use, value);
        }
    }
    else
    {
        Code* code = fn->Parent();
        Context* context = code->GetContext();
        Error("reg value " + std::to_string(RegValueIndex()) + "not found", Span(), context);
    }
}

void Instruction::RemoveFromUses()
{
    std::vector<Instruction*> uses = Uses();
    for (Instruction* use : uses)
    {
        use->RemoveUser(this);
    }
}

void Instruction::ReplaceValue(Value* use, Value* value)
{
}

StoreInstruction::StoreInstruction(const soul::ast::Span& span_, Value* value_, Value* ptr_) : Instruction(span_, nullptr, OpCode::store), value(value_), ptr(ptr_)
{
}

void StoreInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, value);
    cmajor::masm::intermediate::AddUser(this, ptr);
}

void StoreInstruction::ReplaceValue(Value* use, Value* value)
{
    if (this->value == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, this->value);
        this->value = value;
        cmajor::masm::intermediate::AddUser(this, this->value);
    }
    if (ptr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::masm::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> StoreInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, value);
    AddToUsesVec(uses, ptr);
    return uses;
}

void StoreInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* StoreInstruction::Clone(CloneContext& cloneContext) const
{
    StoreInstruction* clone = new StoreInstruction(Span(), value->Clone(cloneContext), ptr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void StoreInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("store ", 8));
    formatter.Write(value->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(value->ToString());
    formatter.Write(", ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
}

ArgInstruction::ArgInstruction(const soul::ast::Span& span_, Value* arg_) : Instruction(span_, nullptr, OpCode::arg), arg(arg_)
{
}

void ArgInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ArgInstruction::Clone(CloneContext& cloneContext) const
{
    ArgInstruction* clone = new ArgInstruction(Span(), arg->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void ArgInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, arg);
}

void ArgInstruction::ReplaceValue(Value* use, Value* value)
{
    if (arg == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, arg);
        arg = value;
        cmajor::masm::intermediate::AddUser(this, arg);
    }
}

std::vector<Instruction*> ArgInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, arg);
    return uses;
}

void ArgInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("arg ", 8));
    formatter.Write(arg->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(arg->ToString());
}

JmpInstruction::JmpInstruction(const soul::ast::Span& span_, int32_t targetLabelId_) :
    Instruction(span_, nullptr, OpCode::jmp), targetLabelId(targetLabelId_), targetBasicBlock(nullptr)
{
}

void JmpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* JmpInstruction::Clone(CloneContext& cloneContext) const
{
    JmpInstruction* clone = new JmpInstruction(Span(), targetBasicBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* bb = cloneContext.GetMappedBasicBlock(targetBasicBlock);
    if (bb)
    {
        clone->SetTargetBasicBlock(bb);
    }
    else
    {
        Error("mapped basic block not found", Span(), cloneContext.GetContext());
    }
    clone->AddToUses();
    return clone;
}

void JmpInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("jmp ", 8));
    formatter.Write("@" + std::to_string(targetBasicBlock->Id()));
}

BranchInstruction::BranchInstruction(const soul::ast::Span& span_, Value* cond_, int32_t trueTargetLabelId_, int32_t falseTargetLabelId_) :
    Instruction(span_, nullptr, OpCode::branch),
    cond(cond_), trueTargetLabelId(trueTargetLabelId_), trueTargetBasicBlock(nullptr), falseTargetLabelId(falseTargetLabelId_), falseTargetBasicBlock(nullptr)
{
}

void BranchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* BranchInstruction::Clone(CloneContext& cloneContext) const
{
    BranchInstruction* clone = new BranchInstruction(Span(), cond->Clone(cloneContext), trueTargetBasicBlock->Id(), falseTargetBasicBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* trueBB = cloneContext.GetMappedBasicBlock(trueTargetBasicBlock);
    if (trueBB)
    {
        clone->SetTrueTargetBasicBlock(trueBB);
    }
    else
    {
        Error("mapped basic block not found", trueTargetBasicBlock->Span(), cloneContext.GetContext());
    }
    BasicBlock* falseBB = cloneContext.GetMappedBasicBlock(falseTargetBasicBlock);
    if (falseBB)
    {
        clone->SetFalseTargetBasicBlock(falseBB);
    }
    else
    {
        Error("mapped basic block not found", falseTargetBasicBlock->Span(), cloneContext.GetContext());
    }
    clone->AddToUses();
    return clone;
}

void BranchInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, cond);
}

void BranchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, cond);
        cond = value;
        cmajor::masm::intermediate::AddUser(this, cond);
    }
}

std::vector<Instruction*> BranchInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, cond);
    return uses;
}

void BranchInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("branch ", 8));
    formatter.Write(cond->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(cond->ToString());
    formatter.Write(", ");
    formatter.Write("@" + std::to_string(trueTargetBasicBlock->Id()));
    formatter.Write(", ");
    formatter.Write("@" + std::to_string(falseTargetBasicBlock->Id()));
}

ProcedureCallInstruction::ProcedureCallInstruction(const soul::ast::Span& span_, Value* callee_) : Instruction(span_, nullptr, OpCode::procedure_call), callee(callee_)
{
}

void ProcedureCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ProcedureCallInstruction::Clone(CloneContext& cloneContext) const
{
    ProcedureCallInstruction* clone = new ProcedureCallInstruction(Span(), callee->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    std::vector<Value*> clonedArgs;
    for (const auto& arg : args)
    {
        clonedArgs.push_back(arg->Clone(cloneContext));
    }
    clone->SetArgs(std::move(clonedArgs));
    clone->AddToUses();
    return clone;
}

void ProcedureCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

Function* ProcedureCallInstruction::CalleeFn() const
{
    if (callee->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(callee);
        return symbolValue->GetFunction();
    }
    return nullptr;
}

void ProcedureCallInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::masm::intermediate::AddUser(this, arg);
    }
}

void ProcedureCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, callee);
        callee = value;
        cmajor::masm::intermediate::AddUser(this, callee);
    }
    for (auto& arg : args)
    {
        if (arg == use)
        {
            cmajor::masm::intermediate::RemoveUser(this, arg);
            arg = value;
            cmajor::masm::intermediate::AddUser(this, arg);
        }
    }
}

std::vector<Instruction*> ProcedureCallInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, callee);
    for (auto& arg : args)
    {
        AddToUsesVec(uses, arg);
    }
    return uses;
}

void ProcedureCallInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("call ", 8));
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
}

RetInstruction::RetInstruction(const soul::ast::Span& span_, Value* returnValue_) : Instruction(span_, nullptr, OpCode::ret), returnValue(returnValue_)
{
}

void RetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* RetInstruction::Clone(CloneContext& cloneContext) const
{
    Value* clonedReturnValue = nullptr;
    if (returnValue)
    {
        clonedReturnValue = returnValue->Clone(cloneContext);
    }
    RetInstruction* clone = new RetInstruction(Span(), clonedReturnValue);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void RetInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("ret ", 8));
    if (returnValue)
    {
        formatter.Write(returnValue->GetType()->Name());
        formatter.Write(" ");
        formatter.Write(returnValue->ToString());
    }
    else
    {
        formatter.Write("void");
    }
}

bool RetInstruction::IsFloatingPointInstruction() const 
{ 
    if (returnValue)
    {
        return returnValue->GetType()->IsFloatingPointType();
    }
    return false;
}

void RetInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, returnValue);
}

void RetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (returnValue == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, returnValue);
        returnValue = value;
        cmajor::masm::intermediate::AddUser(this, returnValue);
    }
}

std::vector<Instruction*> RetInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    if (returnValue)
    {
        AddToUsesVec(uses, returnValue);
    }
    return uses;
}

CaseTarget::CaseTarget() : caseValue(nullptr), targetLabelId(-1), targetBlock(nullptr)
{
}

SwitchInstruction::SwitchInstruction(const soul::ast::Span& span_, Value* cond_, int32_t defaultTargetLabelId_) :
    Instruction(span_, nullptr, OpCode::switch_), cond(cond_), defaultTargetLabelId(defaultTargetLabelId_), defaultTargetBlock(nullptr)
{
}

void SwitchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* SwitchInstruction::Clone(CloneContext& cloneContext) const
{
    SwitchInstruction* clone = new SwitchInstruction(Span(), cond->Clone(cloneContext), defaultTargetBlock->Id());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    BasicBlock* bb = cloneContext.GetMappedBasicBlock(defaultTargetBlock);
    if (bb)
    {
        clone->SetDefaultTargetBlock(bb);
    }
    else
    {
        Error("mapped basic block not found", defaultTargetBlock->Span(), cloneContext.GetContext());
    }
    for (const auto& caseTarget : caseTargets)
    {
        CaseTarget clonedCaseTarget;
        clonedCaseTarget.caseValue = caseTarget.caseValue->Clone(cloneContext);
        BasicBlock* bb = cloneContext.GetMappedBasicBlock(caseTarget.targetBlock);
        if (bb)
        {
            clonedCaseTarget.targetBlock = bb;
        }
        else
        {
            Error("mapped basic block not found", caseTarget.targetBlock->Span(), cloneContext.GetContext());
        }
        clone->AddCaseTarget(clonedCaseTarget);
    }
    clone->AddToUses();
    return clone;
}

void SwitchInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("switch ", 8));
    formatter.Write(cond->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(cond->ToString());
    formatter.Write(" ");
    formatter.Write("@" + std::to_string(defaultTargetBlock->Id()));
    formatter.Write(", [");
    bool first = true;
    for (const auto& p : caseTargets)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(" : ");
        }
        Value* value = p.caseValue;
        BasicBlock* dest = p.targetBlock;
        formatter.Write(value->GetType()->Name());
        formatter.Write(" ");
        formatter.Write(value->ToString());
        formatter.Write(", ");
        formatter.Write("@" + std::to_string(dest->Id()));
    }
    formatter.Write("]");
}

void SwitchInstruction::AddCaseTarget(const CaseTarget& caseTarget)
{
    caseTargets.push_back(caseTarget);
}

void SwitchInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, cond);
    for (auto& caseTarget : caseTargets)
    {
        cmajor::masm::intermediate::AddUser(this, caseTarget.caseValue);
    }
}

void SwitchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, cond);
        cond = value;
        cmajor::masm::intermediate::AddUser(this, cond);
    }
    for (auto& caseTarget : caseTargets)
    {
        if (caseTarget.caseValue == use)
        {
            cmajor::masm::intermediate::RemoveUser(this, caseTarget.caseValue);
            caseTarget.caseValue = value;
            cmajor::masm::intermediate::AddUser(this, caseTarget.caseValue);
        }
    }
}

std::vector<Instruction*> SwitchInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, cond);
    for (const CaseTarget& caseTarget : caseTargets)
    {
        AddToUsesVec(uses, caseTarget.caseValue);
    }
    return uses;
}

ValueInstruction::ValueInstruction(const soul::ast::Span& span_, RegValue* result_, OpCode opCode_) : Instruction(span_, result_->GetType(), opCode_), result(result_)
{
    result->SetInst(this);
}

ValueInstruction::~ValueInstruction()
{
    if (result)
    {
        result->SetInst(nullptr);
    }
}

void ValueInstruction::WriteResult(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format(result->GetType()->Name(), 7, util::FormatWidth::min));
    formatter.Write(" " + result->ToString());
}

UnaryInstruction::UnaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_, OpCode opCode_) : 
    ValueInstruction(span_, result_, opCode_), operand(operand_)
{
}

void UnaryInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, operand);
}

void UnaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (operand == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, operand);
        operand = value;
        cmajor::masm::intermediate::AddUser(this, operand);
    }
}

std::vector<Instruction*> UnaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, operand);
    return uses;
}

void UnaryInstruction::WriteArg(util::CodeFormatter& formatter)
{
    formatter.Write(operand->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(operand->ToString());
}

NotInstruction::NotInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::not_)
{
}

void NotInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* NotInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    NotInstruction* clone = new NotInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void NotInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = not ");
    WriteArg(formatter);
}

NegInstruction::NegInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::neg)
{
}

void NegInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* NegInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    NegInstruction* clone = new NegInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void NegInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = neg ");
    WriteArg(formatter);
}

SignExtendInstruction::SignExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::signextend)
{
}

void SignExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* SignExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    SignExtendInstruction* clone = new SignExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void SignExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = signextend ");
    WriteArg(formatter);
}

ZeroExtendInstruction::ZeroExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::zeroextend)
{
}

void ZeroExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ZeroExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ZeroExtendInstruction* clone = new ZeroExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ZeroExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = zeroextend ");
    WriteArg(formatter);
}

FloatingPointExtendInstruction::FloatingPointExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) :
    UnaryInstruction(span_, result_, operand_, OpCode::fpextend)
{
}

void FloatingPointExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* FloatingPointExtendInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FloatingPointExtendInstruction* clone = new FloatingPointExtendInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void FloatingPointExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = fpextend ");
    WriteArg(formatter);
}

TruncateInstruction::TruncateInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::truncate)
{
}

void TruncateInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* TruncateInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    TruncateInstruction* clone = new TruncateInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void TruncateInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = truncate ");
    WriteArg(formatter);
}

BitcastInstruction::BitcastInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::bitcast)
{
}

void BitcastInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* BitcastInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    BitcastInstruction* clone = new BitcastInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void BitcastInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = bitcast ");
    WriteArg(formatter);
}

IntToFloatInstruction::IntToFloatInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::inttofloat)
{
}

void IntToFloatInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* IntToFloatInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    IntToFloatInstruction* clone = new IntToFloatInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void IntToFloatInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttofloat ");
    WriteArg(formatter);
}

FloatToIntInstruction::FloatToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::floattoint)
{
}

void FloatToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* FloatToIntInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FloatToIntInstruction* clone = new FloatToIntInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void FloatToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = floattoint ");
    WriteArg(formatter);
}

IntToPtrInstruction::IntToPtrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::inttoptr)
{
}

void IntToPtrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* IntToPtrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    IntToPtrInstruction* clone = new IntToPtrInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void IntToPtrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttoptr ");
    WriteArg(formatter);
}

PtrToIntInstruction::PtrToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::ptrtoint)
{
}

void PtrToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* PtrToIntInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrToIntInstruction* clone = new PtrToIntInstruction(Span(), result, Operand()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptrtoint ");
    WriteArg(formatter);
}

BinaryInstruction::BinaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_) :
    ValueInstruction(span_, result_, opCode_), left(left_), right(right_)
{
}

void BinaryInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, left);
    cmajor::masm::intermediate::AddUser(this, right);
}

void BinaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (left == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, left);
        left = value;
        cmajor::masm::intermediate::AddUser(this, left);
    }
    if (right == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, right);
        right = value;
        cmajor::masm::intermediate::AddUser(this, right);
    }
}

std::vector<Instruction*> BinaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, left);
    AddToUsesVec(uses, right);
    return uses;
}

void BinaryInstruction::WriteArgs(util::CodeFormatter& formatter)
{
    formatter.Write(left->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(left->ToString());
    formatter.Write(", ");
    formatter.Write(right->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(right->ToString());
}

AddInstruction::AddInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::add)
{
}

void AddInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* AddInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    AddInstruction* clone = new AddInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void AddInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = add ");
    WriteArgs(formatter);
}

SubInstruction::SubInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::sub)
{
}

void SubInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* SubInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    SubInstruction* clone = new SubInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void SubInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = sub ");
    WriteArgs(formatter);
}

MulInstruction::MulInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::mul)
{
}

void MulInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* MulInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    MulInstruction* clone = new MulInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void MulInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mul ");
    WriteArgs(formatter);
}

DivInstruction::DivInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::div_)
{
}

void DivInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* DivInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    DivInstruction* clone = new DivInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void DivInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = div ");
    WriteArgs(formatter);
}

ModInstruction::ModInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::mod)
{
}

void ModInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ModInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ModInstruction* clone = new ModInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ModInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mod ");
    WriteArgs(formatter);
}

AndInstruction::AndInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::and_)
{
}

void AndInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* AndInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    AndInstruction* clone = new AndInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void AndInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = and ");
    WriteArgs(formatter);
}

OrInstruction::OrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::or_)
{
}

void OrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* OrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    OrInstruction* clone = new OrInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void OrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = or ");
    WriteArgs(formatter);
}

XorInstruction::XorInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::xor_)
{
}

void XorInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* XorInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    XorInstruction* clone = new XorInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void XorInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = xor ");
    WriteArgs(formatter);
}

ShlInstruction::ShlInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::shl)
{
}

void ShlInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ShlInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ShlInstruction* clone = new ShlInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ShlInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shl ");
    WriteArgs(formatter);
}

ShrInstruction::ShrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::shr)
{
}

void ShrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ShrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ShrInstruction* clone = new ShrInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ShrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shr ");
    WriteArgs(formatter);
}

EqualInstruction::EqualInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::equal)
{
}

void EqualInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* EqualInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    EqualInstruction* clone = new EqualInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void EqualInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = equal ");
    WriteArgs(formatter);
}

LessInstruction::LessInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::less)
{
}

void LessInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* LessInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LessInstruction* clone = new LessInstruction(Span(), result, Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LessInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = less ");
    WriteArgs(formatter);
}

ParamInstruction::ParamInstruction(const soul::ast::Span& span_, RegValue* result_) : ValueInstruction(span_, result_, OpCode::param)
{
}

void ParamInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ParamInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ParamInstruction* clone = new ParamInstruction(Span(), result);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ParamInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = param");
}

LocalInstruction::LocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_) : ValueInstruction(span_, result_, OpCode::local), localType(localType_)
{
}

void LocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* LocalInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LocalInstruction* clone = new LocalInstruction(Span(), result, localType);
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LocalInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = local ");
    formatter.Write(localType->Name());
}

LoadInstruction::LoadInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_) : ValueInstruction(span_, result_, OpCode::load), ptr(ptr_)
{
}

void LoadInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* LoadInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    LoadInstruction* clone = new LoadInstruction(Span(), result, ptr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void LoadInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = load ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
}

void LoadInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, ptr);
}

void LoadInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::masm::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> LoadInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    return uses;
}

ElemAddrInstruction::ElemAddrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* indexValue_) :
    ValueInstruction(span_, result_, OpCode::elemaddr), ptr(ptr_), indexValue(indexValue_)
{
}

void ElemAddrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* ElemAddrInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    ElemAddrInstruction* clone = new ElemAddrInstruction(Span(), result, Ptr()->Clone(cloneContext), IndexValue()->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void ElemAddrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = elemaddr ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
    formatter.Write(", ");
    formatter.Write(indexValue->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(indexValue->ToString());
}

ElemAddrKind ElemAddrInstruction::GetElemAddrKind(Context* context) const
{
    if (ptr->GetType()->IsPointerType())
    {
        PointerType* ptrType = static_cast<PointerType*>(ptr->GetType());
        Type* pointeeType = ptrType->RemovePointer(Span(), context);
        if (pointeeType->IsArrayType())
        {
            return ElemAddrKind::array;
        }
        else if (pointeeType->IsStructureType())
        {
            return ElemAddrKind::structure;
        }
    }
    Error("invalid elem addr kind", Span(), context);
    return ElemAddrKind::none;
}

void ElemAddrInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, ptr);
    cmajor::masm::intermediate::AddUser(this, indexValue);
}

void ElemAddrInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::masm::intermediate::AddUser(this, ptr);
    }
    if (indexValue == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, indexValue);
        indexValue = value;
        cmajor::masm::intermediate::AddUser(this, indexValue);
    }
}

std::vector<Instruction*> ElemAddrInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, indexValue);
    return uses;
}

PtrOffsetInstruction::PtrOffsetInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* offset_) :
    ValueInstruction(span_, result_, OpCode::ptroffset), ptr(ptr_), offset(offset_)
{
}

void PtrOffsetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* PtrOffsetInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrOffsetInstruction* clone = new PtrOffsetInstruction(Span(), result, Ptr()->Clone(cloneContext), offset->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrOffsetInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptroffset ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
    formatter.Write(", ");
    formatter.Write(offset->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(offset->ToString());
}

void PtrOffsetInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, ptr);
    cmajor::masm::intermediate::AddUser(this, offset);
}

void PtrOffsetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::masm::intermediate::AddUser(this, ptr);
    }
    if (offset == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, offset);
        offset = value;
        cmajor::masm::intermediate::AddUser(this, offset);
    }
}

std::vector<Instruction*> PtrOffsetInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, offset);
    return uses;
}

PtrDiffInstruction::PtrDiffInstruction(const soul::ast::Span& span_, RegValue* result_, Value* leftPtr_, Value* rightPtr_) :
    ValueInstruction(span_, result_, OpCode::ptrdiff), leftPtr(leftPtr_), rightPtr(rightPtr_)
{
}

void PtrDiffInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* PtrDiffInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    PtrDiffInstruction* clone = new PtrDiffInstruction(Span(), result, leftPtr->Clone(cloneContext), rightPtr->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    clone->AddToUses();
    return clone;
}

void PtrDiffInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptrdiff ");
    formatter.Write(leftPtr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(leftPtr->ToString());
    formatter.Write(", ");
    formatter.Write(rightPtr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(rightPtr->ToString());
}

void PtrDiffInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, leftPtr);
    cmajor::masm::intermediate::AddUser(this, rightPtr);
}

void PtrDiffInstruction::ReplaceValue(Value* use, Value* value)
{
    if (leftPtr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, leftPtr);
        leftPtr = value;
        cmajor::masm::intermediate::AddUser(this, leftPtr);
    }
    if (rightPtr == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, rightPtr);
        rightPtr = value;
        cmajor::masm::intermediate::AddUser(this, rightPtr);
    }
}

std::vector<Instruction*> PtrDiffInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, leftPtr);
    AddToUsesVec(uses, rightPtr);
    return uses;
}

FunctionCallInstruction::FunctionCallInstruction(const soul::ast::Span& span_, RegValue* result_, Value* callee_) :
    ValueInstruction(span_, result_, OpCode::function_call), callee(callee_)
{
}

void FunctionCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* FunctionCallInstruction::Clone(CloneContext& cloneContext) const
{
    RegValue* result = cloneContext.CurrentFunction()->MakeRegValue(Span(), GetType(), Result()->Reg(), cloneContext.GetContext());
    FunctionCallInstruction* clone = new FunctionCallInstruction(Span(), result, callee->Clone(cloneContext));
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    result->SetInst(clone);
    std::vector<Value*> clonedArgs;
    for (const auto& arg : args)
    {
        clonedArgs.push_back(arg->Clone(cloneContext));
    }
    clone->SetArgs(std::move(clonedArgs));
    clone->AddToUses();
    return clone;
}

void FunctionCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

Function* FunctionCallInstruction::CalleeFn() const
{
    if (callee->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(callee);
        return symbolValue->GetFunction();
    }
    return nullptr;
}

void FunctionCallInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::masm::intermediate::AddUser(this, arg);
    }
}

void FunctionCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        cmajor::masm::intermediate::RemoveUser(this, callee);
        callee = value;
        cmajor::masm::intermediate::AddUser(this, callee);
    }
    for (auto& arg : args)
    {
        if (arg == use)
        {
            cmajor::masm::intermediate::RemoveUser(this, arg);
            arg = value;
            cmajor::masm::intermediate::AddUser(this, arg);
        }
    }
}

void FunctionCallInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = call ");
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
}

std::vector<Instruction*> FunctionCallInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, callee);
    for (const auto& arg : args)
    {
        AddToUsesVec(uses, arg);
    }
    return uses;
}

BlockValue::BlockValue(Value* value_, BasicBlock* block_) : value(value_), blockId(block_->Id()), block(block_)
{
}

NoOperationInstruction::NoOperationInstruction(const soul::ast::Span& span_) : Instruction(span_, nullptr, OpCode::nop)
{
}

void NoOperationInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Instruction* NoOperationInstruction::Clone(CloneContext& cloneContext) const
{
    NoOperationInstruction* clone = new NoOperationInstruction(Span());
    clone->SetIndex(Index());
    clone->SetRegValueIndex(RegValueIndex());
    clone->AddToUses();
    return clone;
}

void NoOperationInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write("nop");
}

BasicBlock::BasicBlock(const soul::ast::Span& span_, int32_t id_) : span(span_), id(id_), instructions(this)
{
}

void BasicBlock::Check()
{
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        inst->Check();
        inst = inst->Next();
    }
}

void BasicBlock::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BasicBlock* BasicBlock::Clone(CloneContext& cloneContext) const
{
    BasicBlock* clone = new BasicBlock(Span(), id);
    cloneContext.MapBasicBlock(const_cast<BasicBlock*>(this), clone);
    return clone;
}

void BasicBlock::CloneInstructions(CloneContext& cloneContext, BasicBlock* to)
{
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        Instruction* clonedInst = inst->Clone(cloneContext);
        cloneContext.MapInstruction(inst, clonedInst);
        to->AddInstruction(clonedInst);
        inst = inst->Next();
    }
}

void BasicBlock::VisitInstructions(Visitor& visitor)
{
    Instruction* inst = FirstInstruction();
    while (inst != nullptr)
    {
        inst->Accept(visitor);
        inst = inst->Next();
    }
}

std::string BasicBlock::Name() const
{
    if (id == entryBlockId)
    {
        return "entry";
    }
    else if (id == exitBlockId)
    {
        return "exit";
    }
    else
    {
        return std::to_string(id);
    }
}

bool BasicBlock::IsLast() const
{
    Function* fn = Parent();
    return this == fn->LastBasicBlock();
}

void BasicBlock::AddInstruction(Instruction* instruction, bool mapInstruction)
{
    instructions.AddChild(instruction);
    if (mapInstruction && instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        Function* function = Parent();
        Context* context = function->Parent()->GetContext();
        function->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
    }
}

void BasicBlock::AddInstruction(Instruction* instruction)
{
    AddInstruction(instruction, true);
}

std::unique_ptr<Instruction> BasicBlock::RemoveInstruction(Instruction* instruction)
{
    return std::unique_ptr<Instruction>(static_cast<Instruction*>(instructions.RemoveChild(instruction).release()));
}

void BasicBlock::InsertInstructionAfter(Instruction* instruction, Instruction* after)
{
    instructions.InsertAfter(instruction, after);
}

void BasicBlock::InsertFront(Instruction* instruction)
{
    if (instructions.IsEmpty())
    {
        instructions.AddChild(instruction);
    }
    else
    {
        instructions.InsertBefore(instruction, instructions.FirstChild());
    }
    if (instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        Function* function = Parent();
        Context* context = function->Parent()->GetContext();
        function->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
    }
}

BasicBlock* BasicBlock::SplitAfter(Instruction* instruction)
{
    Function* fn = Parent();
    std::unique_ptr<BasicBlock> newBB(new BasicBlock(Span(), fn->GetNextBasicBlockNumber()));
    BasicBlock* nbb = newBB.get();
    fn->InsertBasicBlockAfter(newBB.release(), instruction->Parent());
    instruction = instruction->Next();
    while (instruction)
    {
        Instruction* next = instruction->Next();
        std::unique_ptr<Instruction> removedInst = instruction->Parent()->RemoveInstruction(instruction);
        if (removedInst->IsValueInstruction())
        {
            ValueInstruction* valueInst = static_cast<ValueInstruction*>(removedInst.get());
            valueInst->SetRegValueIndex(fn->GetNextRegNumber());
            valueInst->Result()->SetReg(valueInst->RegValueIndex());
            fn->MapRegValue(valueInst->Result());
        }
        nbb->AddInstruction(removedInst.release(), false);
        instruction = next;
    }
    return nbb;
}

void BasicBlock::AddSuccessor(BasicBlock* successor)
{
    AddPtrToSet(successor, successors);
}

bool BasicBlock::RemoveSuccessor(BasicBlock* successor)
{
    return RemovePtrFromSet(successor, successors);
}

void BasicBlock::AddPredecessor(BasicBlock* predecessor)
{
    AddPtrToSet(predecessor, predecessors);
}

bool BasicBlock::RemovePredecessor(BasicBlock* predecessor)
{
    return RemovePtrFromSet(predecessor, predecessors);
}

void BasicBlock::ClearSuccessorsAndPredecessors()
{
    successors.clear();
    predecessors.clear();
}

int BasicBlock::IndexOf(Instruction* x)
{
    int index = 0;
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        if (inst == x)
        {
            return index;
        }
        ++index;
        inst = inst->Next();
    }
    return -1;
}

Function* BasicBlock::Parent() const
{
    return static_cast<Function*>(GetContainer()->Parent());
}

void BasicBlock::Write(util::CodeFormatter& formatter)
{
    int indent = formatter.IndentSize();
    bool indentDecremented = false;
    if (formatter.CurrentIndent() > 0)
    {
        formatter.DecIndent();
        indentDecremented = true;
    }
    formatter.Write(util::Format("@" + std::to_string(id), indent, util::FormatWidth::min));
    bool first = true;
    Instruction* inst = FirstInstruction();
    while (inst)
    {
        inst->Write(formatter);
        formatter.WriteLine();
        if (first)
        {
            if (indentDecremented)
            {
                formatter.IncIndent();
            }
            first = false;
        }
        inst = inst->Next();
    }
}

Function::Function(const soul::ast::Span& span_, FunctionType* type_, const std::string& name_, bool definition_, MetadataRef* metadataRef_) :
    flags(FunctionFlags::none), span(span_), type(type_), name(name_), metadataRef(metadataRef_), basicBlocks(this), nextRegNumber(0), nextBBNumber(0), mdId(-1)
{
    if (definition_)
    {
        SetFlag(FunctionFlags::defined);
    }
}

void Function::Check()
{
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        bb->Check();
        bb = bb->Next();
    }
}

void Function::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Function* Function::Clone() const
{
    Code* code = Parent();
    Function* clone = new Function(Span(), type, name, IsDefined(), metadataRef);
    clone->SetMdId(mdId);
    std::string fullName = ResolveFullName();
    if (!fullName.empty())
    {
        clone->SetComment(fullName);
    }
    clone->SetContainer(code->Functions());
    CloneContext cloneContext;
    cloneContext.SetContext(code->GetContext());
    cloneContext.SetCurrentFunction(clone);
    BasicBlock* bb = const_cast<Function*>(this)->FirstBasicBlock();
    while (bb)
    {
        BasicBlock* clonedBB = bb->Clone(cloneContext);
        clone->AddBasicBlock(clonedBB);
        bb = bb->Next();
    }
    bb = const_cast<Function*>(this)->FirstBasicBlock();
    while (bb)
    {
        BasicBlock* clonedBB = cloneContext.GetMappedBasicBlock(bb);
        if (clonedBB)
        {
            bb->CloneInstructions(cloneContext, clonedBB);
        }
        else
        {
            Error("mapped basic block not found", bb->Span(), code->GetContext());
        }
        bb = bb->Next();
    }
    for (const auto& instRegValuePair : cloneContext.UnmappedInstructions())
    {
        Instruction* inst = instRegValuePair.first;
        RegValue* regValue = instRegValuePair.second;
        Instruction* mappedInst = cloneContext.GetMappedInstruction(inst);
        if (mappedInst)
        {
            regValue->SetInst(mappedInst);
        }
        else
        {
            Error("mapped instruction not found", inst->Span(), code->GetContext());
        }
    }
    return clone;
}

void Function::VisitBasicBlocks(Visitor& visitor)
{
    BasicBlock* basicBlock = FirstBasicBlock();
    while (basicBlock)
    {
        basicBlock->Accept(visitor);
        basicBlock = basicBlock->Next();
    }
}

Code* Function::Parent() const
{
    return static_cast<Code*>(GetContainer()->Parent());
}

BasicBlock* Function::GetBasicBlock(int32_t id) const
{
    auto it = basicBlockMap.find(id);
    if (it != basicBlockMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

BasicBlock* Function::AddBasicBlock(const soul::ast::Span& span, int32_t id, Context* context)
{
    BasicBlock* prev = GetBasicBlock(id);
    if (prev)
    {
        Error("error adding basic block: basic block id not unique", span, context, prev->Span());
    }
    BasicBlock* basicBlock = new BasicBlock(span, id);
    basicBlocks.AddChild(basicBlock);
    basicBlockMap[id] = basicBlock;
    return basicBlock;
}

void Function::AddBasicBlock(BasicBlock* basicBlock)
{
    basicBlocks.AddChild(basicBlock);
    basicBlockMap[basicBlock->Id()] = basicBlock;
}

void Function::InsertBasicBlockBefore(BasicBlock* basicBlockToInsert, BasicBlock* before)
{
    basicBlocks.InsertBefore(basicBlockToInsert, before);
    basicBlockMap[basicBlockToInsert->Id()] = basicBlockToInsert;
}

void Function::InsertBasicBlockAfter(BasicBlock* basicBlockToInsert, BasicBlock* after)
{
    basicBlocks.InsertAfter(basicBlockToInsert, after);
    basicBlockMap[basicBlockToInsert->Id()] = basicBlockToInsert;
}

std::unique_ptr<BasicBlock> Function::RemoveBasicBlock(BasicBlock* block)
{
    RemovePtrFromSet(block, retBlocks);
    basicBlockMap.erase(block->Id());
    for (BasicBlock* successor : block->Successors())
    {
        successor->RemovePredecessor(block);
    }
    for (BasicBlock* predecessor : block->Predecessors())
    {
        predecessor->RemoveSuccessor(block);
    }
    return std::unique_ptr<BasicBlock>(static_cast<BasicBlock*>(basicBlocks.RemoveChild(block).release()));
}

RegValue* Function::GetRegValue(int32_t reg) const
{
    auto it = regValueMap.find(reg);
    if (it != regValueMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

RegValue* Function::GetRegRef(const soul::ast::Span& span, Type* type, int32_t reg, Context* context) const
{
    RegValue* regValue = GetRegValue(reg);
    if (regValue)
    {
        if (regValue->GetType() == type)
        {
            return regValue;
        }
        else
        {
            Error("error referencing register value " + std::to_string(reg) + ": type conflick", span, context, regValue->Span());
        }
    }
    else
    {
        Error("error referencing register: register " + std::to_string(reg) + " not found", span, context);
    }
    return nullptr;
}

RegValue* Function::MakeRegValue(const soul::ast::Span& span, Type* type, int32_t reg, Context* context)
{
    RegValue* prev = GetRegValue(reg);
    if (prev)
    {
        Error("error adding register " + std::to_string(reg) + ": register not unique", span, context, prev->Span());
    }
    RegValue* regValue = new RegValue(span, type, reg);
    regValues.push_back(std::unique_ptr<RegValue>(regValue));
    regValueMap[reg] = regValue;
    return regValue;
}

RegValue* Function::MakeNextRegValue(const soul::ast::Span& span, Type* type, Context* context)
{
    return MakeRegValue(span, type, GetNextRegNumber(), context);
}

void Function::MapRegValue(RegValue* regValue)
{
    regValueMap[regValue->Reg()] = regValue;
}

void Function::RemoveRegValue(int32_t reg)
{
    regValueMap.erase(reg);
}

Instruction* Function::GetInstruction(int32_t reg) const
{
    auto it = instructionMap.find(reg);
    if (it != instructionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Function::MapInstruction(int32_t reg, Instruction* inst, Context* context)
{
    Instruction* prev = GetInstruction(reg);
    if (prev)
    {
        Error("error mappint instruction " + std::to_string(reg) + ": register number not unique", span, context, prev->Span());
    }
    instructionMap[reg] = inst;
}

void Function::UnmapInstruction(int32_t reg)
{
    instructionMap.erase(reg);
}

int Function::NumBasicBlocks() const
{
    return basicBlockMap.size();
}

void Function::AddRetBlock(BasicBlock* retBlock)
{
    AddPtrToSet(retBlock, retBlocks);
}

void Function::AddEntryAndExitBlocks()
{
    if (basicBlocks.IsEmpty())
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has no basic blocks", Span(), Parent()->GetContext());
    }
    BasicBlock* prevEntryBlock = GetBasicBlock(entryBlockId);
    if (prevEntryBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an entry block", Span(), Parent()->GetContext());
    }
    BasicBlock* prevExitBlock = GetBasicBlock(exitBlockId);
    if (prevExitBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an exit block", Span(), Parent()->GetContext());
    }
    if (RetBlocks().empty())
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has no ret blocks", Span(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = FirstBasicBlock();
    BasicBlock* entryBlock = new BasicBlock(Span(), entryBlockId);
    basicBlockMap[entryBlock->Id()] = entryBlock;
    entryBlock->AddSuccessor(firstBasicBlock);
    firstBasicBlock->AddPredecessor(entryBlock);
    basicBlocks.InsertBefore(entryBlock, firstBasicBlock);
    BasicBlock* exitBlock = new BasicBlock(Span(), exitBlockId);
    for (BasicBlock* retBlock : RetBlocks())
    {
        retBlock->AddSuccessor(exitBlock);
        exitBlock->AddPredecessor(retBlock);
    }
    basicBlocks.AddChild(exitBlock);
}

void Function::RemoveEntryAndExitBlocks()
{
    BasicBlock* entryBlock = GetBasicBlock(entryBlockId);
    if (!entryBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no entry block", Span(), Parent()->GetContext());
    }
    BasicBlock* exitBlock = GetBasicBlock(exitBlockId);
    if (!exitBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no exit block", Span(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = entryBlock->Next();
    if (!firstBasicBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no basic blocks other than entry and exit blocks", Span(), Parent()->GetContext());
    }
    if (!firstBasicBlock->RemovePredecessor(entryBlock))
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' old first block does not have the entry block as a predecessor", Span(), Parent()->GetContext());
    }
    basicBlocks.RemoveChild(entryBlock);
    basicBlockMap.erase(entryBlockId);
    for (BasicBlock* retBlock : RetBlocks())
    {
        if (!retBlock->RemoveSuccessor(exitBlock))
        {
            Error("error removing entry and exit blocks: function '" + Name() + "' ret block does not have the exit block as a successor", Span(), Parent()->GetContext());
        }
    }
    basicBlocks.RemoveChild(exitBlock);
    basicBlockMap.erase(exitBlockId);
}

void Function::SetNumbers()
{
    Context* context = Parent()->GetContext();
    basicBlockMap.clear();
    regValueMap.clear();
    instructionMap.clear();
    nextBBNumber = 0;
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        bb->SetId(nextBBNumber++);
        basicBlockMap[bb->Id()] = bb;
        bb = bb->Next();
    }
    int instructionIndex = 0;
    nextRegNumber = 0;
    bb = FirstBasicBlock();
    while (bb)
    {
        Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            inst->SetIndex(instructionIndex++);
            if (inst->IsValueInstruction())
            {
                ValueInstruction* valueInst = static_cast<ValueInstruction*>(inst);
                valueInst->SetRegValueIndex(nextRegNumber++);
                valueInst->Result()->SetReg(valueInst->RegValueIndex());
                MapRegValue(valueInst->Result());
                MapInstruction(valueInst->RegValueIndex(), valueInst, context);
            }
            inst = inst->Next();
        }
        bb = bb->Next();
    }
}

void Function::MoveRegValues(Function* toFunction)
{
    for (auto& regValue : regValues)
    {
        if (!regValue->Inst()) continue;
        regValue->SetReg(toFunction->GetNextRegNumber());
        toFunction->MapRegValue(regValue.get());
        toFunction->regValues.push_back(std::move(regValue));
    }
}

void Function::SetComment(const std::string& comment_)
{
    comment = comment_;
}

std::string Function::ResolveFullName() const
{
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
        if (metadataStruct)
        {
            MetadataItem* metadataItem = metadataStruct->GetItem("fullName");
            if (metadataItem)
            {
                if (metadataItem->IsMetadataString())
                {
                    MetadataString* metadataString = static_cast<MetadataString*>(metadataItem);
                    return metadataString->Value();
                }
            }
        }
    }
    return std::string();
}

void Function::Write(util::CodeFormatter& formatter)
{
    if (metadataRef)
    {
        SetComment(ResolveFullName());
    }
    if (!comment.empty())
    {
        formatter.WriteLine("// " + comment);
        formatter.WriteLine();
    }
    if (basicBlocks.IsEmpty())
    {
        formatter.Write("extern ");
    }
    else if (IsInline())
    {
        formatter.Write("inline ");
    }
    Context* context = Parent()->GetContext();
    Type* ptrType = type->AddPointer(context);
    if (metadataRef)
    {
        mdId = metadataRef->NodeId();
    }
    std::string mdIdStr;
    if (mdId != -1)
    {
        mdIdStr.append(" !").append(std::to_string(mdId));
    }
    formatter.WriteLine("function " + ptrType->Name() + " " + name + mdIdStr);
    if (basicBlocks.IsEmpty())
    {
        return;
    }
    SetNumbers();
    formatter.WriteLine("{");
    formatter.IncIndent();
    bool first = true;
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        if (bb->IsEmpty())
        {
            continue;
        }
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        bb->Write(formatter);
        bb = bb->Next();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

Code::Code() : context(nullptr), currentFunction(nullptr), functions(this), totalFunctions(0)
{
}

void Code::SetCurrentFunction(Function* function)
{
    currentFunction = function;
}

Function* Code::GetFunction(const std::string& functionId) const
{
    auto it = functionMap.find(functionId);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Function* Code::AddFunctionDefinition(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId, bool inline_, MetadataRef* metadataRef, 
    Context* context)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->IsDefined())
        {
            Error("error adding function '" + functionId + "': function id not unique", span, context, prev->Span());
        }
        else
        {
            if (prev->GetType() != functionType)
            {
                Error("error adding function '" + functionId + "': type '" + functionType->Name() + "' conflicts with earlier declaration", span, context, prev->Span());
            }
            prev->SetDefined();
            if (inline_)
            {
                prev->SetInline();
            }
            return prev;
        }
    }
    Function* function = new Function(span, functionType, functionId, true, metadataRef);
    if (inline_)
    {
        function->SetInline();
    }
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

Function* Code::AddFunctionDeclaration(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->GetType() != functionType)
        {
            Error("error adding function declaration '" + functionId + "': type '" + functionType->Name() + "' conflicts with earlier declaration", span, context, prev->Span());
        }
        return prev;
    }
    Function* function = new Function(span, functionType, functionId, false, nullptr);
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

void Code::VisitFunctions(Visitor& visitor)
{
    totalFunctions = 0;
    Function* function = FirstFunction();
    while (function)
    {
        function->Accept(visitor);
        ++totalFunctions;
        function = function->Next();
    }
}

} // cmajor::masm::intermediate
