// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

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
    "not", "neg", "signextend", "zeroextend", "truncate", "bitcast", "inttofloat", "floattoint", "inttoptr", "ptrtoint",
    "add", "sub", "mul", "div", "mod", "and", "or", "xor", "shl", "shr", "equal", "less",
    "param", "local", "load", "elemaddr", "ptroffset", "ptrdiff", "call", "nop"
};

RegValue::RegValue(const soul::ast::Span& span_, Type* type_, int32_t reg_) : Value(span_, ValueKind::regValue, type_), reg(reg_), inst(nullptr)
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
                if (user != regValue->Inst())
                {
                    regValue->Inst()->AddUser(user);
                }
            }
        }
    }
}

Instruction::Instruction(const soul::ast::Span& span_, Type* type_, OpCode opCode_) : Value(span_, ValueKind::instruction, type_), opCode(opCode_), index(-1)
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

BasicBlock* Instruction::Parent() const
{
    return static_cast<BasicBlock*>(GetContainer()->Parent());
}

void Instruction::AddUser(Instruction* user)
{
    AddPtrToSet(user, users);
}

void Instruction::AddToUses()
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

void StoreInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArgInstruction::ArgInstruction(const soul::ast::Span& span_, Value* arg_) : Instruction(span_, nullptr, OpCode::arg), arg(arg_)
{
}

void ArgInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArgInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, arg);
}

JmpInstruction::JmpInstruction(const soul::ast::Span& span_, int32_t targetLabelId_) :
    Instruction(span_, nullptr, OpCode::jmp), targetLabelId(targetLabelId_), targetBasicBlock(nullptr)
{
}

void JmpInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
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

void BranchInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, cond);
}

ProcedureCallInstruction::ProcedureCallInstruction(const soul::ast::Span& span_, Value* callee_) : Instruction(span_, nullptr, OpCode::procedure_call), callee(callee_)
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
    cmajor::masm::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::masm::intermediate::AddUser(this, arg);
    }
}

RetInstruction::RetInstruction(const soul::ast::Span& span_, Value* returnValue_) : Instruction(span_, nullptr, OpCode::ret), returnValue(returnValue_)
{
}

void RetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RetInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, returnValue);
}

SwitchInstruction::SwitchInstruction(const soul::ast::Span& span_, Value* cond_, int32_t defaultTargetLabelId_) :
    Instruction(span_, nullptr, OpCode::switch_), cond(cond_), defaultTargetLabelId(defaultTargetLabelId_), defaultTargetBlock(nullptr)
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
    cmajor::masm::intermediate::AddUser(this, cond);
    for (auto& caseTarget : caseTargets)
    {
        cmajor::masm::intermediate::AddUser(this, caseTarget.caseValue);
    }
}

ValueInstruction::ValueInstruction(const soul::ast::Span& span_, RegValue* result_, OpCode opCode_) : Instruction(span_, result_->GetType(), opCode_), result(result_)
{
}

void ValueInstruction::AddToUses()
{
    cmajor::masm::intermediate::AddUser(this, result);
}

UnaryInstruction::UnaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_, OpCode opCode_) : ValueInstruction(span_, result_, opCode_), operand(operand_)
{
}

void UnaryInstruction::AddToUses()
{
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, operand);
}

NotInstruction::NotInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::not_)
{
}

void NotInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NegInstruction::NegInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::neg)
{
}

void NegInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SignExtendInstruction::SignExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::signextend)
{
}

void SignExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ZeroExtendInstruction::ZeroExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::zeroextend)
{
}

void ZeroExtendInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TruncateInstruction::TruncateInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::truncate)
{
}

void TruncateInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BitcastInstruction::BitcastInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::bitcast)
{
}

void BitcastInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntToFloatInstruction::IntToFloatInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::inttofloat)
{
}

void IntToFloatInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatToIntInstruction::FloatToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::floattoint)
{
}

void FloatToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntToPtrInstruction::IntToPtrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::inttoptr)
{
}

void IntToPtrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PtrToIntInstruction::PtrToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_) : UnaryInstruction(span_, result_, operand_, OpCode::ptrtoint)
{
}

void PtrToIntInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BinaryInstruction::BinaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_) :
    ValueInstruction(span_, result_, opCode_), left(left_), right(right_)
{
}

void BinaryInstruction::AddToUses()
{
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, left);
    cmajor::masm::intermediate::AddUser(this, right);
}

AddInstruction::AddInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::add)
{
}

void AddInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SubInstruction::SubInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::sub)
{
}

void SubInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MulInstruction::MulInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::mul)
{
}

void MulInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DivInstruction::DivInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::div_)
{
}

void DivInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ModInstruction::ModInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::mod)
{
}

void ModInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AndInstruction::AndInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::and_)
{
}

void AndInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OrInstruction::OrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::or_)
{
}

void OrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

XorInstruction::XorInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::xor_)
{
}

void XorInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShlInstruction::ShlInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::shl)
{
}

void ShlInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShrInstruction::ShrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::shr)
{
}

void ShrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EqualInstruction::EqualInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::equal)
{
}

void EqualInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LessInstruction::LessInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_) : BinaryInstruction(span_, result_, left_, right_, OpCode::less)
{
}

void LessInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParamInstruction::ParamInstruction(const soul::ast::Span& span_, RegValue* result_) : ValueInstruction(span_, result_, OpCode::param)
{
}

void ParamInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LocalInstruction::LocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_) : ValueInstruction(span_, result_, OpCode::local), localType(localType_)
{
}

void LocalInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LoadInstruction::LoadInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_) : ValueInstruction(span_, result_, OpCode::load), ptr(ptr_)
{
}

void LoadInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LoadInstruction::AddToUses()
{
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, ptr);
}

ElemAddrInstruction::ElemAddrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* index_) :
    ValueInstruction(span_, result_, OpCode::elemaddr), ptr(ptr_), index(index_)
{
}

void ElemAddrInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
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
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, ptr);
    cmajor::masm::intermediate::AddUser(this, index);
}

PtrOffsetInstruction::PtrOffsetInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* offset_) :
    ValueInstruction(span_, result_, OpCode::ptroffset), ptr(ptr_), offset(offset_)
{
}

void PtrOffsetInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrOffsetInstruction::AddToUses()
{
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, ptr);
    cmajor::masm::intermediate::AddUser(this, offset);
}

PtrDiffInstruction::PtrDiffInstruction(const soul::ast::Span& span_, RegValue* result_, Value* leftPtr_, Value* rightPtr_) :
    ValueInstruction(span_, result_, OpCode::ptrdiff), leftPtr(leftPtr_), rightPtr(rightPtr_)
{
}

void PtrDiffInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrDiffInstruction::AddToUses()
{
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, leftPtr);
    cmajor::masm::intermediate::AddUser(this, rightPtr);
}

FunctionCallInstruction::FunctionCallInstruction(const soul::ast::Span& span_, RegValue* result_, Value* callee_) :
    ValueInstruction(span_, result_, OpCode::function_call), callee(callee_)
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
    ValueInstruction::AddToUses();
    cmajor::masm::intermediate::AddUser(this, callee);
    for (auto& arg : args)
    {
        cmajor::masm::intermediate::AddUser(this, arg);
    }
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

BasicBlock::BasicBlock(const soul::ast::Span& span_, int32_t id_) : span(span_), id(id_), instructions(this)
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

Function::Function(const soul::ast::Span& span_, FunctionType* type_, const std::string& name_, bool definition_) :
    flags(FunctionFlags::none), span(span_), type(type_), name(name_), basicBlocks(this), nextRegNumber(0)
{
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

Function* Code::AddFunctionDefinition(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId, Context* context)
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
            return prev;
        }
    }
    Function* function = new Function(span, functionType, functionId, true);
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
    Function* function = new Function(span, functionType, functionId, false);
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

} // cmajor::masm::intermediate
