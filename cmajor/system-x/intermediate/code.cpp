// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.code;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.visitor;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.util;
import soul.xml.dom;
import util;

namespace cmajor::systemx::intermediate {

const char* opCodeStr[] =
{
    "store", "arg", "jmp", "branch", "call", "ret", "switch",
    "not", "neg", "signextend", "zeroextend", "truncate", "bitcast", "inttofloat", "floattoint", "inttoptr", "ptrtoint",
    "add", "sub", "mul", "div", "mod", "and", "or", "xor", "shl", "shr", "equal", "less",
    "param", "local", "load", "elemaddr", "ptroffset", "ptrdiff", "call", "trap", "phi",
    "nop"
};

RegValue::RegValue(const soul::ast::SourcePos& sourcePos_, Type* type_, int32_t reg_) : Value(sourcePos_, ValueKind::regValue, type_), reg(reg_), inst(nullptr)
{
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

Instruction::Instruction(const soul::ast::SourcePos& sourcePos_, Type* type_, OpCode opCode_) : 
    Value(sourcePos_, ValueKind::instruction, type_), opCode(opCode_), metadataRef(nullptr), index(-1), regValueIndex(-1)
{
}

std::string Instruction::Name() const
{
    return opCodeStr[static_cast<int>(opCode)];
}

bool Instruction::IsLeader() const
{
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
        case OpCode::trap:
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
        Error("reg value " + std::to_string(RegValueIndex()) + "not found", GetSourcePos(), context);
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

BasicBlock* Instruction::Parent() const
{
    return static_cast<BasicBlock*>(GetContainer()->Parent());
}

StoreInstruction::StoreInstruction(const soul::ast::SourcePos& sourcePos_, Value* value_, Value* ptr_) : Instruction(sourcePos_, nullptr, OpCode::store), value(value_), ptr(ptr_)
{
}

void StoreInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StoreInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, value);
    cmajor::systemx::intermediate::AddUser(this, ptr);
}

void StoreInstruction::ReplaceValue(Value* use, Value* value)
{
    if (this->value == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, this->value);
        this->value = value;
        cmajor::systemx::intermediate::AddUser(this, this->value);
    }
    if (ptr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::systemx::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> StoreInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, value);
    AddToUsesVec(uses, ptr);
    return uses;
}

ArgInstruction::ArgInstruction(const soul::ast::SourcePos& sourcePos_, Value* arg_) : Instruction(sourcePos_, nullptr, OpCode::arg), arg(arg_), argIndex(0)
{
}

void ArgInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArgInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, arg);
}

void ArgInstruction::ReplaceValue(Value* use, Value* value)
{
    if (arg == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, arg);
        arg = value;
        cmajor::systemx::intermediate::AddUser(this, arg);
    }
}

std::vector<Instruction*> ArgInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, arg);
    return uses;
}

JmpInstruction::JmpInstruction(const soul::ast::SourcePos& sourcePos_, int32_t targetLabelId_) :
    Instruction(sourcePos_, nullptr, OpCode::jmp), targetLabelId(targetLabelId_), targetBasicBlock(nullptr)
{
}

void JmpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BranchInstruction::BranchInstruction(const soul::ast::SourcePos& sourcePos_, Value* cond_, int32_t trueTargetLabelId_, int32_t falseTargetLabelId_) :
    Instruction(sourcePos_, nullptr, OpCode::branch),
    cond(cond_), trueTargetLabelId(trueTargetLabelId_), trueTargetBasicBlock(nullptr), falseTargetLabelId(falseTargetLabelId_), falseTargetBasicBlock(nullptr)
{
}

void BranchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BranchInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, cond);
}

void BranchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, cond);
        cond = value;
        cmajor::systemx::intermediate::AddUser(this, cond);
    }
}

std::vector<Instruction*> BranchInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, cond);
    return uses;
}

