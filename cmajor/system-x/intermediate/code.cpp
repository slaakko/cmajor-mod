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

std::string RegValue::ToString() const
{
    return "$" + std::to_string(inst->RegValueIndex());
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

void Instruction::WriteMetadataRef(util::CodeFormatter& formatter)
{
    if (metadataRef)
    {
        formatter.Write(" ");
        metadataRef->Write(formatter);
    }
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
    WriteMetadataRef(formatter);
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

void ArgInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("arg ", 8));
    formatter.Write(arg->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(arg->ToString());
    WriteMetadataRef(formatter);
}

JmpInstruction::JmpInstruction(const soul::ast::SourcePos& sourcePos_, int32_t targetLabelId_) :
    Instruction(sourcePos_, nullptr, OpCode::jmp), targetLabelId(targetLabelId_), targetBasicBlock(nullptr)
{
}

void JmpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void JmpInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("jmp ", 8));
    formatter.Write("@" + std::to_string(targetBasicBlock->Id()));
    WriteMetadataRef(formatter);
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
    WriteMetadataRef(formatter);
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

void ProcedureCallInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format("call ", 8));
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
    WriteMetadataRef(formatter);
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
    WriteMetadataRef(formatter);
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

void SwitchInstruction::AddCase(Value* caseValue, BasicBlock* caseDest)
{
    CaseTarget caseTarget(caseValue, caseDest->Id());
    caseTarget.targetBlock = caseDest;
    AddCaseTarget(caseTarget);
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
    WriteMetadataRef(formatter);
}

ValueInstruction::ValueInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, OpCode opCode_) : 
    Instruction(sourcePos_, result_->GetType(), opCode_), result(result_)
{
    result->SetInst(this);
    SetRegValueIndex(result->Reg());
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

std::string ValueInstruction::ToString() const
{
    return result->ToString();
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

void UnaryInstruction::WriteArg(util::CodeFormatter& formatter)
{
    formatter.Write(operand->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(operand->ToString());
}

NotInstruction::NotInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : UnaryInstruction(sourcePos_, result_, operand_, OpCode::not_)
{
}

void NotInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NotInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = not ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

NegInstruction::NegInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : UnaryInstruction(sourcePos_, result_, operand_, OpCode::neg)
{
}

void NegInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NegInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = neg ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

SignExtendInstruction::SignExtendInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::signextend)
{
}

void SignExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SignExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = signextend ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

ZeroExtendInstruction::ZeroExtendInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::zeroextend)
{
}

void ZeroExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ZeroExtendInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = zeroextend ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

TruncateInstruction::TruncateInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::truncate)
{
}

void TruncateInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TruncateInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = truncate ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

BitcastInstruction::BitcastInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::bitcast)
{
}

void BitcastInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BitcastInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = bitcast ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

IntToFloatInstruction::IntToFloatInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::inttofloat)
{
}

void IntToFloatInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntToFloatInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttofloat ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

FloatToIntInstruction::FloatToIntInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::floattoint)
{
}

void FloatToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FloatToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = floattoint ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

IntToPtrInstruction::IntToPtrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::inttoptr)
{
}

void IntToPtrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntToPtrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = inttoptr ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
}

PtrToIntInstruction::PtrToIntInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* operand_) : 
    UnaryInstruction(sourcePos_, result_, operand_, OpCode::ptrtoint)
{
}

void PtrToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrToIntInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = ptrtoint ");
    WriteArg(formatter);
    WriteMetadataRef(formatter);
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

AddInstruction::AddInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) :
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::add)
{
}

void AddInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AddInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = add ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

SubInstruction::SubInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::sub)
{
}

void SubInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SubInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = sub ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

MulInstruction::MulInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::mul)
{
}

void MulInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void MulInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mul ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

DivInstruction::DivInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::div_)
{
}

void DivInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DivInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = div ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

ModInstruction::ModInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::mod)
{
}

void ModInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ModInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = mod ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

AndInstruction::AndInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::and_)
{
}

void AndInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AndInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = and ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

OrInstruction::OrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::or_)
{
}

void OrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void OrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = or ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

XorInstruction::XorInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::xor_)
{
}

void XorInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void XorInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = xor ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

