// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.ir.instruction;

import cmajor.debug;
import cmajor.cpp.ir.value;
import util;
import std.core;

export namespace cmajor::cpp::ir {

class BasicBlock;
class Function;
class MDStructRef;

class Instruction : public Value
{
public:
    Instruction();
    Type* GetType(Context& context) override;
    virtual void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) = 0;
    std::string Name(Context& context) override;
    virtual void ObtainResultId(Function& function);
    virtual void WriteResultDeclaration(util::CodeFormatter& formatter, Function& function, Context& context);
    virtual void WriteResult(util::CodeFormatter& formatter, Function& function, Context& context);
    virtual bool IsResultInstruction(Context& context) { return false; }
    virtual bool IsNoOperation() const { return false; }
    virtual std::string IrName() const = 0;
    void SetSourceSpan(const cmajor::debug::SourceSpan& span_) { span = span_; } 
    const cmajor::debug::SourceSpan& GetSourceSpan() const { return span; } 
    void SetResultId(int resultId_) { resultId = resultId_; }
    void SetNoSemicolon() { noSemicolon = true; }
    bool NoSemicolon() const { return noSemicolon; }
    virtual void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks);
    void WriteDebugInfoRecord(util::BinaryStreamWriter& writer, int32_t& numInsts);
    int32_t CppLineNumber() const { return cppLineNumber; }
    void SetCppLineNumber(int32_t cppLineNumber_) { cppLineNumber = cppLineNumber_; }
    int32_t CppLineIndex() const { return cppLineIndex; }
    void SetCppLineIndex(int32_t cppLineIndex_) { cppLineIndex = cppLineIndex_; }
    void SetLineNumbers(util::CodeFormatter& formatter, Context& context);
    int16_t ScopeId() const { return scopeId; }
    void SetScopeId(int16_t scopeId_) { scopeId = scopeId_; }
    int16_t Flags() const { return flags; }
    void SetFlags(int16_t flags_) { flags = flags_; }
private:
    int resultId;
    cmajor::debug::SourceSpan span; 
    int32_t cppLineNumber;
    int32_t cppLineIndex;
    int16_t flags;
    int16_t scopeId;
    bool noSemicolon;
};

class UnaryInstruction : public Instruction
{
public:
    UnaryInstruction(Value* arg_);
    bool IsResultInstruction(Context& context) override { return true; }
    Type* GetType(Context& context) override { return arg->GetType(context); }
    void WriteArg(util::CodeFormatter& formatter, Context& context);
    Value* Arg() const { return arg; }
private:
    Value* arg;
};

class UnaryTypeInstruction : public UnaryInstruction
{
public:
    UnaryTypeInstruction(Value* arg_, Type* type_);
    Type* GetType(Context& context) override { return type; }
private:
    Type* type;
};

class BinaryInstruction : public Instruction
{
public:
    BinaryInstruction(Value* left_, Value* right_);
    bool IsResultInstruction(Context& context) override { return true; }
    Type* GetType(Context& context) override;
    void WriteArgs(util::CodeFormatter& formatter, Context& context, const std::string& op);
private:
    Value* left;
    Value* right;
};

class NotInstruction : public UnaryInstruction
{
public:
    NotInstruction(Value* arg_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "not"; }
};

class NegInstruction : public UnaryInstruction
{
public:
    NegInstruction(Value* arg_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "neg"; }
};

class AddInstruction : public BinaryInstruction
{
public:
    AddInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "add"; }
};

class SubInstruction : public BinaryInstruction
{
public:
    SubInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "sub"; }
};

class MulInstruction : public BinaryInstruction
{
public:
    MulInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "mul"; }
};

class DivInstruction : public BinaryInstruction
{
public:
    DivInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "div"; }
};

class ModInstruction : public BinaryInstruction
{
public:
    ModInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "mod"; }
};

class AndInstruction : public BinaryInstruction
{
public:
    AndInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "and"; }
};

class OrInstruction : public BinaryInstruction
{
public:
    OrInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "or"; }
};

class XorInstruction : public BinaryInstruction
{
public:
    XorInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "xor"; }
};

class ShlInstruction : public BinaryInstruction
{
public:
    ShlInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "shl"; }
};

class ShrInstruction : public BinaryInstruction
{
public:
    ShrInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "shr"; }
};

class EqualInstruction : public BinaryInstruction
{
public:
    EqualInstruction(Value* left_, Value* right_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "equal"; }
};

class LessInstruction : public BinaryInstruction
{
public:
    LessInstruction(Value* left_, Value* right_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "less"; }
};

class SignExtendInstruction : public UnaryTypeInstruction
{
public:
    SignExtendInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "signextend"; }
};

class ZeroExtendInstruction : public UnaryTypeInstruction
{
public:
    ZeroExtendInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "zeroextend"; }
};

class TruncateInstruction : public UnaryTypeInstruction
{
public:
    TruncateInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "truncate"; }
};

class BitCastInstruction : public UnaryTypeInstruction
{
public:
    BitCastInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "bitcast"; }
};

class IntToFloatInstruction : public UnaryTypeInstruction
{
public:
    IntToFloatInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "inttofloat"; }
};

class FloatToIntInstruction : public UnaryTypeInstruction
{
public:
    FloatToIntInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "floattoint"; }
};

class IntToPtrInstruction : public UnaryTypeInstruction
{
public:
    IntToPtrInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "inttoptr"; }
};