ProcedureCallInstruction::ProcedureCallInstruction(const soul::ast::SourcePos& sourcePos_, Value* callee_) : Instruction(sourcePos_, nullptr, OpCode::procedure_call), callee(callee_)
{
}

void ProcedureCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ProcedureCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

void ProcedureCallInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::systemx::intermediate::AddUser(this, arg);
    }
}

void ProcedureCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, callee);
        callee = value;
        cmajor::systemx::intermediate::AddUser(this, callee);
    }
    for (auto& arg : args)
    {
        if (arg == use)
        {
            cmajor::systemx::intermediate::RemoveUser(this, arg);
            arg = value;
            cmajor::systemx::intermediate::AddUser(this, arg);
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

RetInstruction::RetInstruction(const soul::ast::SourcePos& sourcePos_, Value* returnValue_) : Instruction(sourcePos_, nullptr, OpCode::ret), returnValue(returnValue_)
{
}

void RetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RetInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, returnValue);
}

void RetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (returnValue == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, returnValue);
        returnValue = value;
        cmajor::systemx::intermediate::AddUser(this, returnValue);
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

SwitchInstruction::SwitchInstruction(const soul::ast::SourcePos& sourcePos_, Value* cond_, int32_t defaultTargetLabelId_) :
    Instruction(sourcePos_, nullptr, OpCode::switch_), cond(cond_), defaultTargetLabelId(defaultTargetLabelId_), defaultTargetBlock(nullptr)
{
}

void SwitchInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SwitchInstruction::AddCaseTarget(const CaseTarget& caseTarget)
{
    caseTargets.push_back(caseTarget);
}

void SwitchInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, cond);
    for (auto& caseTarget : caseTargets)
    {
        cmajor::systemx::intermediate::AddUser(this, caseTarget.caseValue);
    }
}

void SwitchInstruction::ReplaceValue(Value* use, Value* value)
{
    if (cond == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, cond);
        cond = value;
        cmajor::systemx::intermediate::AddUser(this, cond);
    }
    for (auto& caseTarget : caseTargets)
    {
        if (caseTarget.caseValue == use)
        {
            cmajor::systemx::intermediate::RemoveUser(this, caseTarget.caseValue);
            caseTarget.caseValue = value;
            cmajor::systemx::intermediate::AddUser(this, caseTarget.caseValue);
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

ValueInstruction::ValueInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, OpCode opCode_) : 
    Instruction(sourcePos_, result_->GetType(), opCode_), result(result_)
{
}

ValueInstruction::~ValueInstruction()
{
    if (result)
    {
        result->SetInst(nullptr);
    }
}

UnaryInstruction::UnaryInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_, OpCode opCode_) : 
    ValueInstruction(sourcePos_, result_, opCode_), operand(operand_)
{
}

void UnaryInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, operand);
}

void UnaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (operand == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, operand);
        operand = value;
        cmajor::systemx::intermediate::AddUser(this, operand);
    }
}

std::vector<Instruction*> UnaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, operand);
    return uses;
}

NotInstruction::NotInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : UnaryInstruction(sourcePos_, result_, operand_, OpCode::not_)
{
}

void NotInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NegInstruction::NegInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : UnaryInstruction(sourcePos_, result_, operand_, OpCode::neg)
{
}

void NegInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SignExtendInstruction::SignExtendInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::signextend)
{
}

void SignExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ZeroExtendInstruction::ZeroExtendInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::zeroextend)
{
}

void ZeroExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TruncateInstruction::TruncateInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::truncate)
{
}

void TruncateInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BitcastInstruction::BitcastInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::bitcast)
{
}

void BitcastInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntToFloatInstruction::IntToFloatInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::inttofloat)
{
}

void IntToFloatInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatToIntInstruction::FloatToIntInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::floattoint)
{
}

void FloatToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntToPtrInstruction::IntToPtrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::inttoptr)
{
}

void IntToPtrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PtrToIntInstruction::PtrToIntInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::ptrtoint)
{
}

void PtrToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BinaryInstruction::BinaryInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_) :
    ValueInstruction(sourcePos_, result_, opCode_), left(left_), right(right_)
{
}

void BinaryInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, left);
    cmajor::systemx::intermediate::AddUser(this, right);
}

void BinaryInstruction::ReplaceValue(Value* use, Value* value)
{
    if (left == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, left);
        left = value;
        cmajor::systemx::intermediate::AddUser(this, left);
    }
    if (right == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, right);
        right = value;
        cmajor::systemx::intermediate::AddUser(this, right);
    }
}

std::vector<Instruction*> BinaryInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, left);
    AddToUsesVec(uses, right);
    return uses;
}

AddInstruction::AddInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) :
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::add)
{
}

void AddInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SubInstruction::SubInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::sub)
{
}

void SubInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MulInstruction::MulInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::mul)
{
}

void MulInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DivInstruction::DivInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::div_)
{
}

void DivInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModInstruction::ModInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::mod)
{
}

void ModInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AndInstruction::AndInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::and_)
{
}

void AndInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OrInstruction::OrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::or_)
{
}

void OrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

XorInstruction::XorInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::xor_)
{
}

void XorInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShlInstruction::ShlInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::shl)
{
}

void ShlInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShrInstruction::ShrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::shr)
{
}

void ShrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EqualInstruction::EqualInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::equal)
{
}

void EqualInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LessInstruction::LessInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::less)
{
}

void LessInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParamInstruction::ParamInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_) : ValueInstruction(sourcePos_, result_, OpCode::param), paramIndex(0)
{
}

void ParamInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LocalInstruction::LocalInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Type* localType_) : 
    ValueInstruction(sourcePos_, result_, OpCode::local), localType(localType_)
{
}

void LocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LoadInstruction::LoadInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* ptr_) : ValueInstruction(sourcePos_, result_, OpCode::load), ptr(ptr_)
{
}

void LoadInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LoadInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, ptr);
}

void LoadInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::systemx::intermediate::AddUser(this, ptr);
    }
}

std::vector<Instruction*> LoadInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    return uses;
}

ElemAddrInstruction::ElemAddrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* ptr_, Value* indexValue_) :
    ValueInstruction(sourcePos_, result_, OpCode::elemaddr), ptr(ptr_), indexValue(indexValue_)
{
}

void ElemAddrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ElemAddrInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, ptr);
    cmajor::systemx::intermediate::AddUser(this, indexValue);
}

void ElemAddrInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::systemx::intermediate::AddUser(this, ptr);
    }
    if (indexValue == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, indexValue);
        indexValue = value;
        cmajor::systemx::intermediate::AddUser(this, indexValue);
    }
}

std::vector<Instruction*> ElemAddrInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, indexValue);
    return uses;
}

ElemAddrKind ElemAddrInstruction::GetElemAddrKind(Context* context) const
{
    if (ptr->GetType()->IsPointerType())
    {
        PointerType* ptrType = static_cast<PointerType*>(ptr->GetType());
        Type* pointeeType = ptrType->RemovePointer(GetSourcePos(), context);
        if (pointeeType->IsArrayType())
        {
            return ElemAddrKind::array;
        }
        else if (pointeeType->IsStructureType())
        {
            return ElemAddrKind::structure;
        }
    }
    Error("invalid elem addr kind", GetSourcePos(), context);
    return ElemAddrKind::none;
}

PtrOffsetInstruction::PtrOffsetInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* ptr_, Value* offset_) :
    ValueInstruction(sourcePos_, result_, OpCode::ptroffset), ptr(ptr_), offset(offset_)
{
}

void PtrOffsetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrOffsetInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, ptr);
    cmajor::systemx::intermediate::AddUser(this, offset);
}

