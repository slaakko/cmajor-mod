// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.code;

import cmajor.masm.intermediate.data;
import util;
import std.core;

export namespace cmajor::masm::intermediate {

namespace code {}

class Instruction;
class BasicBlock;
class Function;
class Code;

class RegValue : public Value
{
public:
    RegValue(const soul::ast::Span& span_, Type* type_, int32_t reg_);
    int32_t Reg() const { return reg; }
    void SetInst(Instruction* inst_) { inst = inst_; }
    Instruction* Inst() const { return inst; }
private:
    int32_t reg;
    Instruction* inst;
};

enum class OpCode : int
{
    store, arg, jmp, branch, procedure_call, ret, switch_,
    not_, neg, signextend, zeroextend, truncate, bitcast, inttofloat, floattoint, inttoptr, ptrtoint,
    add, sub, mul, div_, mod, and_, or_, xor_, shl, shr, equal, less,
    param, local, load, elemaddr, ptroffset, ptrdiff, function_call,
    nop
};

void AddUser(Instruction* user, Value* value);

class Instruction : public Value, public util::Component
{
public:
    Instruction(const soul::ast::Span& span_, Type* type_, OpCode opCode_);
    std::string Name() const;
    virtual void Accept(Visitor& visitor) = 0;
    BasicBlock* Parent() const;
    Instruction* Next() { return static_cast<Instruction*>(NextSibling()); }
    Instruction* Prev() { return static_cast<Instruction*>(PrevSibling()); }
    OpCode GetOpCode() const { return opCode; }
    bool IsLeader() const;
    bool IsTerminator() const;
    bool IsValueInstruction() const;
    bool IsUnaryInstruction() const;
    bool IsBinaryInstruction() const;
    bool IsSwitchInstruction() const { return opCode == OpCode::switch_; }
    bool IsParamInstruction() const { return opCode == OpCode::param; }
    bool IsLocalInstruction() const { return opCode == OpCode::local; }
    bool IsLoadInstruction() const { return opCode == OpCode::load; }
    bool IsStoreInstruction() const { return opCode == OpCode::store; }
    bool IsElemAddrInstruction() const { return opCode == OpCode::elemaddr; }
    bool IsArgInstruction() const { return opCode == OpCode::arg; }
    bool IsFunctionCallInstruction() const { return opCode == OpCode::function_call; }
    bool IsProcedureCallInstruction() const { return opCode == OpCode::procedure_call; }
    bool RequiresLocalRegister() const;
    virtual bool IsFloatingPointInstruction() const { return false; }
    std::vector<BasicBlock*> Successors() const;
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    const std::vector<Instruction*>& Users() const { return users; }
    void AddUser(Instruction* user);
    virtual void AddToUses();
private:
    OpCode opCode;
    int index;
    std::vector<Instruction*> users;
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(const soul::ast::Span& span_, Value* value_, Value* ptr_);
    void Accept(Visitor& visitor) override;
    Value* GetValue() const { return value; }
    Value* GetPtr() const { return ptr; }
    bool IsFloatingPointInstruction() const override { return value->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
private:
    Value* value;
    Value* ptr;
};

class ArgInstruction : public Instruction
{
public:
    ArgInstruction(const soul::ast::Span& span_, Value* arg_);
    void Accept(Visitor& visitor) override;
    Value* Arg() const { return arg; }
    bool IsFloatingPointInstruction() const override { return arg->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
private:
    Value* arg;
};

class JmpInstruction : public Instruction
{
public:
    JmpInstruction(const soul::ast::Span& span_, int32_t targetLabelId_);
    void Accept(Visitor& visitor) override;
    int32_t TargetLabelId() const { return targetLabelId; }
    BasicBlock* TargetBasicBlock() const { return targetBasicBlock; }
    void SetTargetBasicBlock(BasicBlock* targetBasicBlock_) { targetBasicBlock = targetBasicBlock_; }
private:
    int32_t targetLabelId;
    BasicBlock* targetBasicBlock;
};

class BranchInstruction : public Instruction
{
public:
    BranchInstruction(const soul::ast::Span& span_, Value* cond_, int32_t trueTargetLabelId_, int32_t falseTargetLabelId_);
    void Accept(Visitor& visitor) override;
    Value* Cond() const { return cond; }
    int32_t TrueTargetLabelId() const { return trueTargetLabelId; }
    BasicBlock* TrueTargetBasicBlock() const { return trueTargetBasicBlock; }
    void SetTrueTargetBasicBlock(BasicBlock* trueTargetBasicBlock_) { trueTargetBasicBlock = trueTargetBasicBlock_; }
    int32_t FalseTargetLabelId() const { return falseTargetLabelId; }
    BasicBlock* FalseTargetBasicBlock() const { return falseTargetBasicBlock; }
    void SetFalseTargetBasicBlock(BasicBlock* falseTargetBasicBlock_) { falseTargetBasicBlock = falseTargetBasicBlock_; }
    void AddToUses() override;
private:
    Value* cond;
    int32_t trueTargetLabelId;
    BasicBlock* trueTargetBasicBlock;
    int32_t falseTargetLabelId;
    BasicBlock* falseTargetBasicBlock;
};

class ProcedureCallInstruction : public Instruction
{
public:
    ProcedureCallInstruction(const soul::ast::Span& span_, Value* callee_);
    void Accept(Visitor& visitor) override;
    Value* Callee() const { return callee; }
    const std::vector<Value*>& Args() const { return args; }
    void SetArgs(std::vector<Value*>&& args_);
    void AddToUses() override;
private:
    Value* callee;
    std::vector<Value*> args;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(const soul::ast::Span& span_, Value* returnValue_);
    void Accept(Visitor& visitor) override;
    Value* ReturnValue() const { return returnValue; }
    bool IsFloatingPointInstruction() const override;
    void AddToUses() override;
private:
    Value* returnValue;
};

struct CaseTarget
{
    CaseTarget(Value* caseValue_, int32_t targetLabelId_) : caseValue(caseValue_), targetLabelId(targetLabelId_), targetBlock(nullptr) {}
    Value* caseValue;
    int32_t targetLabelId;
    BasicBlock* targetBlock;
};

class SwitchInstruction : public Instruction
{
public:
    SwitchInstruction(const soul::ast::Span& span_, Value* cond_, int32_t defaultTargetLabelId_);
    void Accept(Visitor& visitor) override;
    Value* Cond() const { return cond; }
    int32_t DefaultTargetLabelId() const { return defaultTargetLabelId; }
    void AddCaseTarget(const CaseTarget& caseTarget);
    const std::vector<CaseTarget>& CaseTargets() const { return caseTargets; }
    std::vector<CaseTarget>& CaseTargets() { return caseTargets; }
    BasicBlock* DefaultTargetBlock() const { return defaultTargetBlock; }
    void SetDefaultTargetBlock(BasicBlock* defaultTargetBlock_) { defaultTargetBlock = defaultTargetBlock_; }
    void AddToUses() override;
private:
    Value* cond;
    int32_t defaultTargetLabelId;
    BasicBlock* defaultTargetBlock;
    std::vector<CaseTarget> caseTargets;
};

class ValueInstruction : public Instruction
{
public:
    ValueInstruction(const soul::ast::Span& span_, RegValue* result_, OpCode opCode_);
    RegValue* Result() const { return result; }
    bool IsFloatingPointInstruction() const override { return result->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
private:
    RegValue* result;
};

class UnaryInstruction : public ValueInstruction
{
public:
    UnaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_, OpCode opCode_);
    Value* Operand() const { return operand; }
    void SetOperand(Value* operand_) { operand = operand_; }
    void AddToUses() override;
private:
    Value* operand;
};

class NotInstruction : public UnaryInstruction
{
public:
    NotInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class NegInstruction : public UnaryInstruction
{
public:
    NegInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    bool IsFloatingPointInstruction() const override { return Operand()->GetType()->IsFloatingPointType(); }
    void Accept(Visitor& visitor) override;
};

class SignExtendInstruction : public UnaryInstruction
{
public:
    SignExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class ZeroExtendInstruction : public UnaryInstruction
{
public:
    ZeroExtendInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class TruncateInstruction : public UnaryInstruction
{
public:
    TruncateInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class BitcastInstruction : public UnaryInstruction
{
public:
    BitcastInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class IntToFloatInstruction : public UnaryInstruction
{
public:
    IntToFloatInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    bool IsFloatingPointInstruction() const override { return true; }
    void Accept(Visitor& visitor) override;
};

class FloatToIntInstruction : public UnaryInstruction
{
public:
    FloatToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    bool IsFloatingPointInstruction() const override { return false; }
    void Accept(Visitor& visitor) override;
};

class IntToPtrInstruction : public UnaryInstruction
{
public:
    IntToPtrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class PtrToIntInstruction : public UnaryInstruction
{
public:
    PtrToIntInstruction(const soul::ast::Span& span_, RegValue* result_, Value* operand_);
    void Accept(Visitor& visitor) override;
};

class BinaryInstruction : public ValueInstruction
{
public:
    BinaryInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_, OpCode opCode_);
    Value* Left() const { return left; }
    Value* Right() const { return right; }
    bool IsFloatingPointInstruction() const override { return Left()->GetType()->IsFloatingPointType(); }
    void AddToUses() override;
private:
    Value* left;
    Value* right;
};

class AddInstruction : public BinaryInstruction
{
public:
    AddInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class SubInstruction : public BinaryInstruction
{
public:
    SubInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class MulInstruction : public BinaryInstruction
{
public:
    MulInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class DivInstruction : public BinaryInstruction
{
public:
    DivInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class ModInstruction : public BinaryInstruction
{
public:
    ModInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class AndInstruction : public BinaryInstruction
{
public:
    AndInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class OrInstruction : public BinaryInstruction
{
public:
    OrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class XorInstruction : public BinaryInstruction
{
public:
    XorInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class ShlInstruction : public BinaryInstruction
{
public:
    ShlInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class ShrInstruction : public BinaryInstruction
{
public:
    ShrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
};

class EqualInstruction : public BinaryInstruction
{
public:
    EqualInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
    bool IsFloatingPointInstruction() const override { return false; }
};

class LessInstruction : public BinaryInstruction
{
public:
    LessInstruction(const soul::ast::Span& span_, RegValue* result_, Value* left_, Value* right_);
    void Accept(Visitor& visitor) override;
    bool IsFloatingPointInstruction() const override { return false; }
};

class ParamInstruction : public ValueInstruction
{
public:
    ParamInstruction(const soul::ast::Span& span_, RegValue* result_);
    void Accept(Visitor& visitor) override;
};

class LocalInstruction : public ValueInstruction
{
public:
    LocalInstruction(const soul::ast::Span& span_, RegValue* result_, Type* localType_);
    void Accept(Visitor& visitor) override;
    Type* LocalType() const { return localType; }
private:
    Type* localType;
};

class LoadInstruction : public ValueInstruction
{
public:
    LoadInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_);
    void Accept(Visitor& visitor) override;
    Value* Ptr() const { return ptr; }
    void AddToUses() override;
private:
    Value* ptr;
};

enum class ElemAddrKind
{
    none, array, structure
};

class ElemAddrInstruction : public ValueInstruction
{
public:
    ElemAddrInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* index_);
    void Accept(Visitor& visitor) override;
    Value* Ptr() const { return ptr; }
    Value* Index() const { return index; }
    ElemAddrKind GetElemAddrKind(Context* context) const;
    void AddToUses() override;
private:
    Value* ptr;
    Value* index;
};

class PtrOffsetInstruction : public ValueInstruction
{
public:
    PtrOffsetInstruction(const soul::ast::Span& span_, RegValue* result_, Value* ptr_, Value* offset_);
    void Accept(Visitor& visitor) override;
    Value* Ptr() const { return ptr; }
    Value* Offset() const { return offset; }
    void AddToUses() override;
private:
    Value* ptr;
    Value* offset;
};

class PtrDiffInstruction : public ValueInstruction
{
public:
    PtrDiffInstruction(const soul::ast::Span& span_, RegValue* result_, Value* leftPtr_, Value* rightPtr_);
    void Accept(Visitor& visitor) override;
    Value* LeftPtr() const { return leftPtr; }
    Value* RightPtr() const { return rightPtr; }
    void AddToUses() override;
private:
    Value* leftPtr;
    Value* rightPtr;
};

class FunctionCallInstruction : public ValueInstruction
{
public:
    FunctionCallInstruction(const soul::ast::Span& span_, RegValue* result_, Value* callee_);
    void Accept(Visitor& visitor) override;
    Value* Callee() const { return callee; }
    const std::vector<Value*>& Args() const { return args; }
    void SetArgs(std::vector<Value*>&& args_);
    void AddToUses() override;
private:
    Value* callee;
    std::vector<Value*> args;
};

struct BlockValue
{
    BlockValue(Value* value_, int32_t blockId_) : value(value_), blockId(blockId_), block(nullptr) {}
    BlockValue(Value* value_, BasicBlock* block_);
    Value* value;
    int32_t blockId;
    BasicBlock* block;
};

class NoOperationInstruction : public Instruction
{
public:
    NoOperationInstruction(const soul::ast::Span& span_);
    void Accept(Visitor& visitor) override;
};

const int32_t entryBlockId = -1;
const int32_t exitBlockId = -2;

class BasicBlock : public util::Component
{
public:
    BasicBlock(const soul::ast::Span& span_, int32_t id_);
    void Accept(Visitor& visitor);
    void VisitInstructions(Visitor& visitor);
    const soul::ast::Span& Span() const { return span; }
    std::string Name() const;
    int32_t Id() const { return id; }
    Function* Parent() const;
    BasicBlock* Next() { return static_cast<BasicBlock*>(NextSibling()); }
    BasicBlock* Prev() { return static_cast<BasicBlock*>(PrevSibling()); }
    Instruction* FirstInstruction() { return static_cast<Instruction*>(instructions.FirstChild()); }
    Instruction* LastInstruction() { return static_cast<Instruction*>(instructions.LastChild()); }
    void AddInstruction(Instruction* instruction);
    void InsertFront(Instruction* instruction);
    bool IsEmpty() const { return instructions.IsEmpty(); }
    bool IsEntryBlock() const { return id == entryBlockId; }
    bool IsExitBlock() const { return id == exitBlockId; }
    const std::vector<BasicBlock*>& Successors() const { return successors; }
    void AddSuccessor(BasicBlock* successor);
    bool RemoveSuccessor(BasicBlock* successor);
    const std::vector<BasicBlock*>& Predecessors() const { return predecessors; }
    void AddPredecessor(BasicBlock* predecessor);
    bool RemovePredecessor(BasicBlock* predecessor);
    void ClearSuccessorsAndPredecessors();
    int IndexOf(Instruction* x);
private:
    soul::ast::Span span;
    int32_t id;
    util::Container instructions;
    std::vector<BasicBlock*> successors;
    std::vector<BasicBlock*> predecessors;
};

enum class FunctionFlags : int
{
    none = 0, defined = 1 << 0
};

inline FunctionFlags operator|(FunctionFlags left, FunctionFlags right)
{
    return FunctionFlags(static_cast<int>(left) | static_cast<int>(right));
}

inline FunctionFlags operator&(FunctionFlags left, FunctionFlags right)
{
    return FunctionFlags(static_cast<int>(left) & static_cast<int>(right));
}

inline FunctionFlags operator~(FunctionFlags flags)
{
    return FunctionFlags(~static_cast<int>(flags));
}

class Function : public util::Component
{
public:
    Function(const soul::ast::Span& span_, FunctionType* functionType_, const std::string& name_, bool definition_);
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    bool GetFlag(FunctionFlags flag) const { return (flags & flag) != FunctionFlags::none; }
    void SetFlag(FunctionFlags flag) { flags = flags | flag; }
    void ResetFlag(FunctionFlags flag) { flags = flags & ~flag; }
    bool IsDefined() const { return GetFlag(FunctionFlags::defined); }
    bool IsExternal() const { return !IsDefined(); }
    void SetDefined() { SetFlag(FunctionFlags::defined); }
    int Arity() const { return type->Arity(); }
    void Accept(Visitor& visitor);
    void VisitBasicBlocks(Visitor& visitor);
    Code* Parent() const;
    Function* Next() { return static_cast<Function*>(NextSibling()); }
    Function* Prev() { return static_cast<Function*>(PrevSibling()); }
    bool IsEmpty() const { return basicBlocks.IsEmpty(); }
    BasicBlock* GetBasicBlock(int32_t id) const;
    BasicBlock* AddBasicBlock(const soul::ast::Span& span, int32_t id, Context* context);
    bool RemoveBasicBlock(BasicBlock* block);
    BasicBlock* FirstBasicBlock() { return static_cast<BasicBlock*>(basicBlocks.FirstChild()); }
    BasicBlock* LastBasicBlock() { return static_cast<BasicBlock*>(basicBlocks.LastChild()); }
    const soul::ast::Span& Span() const { return span; }
    FunctionType* GetType() const { return type; }
    const std::string& Name() const { return name; }
    RegValue* GetRegValue(int32_t reg) const;
    RegValue* GetRegRef(const soul::ast::Span& span, Type* type, int32_t reg, Context* context) const;
    RegValue* MakeRegValue(const soul::ast::Span& span, Type* type, int32_t reg, Context* context);
    Instruction* GetInstruction(int32_t reg) const;
    void MapInstruction(int32_t reg, Instruction* inst, Context* context);
    int NumBasicBlocks() const;
    const std::vector<BasicBlock*>& RetBlocks() const { return retBlocks; }
    void AddRetBlock(BasicBlock* retBlock);
    void AddEntryAndExitBlocks();
    void RemoveEntryAndExitBlocks();
    void WriteXmlDocument(const std::string& filePath);
    void SetNextRegNumber(int32_t nextRegNumber_) { nextRegNumber = nextRegNumber_; }
    int32_t NextRegNumber() const { return nextRegNumber; }
private:
    FunctionFlags flags;
    soul::ast::Span span;
    FunctionType* type;
    std::string name;
    util::Container basicBlocks;
    std::map<int32_t, BasicBlock*> basicBlockMap;
    std::map<int32_t, RegValue*> regValueMap;
    std::map<int32_t, Instruction*> instructionMap;
    std::vector<std::unique_ptr<RegValue>> regValues;
    std::vector<BasicBlock*> retBlocks;
    int32_t nextRegNumber;
};

class Code : public util::Component
{
public:
    Code();
    Code(const Code&) = delete;
    Code& operator=(const Code&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    Function* CurrentFunction() const { return currentFunction; }
    void SetCurrentFunction(Function* function);
    Function* GetFunction(const std::string& functionId) const;
    Function* AddFunctionDefinition(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId, Context* context);
    Function* AddFunctionDeclaration(const soul::ast::Span& span, FunctionType* functionType, const std::string& functionId);
    Function* FirstFunction() { return static_cast<Function*>(functions.FirstChild()); }
    Function* LastFunction() { return static_cast<Function*>(functions.LastChild()); }
    void VisitFunctions(Visitor& visitor);
private:
    Context* context;
    Function* currentFunction;
    util::Container functions;
    std::map<std::string, Function*> functionMap;
};

} // cmajor::masm::intermediate