class PtrToIntInstruction : public UnaryTypeInstruction
{
public:
    PtrToIntInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "ptrtoint"; }
};

class ParamInstruction : public Instruction
{
public:
    ParamInstruction(Type* type_, const std::string& paramName_);
    Type* GetType(Context& context) override { return type; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string Name(Context& context) override { return paramName; }
    std::string IrName() const override { return "param"; }
private:
    Type* type;
    std::string paramName;
};

class LocalInstruction : public Instruction
{
public:
    LocalInstruction(Type* type_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override { return true; }
    void ObtainResultId(Function& function) override;
    void WriteResultDeclaration(util::CodeFormatter& formatter, Function& function, Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "local"; }
    const std::string& LocalName() const { return localName; }
    void ObtainLocalName(Function& function);
private:
    Type* type;
    std::string localName;
};

class LoadInstruction : public Instruction
{
public:
    LoadInstruction(Value* ptr_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override { return true; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "load"; }
private:
    Value* ptr;
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(Value* value_, Value* ptr_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "store"; }
private:
    Value* value;
    Value* ptr;
};

class ArgInstruction : public Instruction
{
public:
    ArgInstruction(Value* arg_);
    void ObtainResultId(Function& function) override;
    void WriteResultDeclaration(util::CodeFormatter& formatter, Function& function, Context& context) override;
    void WriteResult(util::CodeFormatter& formatter, Function& function, Context& context) override;
    bool IsResultInstruction(Context& context) override { return true; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "arg"; }
    std::string Name(Context& context) override { return argName; }
private:
    Value* arg;
    std::string argName;
};

class ElemAddrInstruction : public Instruction
{
public:
    ElemAddrInstruction(Value* ptr_, Value* index_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override { return true; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "elemaddr"; }
private:
    Value* ptr;
    Value* index;
};

class PtrOffsetInstruction : public Instruction
{
public:
    PtrOffsetInstruction(Value* ptr_, Value* offset_);
    Type* GetType(Context& context) override { return ptr->GetType(context); }
    bool IsResultInstruction(Context& context) override { return true; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "ptroffset"; }
private:
    Value* ptr;
    Value* offset;
};

class PtrDiffInstruction : public Instruction
{
public:
    PtrDiffInstruction(Value* leftPtr_, Value* rightPtr_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override { return true; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "ptrdiff"; }
private:
    Value* leftPtr;
    Value* rightPtr;
};

class CallInstruction : public Instruction
{
public:
    CallInstruction(Value* function_, const std::vector<Value*>& args_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "call"; }
private:
    Value* function;
    std::vector<Value*> args;
};

class InvokeInstruction : public Instruction
{
public:
    InvokeInstruction(Value* function_, const std::vector<Value*> args_, BasicBlock* normalBlockNext_, BasicBlock* unwindBlockNext_);
    Type* GetType(Context& context) override;
    bool IsResultInstruction(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "invoke"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    Value* function;
    std::vector<Value*> args;
    BasicBlock* normalBlockNext;
    BasicBlock* unwindBlockNext;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(Value* value_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "ret"; }
private:
    Value* value;
};

class JumpInstruction : public Instruction
{
public:
    JumpInstruction(BasicBlock* dest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "jmp"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    BasicBlock* dest;
};

class BranchInstruction : public Instruction
{
public:
    BranchInstruction(Value* cond_, BasicBlock* trueDest_, BasicBlock* falseDest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "branch"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    Value* cond;
    BasicBlock* trueDest;
    BasicBlock* falseDest;
};

class SwitchInstruction : public Instruction
{
public:
    SwitchInstruction(Value* cond_, BasicBlock* defaultDest_);
    void AddCase(Value* caseValue, BasicBlock* dest);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "switch"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    Value* cond;
    BasicBlock* defaultDest;
    std::vector<std::pair<Value*, BasicBlock*>> destinations;
};

class NoOperationInstruction : public Instruction
{
public:
    NoOperationInstruction();
    bool IsResultInstruction(Context& context) override { return true; }
    void WriteResultDeclaration(util::CodeFormatter& formatter, Function& function, Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    bool IsNoOperation() const override { return true; }
    std::string IrName() const override { return "nop"; }
};

class BeginTryInstruction : public Instruction
{
public:
    BeginTryInstruction();
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "beginTry"; }
};

class EndTryInstruction : public Instruction
{
public:
    EndTryInstruction(BasicBlock* nextDest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "endTry"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    BasicBlock* nextDest;
};

class BeginCatchInstruction : public Instruction
{
public:
    BeginCatchInstruction();
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "beginCatch"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
};

class EndCatchInstruction : public Instruction
{
public:
    EndCatchInstruction(BasicBlock* nextDest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "endCatch"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    BasicBlock* nextDest;
};

class ResumeInstruction : public Instruction
{
public:
    ResumeInstruction();
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "resume"; }
};

class IncludeBasicBlockInstruction : public Instruction
{
public:
    IncludeBasicBlockInstruction(BasicBlock* block_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "include"; }
    void CollectReferencedBasicBlocks(BasicBlock* parent, std::set<BasicBlock*>& basicBlocks) override;
private:
    BasicBlock* block;
};

class StartFunctionInstruction : public Instruction
{
public:
    StartFunctionInstruction();
    void Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords) override;
    std::string IrName() const override { return "start"; }
};

} // namespace cmajor::cpp::ir