void PtrOffsetInstruction::ReplaceValue(Value* use, Value* value)
{
    if (ptr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, ptr);
        ptr = value;
        cmajor::systemx::intermediate::AddUser(this, ptr);
    }
    if (offset == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, offset);
        offset = value;
        cmajor::systemx::intermediate::AddUser(this, offset);
    }
}

std::vector<Instruction*> PtrOffsetInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, ptr);
    AddToUsesVec(uses, offset);
    return uses;
}

PtrDiffInstruction::PtrDiffInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* leftPtr_, Value* rightPtr_) :
    ValueInstruction(sourcePos_, result_, OpCode::ptrdiff), leftPtr(leftPtr_), rightPtr(rightPtr_)
{
}

void PtrDiffInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrDiffInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, leftPtr);
    cmajor::systemx::intermediate::AddUser(this, rightPtr);
}

void PtrDiffInstruction::ReplaceValue(Value* use, Value* value)
{
    if (leftPtr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, leftPtr);
        leftPtr = value;
        cmajor::systemx::intermediate::AddUser(this, leftPtr);
    }
    if (rightPtr == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, rightPtr);
        rightPtr = value;
        cmajor::systemx::intermediate::AddUser(this, rightPtr);
    }
}

std::vector<Instruction*> PtrDiffInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    AddToUsesVec(uses, leftPtr);
    AddToUsesVec(uses, rightPtr);
    return uses;
}

FunctionCallInstruction::FunctionCallInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* callee_) :
    ValueInstruction(sourcePos_, result_, OpCode::function_call), callee(callee_)
{
}

void FunctionCallInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionCallInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

void FunctionCallInstruction::AddToUses()
{
    cmajor::systemx::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::systemx::intermediate::AddUser(this, arg);
    }
}

void FunctionCallInstruction::ReplaceValue(Value* use, Value* value)
{
    if (callee == use)
    {
        cmajor::systemx::intermediate::RemoveUser(this, callee);
        callee = value;
        cmajor::systemx::intermediate::AddUser(this, callee);
    }
    for (auto& arg : args)
    {
        if (arg == use)
        {
            cmajor::systemx::intermediate::RemoveUser(this, arg);
            arg = value;
            cmajor::systemx::intermediate::AddUser(this, arg);
        }
    }
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

TrapInstruction::TrapInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* op1_, Value* op2_, Value* op3_) :
    ValueInstruction(sourcePos_, result_, OpCode::trap), op1(op1_), op2(op2_), op3(op3_), args()
{
}

void TrapInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TrapInstruction::SetArgs(std::vector<Value*>&& args_)
{
    args = std::move(args_);
}

void TrapInstruction::AddToUses()
{
    for (auto& arg : args)
    {
        cmajor::systemx::intermediate::AddUser(this, arg);
    }
}

void TrapInstruction::ReplaceValue(Value* use, Value* value)
{
    for (auto& arg : args)
    {
        if (arg == use)
        {
            cmajor::systemx::intermediate::RemoveUser(this, arg);
            arg = value;
            cmajor::systemx::intermediate::AddUser(this, arg);
        }
    }
}

std::vector<Instruction*> TrapInstruction::Uses() const
{
    std::vector<Instruction*> uses;
    for (const auto& arg : args)
    {
        AddToUsesVec(uses, arg);
    }
    return uses;
}

NoOperationInstruction::NoOperationInstruction(const soul::ast::SourcePos& sourcePos_) : Instruction(sourcePos_, nullptr, OpCode::nop)
{
}

void NoOperationInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BasicBlock::BasicBlock(const soul::ast::SourcePos& sourcePos_, int32_t id_) : sourcePos(sourcePos_), id(id_), instructions(this)
{
}

void BasicBlock::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
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

