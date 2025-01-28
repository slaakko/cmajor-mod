// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.instruction;

import cmajor.systemx.ir.value;
import util;
import std.core;

export namespace cmajor::systemx::ir {

class Type;
class BasicBlock;
class Function;
class Context;
class MDStructRef;

class Instruction : public Value
{
public:
    Instruction();
    Type* GetType(Context& context) override;
    virtual void Write(util::CodeFormatter& formatter, Function& function, Context& context) = 0;
    std::string Name(Context& context) override;
    void WriteResult(util::CodeFormatter& formatter, Function& function, Context& context);
    void SetMetadataRef(MDStructRef* metadataRef_) { metadataRef = metadataRef_; }
    void WriteMetadataRef(util::CodeFormatter& formatter);
    virtual bool IsNoOperation() const { return false; }
private:
    int resultId;
    MDStructRef* metadataRef;
};

class UnaryInstruction : public Instruction
{
public:
    UnaryInstruction(Value* arg_);
    Type* GetType(Context& context) override { return arg->GetType(context); }
    void WriteArg(util::CodeFormatter& formatter, Context& context);
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
    Type* GetType(Context& context) override;
    void WriteArgs(util::CodeFormatter& formatter, Context& context);
private:
    Value* left;
    Value* right;
};

class NotInstruction : public UnaryInstruction
{
public:
    NotInstruction(Value* arg_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class NegInstruction : public UnaryInstruction
{
public:
    NegInstruction(Value* arg_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class AddInstruction : public BinaryInstruction
{
public:
    AddInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class SubInstruction : public BinaryInstruction
{
public:
    SubInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class MulInstruction : public BinaryInstruction
{
public:
    MulInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class DivInstruction : public BinaryInstruction
{
public:
    DivInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class ModInstruction : public BinaryInstruction
{
public:
    ModInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class AndInstruction : public BinaryInstruction
{
public:
    AndInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class OrInstruction : public BinaryInstruction
{
public:
    OrInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class XorInstruction : public BinaryInstruction
{
public:
    XorInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class ShlInstruction : public BinaryInstruction
{
public:
    ShlInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class ShrInstruction : public BinaryInstruction
{
public:
    ShrInstruction(Value* left_, Value* right_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class EqualInstruction : public BinaryInstruction
{
public:
    EqualInstruction(Value* left_, Value* right_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class LessInstruction : public BinaryInstruction
{
public:
    LessInstruction(Value* left_, Value* right_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class SignExtendInstruction : public UnaryTypeInstruction
{
public:
    SignExtendInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class ZeroExtendInstruction : public UnaryTypeInstruction
{
public:
    ZeroExtendInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class TruncateInstruction : public UnaryTypeInstruction
{
public:
    TruncateInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class BitCastInstruction : public UnaryTypeInstruction
{
public:
    BitCastInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class IntToFloatInstruction : public UnaryTypeInstruction
{
public:
    IntToFloatInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class FloatToIntInstruction : public UnaryTypeInstruction
{
public:
    FloatToIntInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class IntToPtrInstruction : public UnaryTypeInstruction
{
public:
    IntToPtrInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class PtrToIntInstruction : public UnaryTypeInstruction
{
public:
    PtrToIntInstruction(Value* arg_, Type* destType_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
};

class ParamInstruction : public Instruction
{
public:
    ParamInstruction(Type* type_);
    Type* GetType(Context& context) override { return type; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Type* type;
};

class LocalInstruction : public Instruction
{
public:
    LocalInstruction(Type* type_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Type* type;
};

class LoadInstruction : public Instruction
{
public:
    LoadInstruction(Value* ptr_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* ptr;
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(Value* value_, Value* ptr_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* value;
    Value* ptr;
};

class ArgInstruction : public Instruction
{
public:
    ArgInstruction(Value* arg_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* arg;
};

class ElemAddrInstruction : public Instruction
{
public:
    ElemAddrInstruction(Value* ptr_, Value* index_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* ptr;
    Value* index;
};

class PtrOffsetInstruction : public Instruction
{
public:
    PtrOffsetInstruction(Value* ptr_, Value* offset_);
    Type* GetType(Context& context) override { return ptr->GetType(context); }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* ptr;
    Value* offset;
};

class PtrDiffInstruction : public Instruction
{
public:
    PtrDiffInstruction(Value* leftPtr_, Value* rightPtr_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* leftPtr;
    Value* rightPtr;
};

class CallInstruction : public Instruction
{
public:
    CallInstruction(Value* function_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* function;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(Value* value_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* value;
};

class JumpInstruction : public Instruction
{
public:
    JumpInstruction(BasicBlock* dest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    BasicBlock* dest;
};

class BranchInstruction : public Instruction
{
public:
    BranchInstruction(Value* cond_, BasicBlock* trueDest_, BasicBlock* falseDest_);
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
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
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* cond;
    BasicBlock* defaultDest;
    std::vector<std::pair<Value*, BasicBlock*>> destinations;
};

class TrapInstruction : public Instruction
{
public:
    TrapInstruction(Value* b0_, Value* b1_, Value* b2_);
    Type* GetType(Context& context) override;
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
private:
    Value* b0;
    Value* b1;
    Value* b2;
};

class NoOperationInstruction : public Instruction
{
public:
    NoOperationInstruction();
    void Write(util::CodeFormatter& formatter, Function& function, Context& context) override;
    bool IsNoOperation() const override { return true; }
};

} // namespace cmajor::systemx::ir