ShlInstruction::ShlInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::shl)
{
}

void ShlInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ShlInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shl ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

ShrInstruction::ShrInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::shr)
{
}

void ShrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ShrInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = shr ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

EqualInstruction::EqualInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::equal)
{
}

void EqualInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EqualInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = equal ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

LessInstruction::LessInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Value* left_, Value* right_) : 
    BinaryInstruction(sourcePos_, result_, left_, right_, OpCode::less)
{
}

void LessInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LessInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = less ");
    WriteArgs(formatter);
    WriteMetadataRef(formatter);
}

ParamInstruction::ParamInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_) : ValueInstruction(sourcePos_, result_, OpCode::param), paramIndex(0)
{
}

void ParamInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParamInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = param");
    WriteMetadataRef(formatter);
}

LocalInstruction::LocalInstruction(const soul::ast::SourcePos& sourcePos_, RegValue* result_, Type* localType_) : 
    ValueInstruction(sourcePos_, result_, OpCode::local), localType(localType_), localMetadataRef(nullptr), offset(-1)
{
}

void LocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LocalInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = local ");
    formatter.Write(localType->Name());
    WriteMetadataRef(formatter);
    if (localMetadataRef)
    {
        formatter.Write(" ");
        localMetadataRef->Write(formatter);
    }
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

void LoadInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = load ");
    formatter.Write(ptr->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(ptr->ToString());
    WriteMetadataRef(formatter);
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
    WriteMetadataRef(formatter);
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
    WriteMetadataRef(formatter);
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
    WriteMetadataRef(formatter);
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

void FunctionCallInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = call ");
    formatter.Write(callee->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(callee->ToString());
    WriteMetadataRef(formatter);
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

void TrapInstruction::Write(util::CodeFormatter& formatter)
{
    WriteResult(formatter);
    formatter.Write(" = trap ");
    formatter.Write(op1->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(op1->ToString());
    formatter.Write(", ");
    formatter.Write(op2->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(op2->ToString());
    formatter.Write(", ");
    formatter.Write(op3->GetType()->Name());
    formatter.Write(" ");
    formatter.Write(op3->ToString());
    WriteMetadataRef(formatter);
}

NoOperationInstruction::NoOperationInstruction(const soul::ast::SourcePos& sourcePos_) : Instruction(sourcePos_, nullptr, OpCode::nop)
{
}

void NoOperationInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NoOperationInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.Write("nop");
    WriteMetadataRef(formatter);
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

void BasicBlock::AddInstruction(Instruction* instruction)
{
    instructions.AddChild(instruction);
    if (instruction->IsLocalInstruction())
    {
        Parent()->AddLocal(static_cast<LocalInstruction*>(instruction));
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
    if (instruction->IsLocalInstruction())
    {
        Parent()->AddLocal(static_cast<LocalInstruction*>(instruction));
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

Function::Function(const soul::ast::SourcePos& sourcePos_, Type* type_, const std::string& name_, bool once_, bool main_, bool inline_, bool definition_, 
    MetadataRef* metadataRef_, bool parsing, Context* context) : 
    Value(sourcePos_, ValueKind::function, type_), flags(FunctionFlags::none), sourcePos(sourcePos_), functionType(nullptr), name(name_), metadataRef(metadataRef_), 
    basicBlocks(this), nextRegNumber(0), nextBBNumber(0), nextIndex(0)
{
    Type* t = GetType();
    if (t->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(t);
        t = pointerType->BaseType();
    }
    if (t->IsFunctionType())
    {
        functionType = static_cast<FunctionType*>(t);
    }
    else
    {
        Error("function type or pointer to function type expected", sourcePos_, context);
    }
    if (!parsing)
    {
        entryBlock.reset(new BasicBlock(soul::ast::SourcePos(), nextBBNumber++));
        int n = functionType->ParamTypeRefs().size();
        for (int i = 0; i < n; ++i)
        {
            Type* paramType = functionType->ParamType(i);
            Instruction* paramInst = new ParamInstruction(soul::ast::SourcePos(), MakeRegValue(paramType));
            entryBlock->AddInstruction(paramInst);
            params.push_back(paramInst);
        }

    }
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

Instruction* Function::GetParam(int index) const
{
    return params[index];
}

void Function::AddLocal(LocalInstruction* local)
{
    if (std::find(locals.begin(), locals.end(), local) == locals.end())
    {
        locals.push_back(local);
    }
}

LocalInstruction* Function::GetLocal(int index) const
{
    if (index >= 0 && index < NumLocals())
    {
        return locals[index];
    }
    else
    {
        throw std::runtime_error("invalid local index");
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

BasicBlock* Function::CreateBasicBlock()
{
    if (basicBlocks.IsEmpty())
    {
        BasicBlock* bb = entryBlock.get();
        basicBlocks.AddChild(entryBlock.release());
        return bb;
    }
    BasicBlock* bb = new BasicBlock(soul::ast::SourcePos(), nextBBNumber++);
    basicBlocks.AddChild(bb);
    return bb;
}

BasicBlock* Function::CreateCleanupBasicBlock()
{
    BasicBlock* cubb = new BasicBlock(soul::ast::SourcePos(), -1);
    cleanupBasicBlocks.push_back(std::unique_ptr<BasicBlock>(cubb));
    return cubb;
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

RegValue* Function::MakeRegValue(Type* type)
{
    RegValue* regValue = new RegValue(soul::ast::SourcePos(), type, regValues.size());
    regValues.push_back(std::unique_ptr<RegValue>(regValue));
    regValueMap[regValue->Reg()] = regValue;
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

bool Function::IndexSeen(int32_t index) const
{
    return indexSet.find(index) != indexSet.end();
}

void Function::AddIndex(int32_t index)
{
    indexSet.insert(index);
}

void Function::MapRegValue(RegValue* regValue)
{
    regValueMap[regValue->Reg()] = regValue;
}

void Function::MapLineColLen(const soul::ast::LineColLen& lineColLen)
{
    auto it = lineColLenIndexMap.find(lineColLen);
    if (it == lineColLenIndexMap.end())
    {
        lineColLenIndexMap[lineColLen] = nextIndex++;
    }
}

int32_t Function::GetLineColLenIndex(const soul::ast::LineColLen& lineColLen) const
{
    auto it = lineColLenIndexMap.find(lineColLen);
    if (it != lineColLenIndexMap.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
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

void Function::Finalize()
{
    nextBBNumber = 0;
    for (std::unique_ptr<BasicBlock>& cubb : cleanupBasicBlocks)
    {
        basicBlocks.AddChild(cubb.release());
    }
    BasicBlock* bb = FirstBasicBlock();
    while (bb)
    {
        bb->SetId(nextBBNumber++);
        bb = bb->Next();
    }
    if (!basicBlocks.IsEmpty())
    {
        SetDefined();
    }
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
    if (Once())
    {
        formatter.Write("once ");
    }
    if (IsMain())
    {
        formatter.Write("main_fn ");
    }
    Context* context = Parent()->GetContext();
    context->GetTypes().Map(functionType);
    Type* ptrType = functionType->AddPointer(context);
    int mdId = -1;
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
            bb = bb->Next();
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

Function* Code::AddFunctionDefinition(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId, bool once, bool main, bool inline_,
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
            if (prev->GetType() != type)
            {
                Error("error adding function '" + functionId + "': type '" + type->Name() + "' conflicts with earlier declaration", sourcePos, context, 
                    prev->GetSourcePos());
            }
            prev->SetDefined();
            return prev;
        }
    }
    Function* function = new Function(sourcePos, type, functionId, once, main, inline_, true, metadataRef, true, context);
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

Function* Code::AddFunctionDeclaration(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId)
{
    Function* prev = GetFunction(functionId);
    if (prev)
    {
        if (prev->GetType() != type)
        {
            Error("error adding function declaration '" + functionId + "': type '" + type->Name() + "' conflicts with earlier declaration", sourcePos, context, prev->GetSourcePos());
        }
        return prev;
    }
    Function* function = new Function(sourcePos, type, functionId, false, false, false, false, nullptr, true, context);
    functions.AddChild(function);
    functionMap[function->Name()] = function;
    return function;
}

Function* Code::GetOrInsertFunction(const std::string& name, Type* type)
{
    Function* prev = GetFunction(name);
    if (prev)
    {
        return prev;
    }
    Function* function = new Function(soul::ast::SourcePos(), type, name, false, false, false, false, nullptr, false, context);
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