void BasicBlock::AddInstruction(Instruction* instruction, MetadataRef* metadataRef)
{
    instruction->SetMetadataRef(metadataRef);
    instructions.AddChild(instruction);
    if (instruction->IsValueInstruction())
    {
        ValueInstruction* valueInstruction = static_cast<ValueInstruction*>(instruction);
        Function* function = Parent();
        Context* context = function->Parent()->GetContext();
        function->MapInstruction(valueInstruction->Result()->Reg(), valueInstruction, context);
    }
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

Function::Function(const soul::ast::SourcePos& sourcePos_, FunctionType* type_, const std::string& name_, bool once_, bool main_, bool definition_, 
    MetadataRef* metadataRef_) :
    flags(FunctionFlags::none), sourcePos(sourcePos_), type(type_), name(name_), metadataRef(metadataRef_), basicBlocks(this), nextRegNumber(0)
{
    if (once_)
    {
        SetFlag(FunctionFlags::once);
    }
    if (main_)
    {
        SetMain();
    }
    if (definition_)
    {
        SetFlag(FunctionFlags::defined);
    }
}

void Function::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
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

BasicBlock* Function::AddBasicBlock(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context)
{
    BasicBlock* prev = GetBasicBlock(id);
    if (prev)
    {
        Error("error adding basic block: basic block id not unique", sourcePos, context, prev->GetSourcePos());
    }
    BasicBlock* basicBlock = new BasicBlock(sourcePos, id);
    basicBlocks.AddChild(basicBlock);
    basicBlockMap[id] = basicBlock;
    return basicBlock;
}

bool Function::RemoveBasicBlock(BasicBlock* block)
{
    if (!GetBasicBlock(block->Id())) return false;
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
    basicBlocks.RemoveChild(block);
    return true;
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

RegValue* Function::GetRegRef(const soul::ast::SourcePos& sourcePos, Type* type, int32_t reg, Context* context) const
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
            Error("error referencing register value " + std::to_string(reg) + ": type conflick", sourcePos, context, regValue->GetSourcePos());
        }
    }
    else
    {
        Error("error referencing register: register " + std::to_string(reg) + " not found", sourcePos, context);
    }
    return nullptr;
}

RegValue* Function::MakeRegValue(const soul::ast::SourcePos& sourcePos, Type* type, int32_t reg, Context* context)
{
    RegValue* prev = GetRegValue(reg);
    if (prev)
    {
        Error("error adding register " + std::to_string(reg) + ": register not unique", sourcePos, context, prev->GetSourcePos());
    }
    RegValue* regValue = new RegValue(sourcePos, type, reg);
    regValues.push_back(std::unique_ptr<RegValue>(regValue));
    regValueMap[reg] = regValue;
    return regValue;
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
        Error("error mappint instruction " + std::to_string(reg) + ": register number not unique", sourcePos, context, prev->GetSourcePos());
    }
    instructionMap[reg] = inst;
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
        Error("error adding entry and exit blocks: function '" + Name() + "' has no basic blocks", GetSourcePos(), Parent()->GetContext());
    }
    BasicBlock* prevEntryBlock = GetBasicBlock(entryBlockId);
    if (prevEntryBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an entry block", GetSourcePos(), Parent()->GetContext());
    }
    BasicBlock* prevExitBlock = GetBasicBlock(exitBlockId);
    if (prevExitBlock)
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has already an exit block", GetSourcePos(), Parent()->GetContext());
    }
    if (RetBlocks().empty())
    {
        Error("error adding entry and exit blocks: function '" + Name() + "' has no ret blocks", GetSourcePos(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = FirstBasicBlock();
    BasicBlock* entryBlock = new BasicBlock(GetSourcePos(), entryBlockId);
    basicBlockMap[entryBlock->Id()] = entryBlock;
    entryBlock->AddSuccessor(firstBasicBlock);
    firstBasicBlock->AddPredecessor(entryBlock);
    basicBlocks.InsertBefore(entryBlock, firstBasicBlock);
    BasicBlock* exitBlock = new BasicBlock(GetSourcePos(), exitBlockId);
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
        Error("error removing entry and exit blocks: function '" + Name() + "' has no entry block", GetSourcePos(), Parent()->GetContext());
    }
    BasicBlock* exitBlock = GetBasicBlock(exitBlockId);
    if (!exitBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no exit block", GetSourcePos(), Parent()->GetContext());
    }
    BasicBlock* firstBasicBlock = entryBlock->Next();
    if (!firstBasicBlock)
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' has no basic blocks other than entry and exit blocks", GetSourcePos(), Parent()->GetContext());
    }
    if (!firstBasicBlock->RemovePredecessor(entryBlock))
    {
        Error("error removing entry and exit blocks: function '" + Name() + "' old first block does not have the entry block as a predecessor", GetSourcePos(), Parent()->GetContext());
    }
    basicBlocks.RemoveChild(entryBlock);
    basicBlockMap.erase(entryBlockId);
    for (BasicBlock* retBlock : RetBlocks())
    {
        if (!retBlock->RemoveSuccessor(exitBlock))
        {
            Error("error removing entry and exit blocks: function '" + Name() + "' ret block does not have the exit block as a successor", GetSourcePos(), Parent()->GetContext());
        }
    }
    basicBlocks.RemoveChild(exitBlock);
    basicBlockMap.erase(exitBlockId);
}

soul::xml::Element* Function::ToXml()
{
    soul::xml::Element* element = soul::xml::MakeElement("function");
    element->SetAttribute("name", Name());
    BasicBlock* block = FirstBasicBlock();
    while (block)
    {
        soul::xml::Element* blockElement = soul::xml::MakeElement("block");
        blockElement->SetAttribute("id", block->Name());
        for (BasicBlock* successor : block->Successors())
        {
            soul::xml::Element* successorElement = soul::xml::MakeElement("successor");
            successorElement->SetAttribute("id", successor->Name());
            blockElement->AppendChild(successorElement);
        }
        for (BasicBlock* predecessor : block->Predecessors())
        {
            soul::xml::Element* predecessorElement = soul::xml::MakeElement("predecessor");
            predecessorElement->SetAttribute("id", predecessor->Name());
            blockElement->AppendChild(predecessorElement);
        }
        element->AppendChild(blockElement);
        block = block->Next();
    }
    return element;
}

void Function::WriteXmlDocument(const std::string& filePath)
{
    soul::xml::Element* element = ToXml();
    soul::xml::Document document;
    document.AppendChild(element);
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    document.Write(formatter);
}

Code::Code() : context(nullptr), currentFunction(nullptr), functions(this)
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

Function* Code::AddFunctionDefinition(const soul::ast::SourcePos& sourcePos, FunctionType* functionType, const std::string& functionId, bool once, bool main, 
    MetadataRef* metadataRef, Context* context)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->IsDefined())
        {
            Error("error adding function '" + functionId + "': function id not unique", sourcePos, context, prev->GetSourcePos());
        }
        else
        {
            if (prev->GetType() != functionType)
            {
                Error("error adding function '" + functionId + "': type '" + functionType->Name() + "' conflicts with earlier declaration", sourcePos, context, 
                    prev->GetSourcePos());
            }
            prev->SetDefined();
            return prev;
        }
    }
    Function* function = new Function(sourcePos, functionType, functionId, once, main, true, metadataRef);
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

Function* Code::AddFunctionDeclaration(const soul::ast::SourcePos& sourcePos, FunctionType* functionType, const std::string& functionId)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->GetType() != functionType)
        {
            Error("error adding function declaration '" + functionId + "': type '" + functionType->Name() + "' conflicts with earlier declaration", sourcePos, context, prev->GetSourcePos());
        }
        return prev;
    }
    Function* function = new Function(sourcePos, functionType, functionId, false, false, false, nullptr);
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

void Code::VisitFunctions(Visitor& visitor)
{
    Function* function = FirstFunction();
    while (function)
    {
        function->Accept(visitor);
        function = function->Next();
    }
}

} // cmajor::systemx::intermediate
