// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.codegen;

import cmajor.systemx.assembler;
import soul.ast.source.pos;
import soul.ast.span;
import std.core;

export namespace cmajor::systemx::intermediate {

class Function;
class Value;
class Type;
class Context;
struct Register;
struct FrameLocation;
class RegisterAllocator;
class Instruction;
class ParamInstruction;
class LoadInstruction;
class StoreInstruction;
class NotInstruction;
class NegInstruction;
class SignExtendInstruction;
class ZeroExtendInstruction;
class SwitchInstruction;
class BinaryInstruction;
class EqualInstruction;
class LessInstruction;
class JmpInstruction;
class BranchInstruction;
class NoOperationInstruction;
class RetInstruction;
class TrapInstruction;
class ArgInstruction;
class ProcedureCallInstruction;
class FunctionCallInstruction;
class ElemAddrInstruction;
class PtrOffsetInstruction;
class PtrDiffInstruction;
class BitcastInstruction;
class PtrToIntInstruction;
class IntToPtrInstruction;
class FloatToIntInstruction;
class IntToFloatInstruction;
class TruncateInstruction;
class BoolValue;
class SByteValue;
class ByteValue;
class ShortValue;
class UShortValue;
class IntValue;
class UIntValue;
class LongValue;
class ULongValue;
class FloatValue;
class DoubleValue;
class AddressValue;
class SymbolValue;
class StringValue;
class CompileUnit;

class CodeGenerator
{
public:
    CodeGenerator();
    virtual ~CodeGenerator();
    virtual Context* Ctx() const = 0;
    virtual void GenerateCode() = 0;
    virtual void WriteOutputFile() = 0;
    virtual const soul::ast::SourcePos& GetSourcePos() const = 0;
    virtual RegisterAllocator* RegAllocator() const = 0;
    virtual void Emit(cmajor::systemx::assembler::Instruction* assemblyInstruction) = 0;
    virtual void EmitOcta(uint64_t value) = 0;
    virtual void EmitTetra(uint32_t value) = 0;
    virtual void EmitWyde(uint16_t value) = 0;
    virtual void EmitByte(uint8_t value) = 0;
    virtual void EmitSymbol(const std::string& name) = 0;
    virtual int ExitLabelId() const = 0;
    virtual void EmitClsId(const std::string& typeId) = 0;
    virtual void EmitDebugInfoInst(cmajor::systemx::assembler::Instruction* assemblyInstruction) = 0;
    virtual void Error(const std::string& message) = 0;
    virtual void AddSourceFileInfo(CompileUnit& compileUnit) = 0;
    virtual void GenerateDebugInfo() = 0;
    virtual void SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen, int32_t index) = 0;
    virtual void BeginTry(uint32_t tryBlockId, uint32_t parentTryBlockId) = 0;
    virtual void EndTry(uint32_t tryBlockId) = 0;
    virtual void Catch(uint32_t catchBlockId, uint32_t tryBlockId, const std::string& caughtTypeIdStr) = 0;
    virtual void BeginCleanup(uint32_t cleanupBlockId, uint32_t tryBlockId) = 0;
    virtual void EndCleanup(uint32_t cleanupBlockId) = 0;
    virtual Function* CurrentFunction() const = 0;
    virtual const soul::ast::LineColLen& CurrentLineColLen() const = 0;
    virtual cmajor::systemx::assembler::AssemblyFile* AssemblyFile() const = 0;
    std::thread::id CreatorThreadId() const { return creatorThreadId; }
private:
    std::thread::id creatorThreadId;
};

cmajor::systemx::assembler::Node* MakeRegOperand(const Register& reg);
int64_t GetIndex(Value* index, CodeGenerator& codeGen);
int64_t GetOffset(Type* type, int64_t index, CodeGenerator& codeGen);
int64_t GetElementSize(Type* type, CodeGenerator& codeGen);
int64_t GetPointeeSize(Type* type, CodeGenerator& codeGen);
FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGen);
void EmitFrameLocationOperand(const FrameLocation& frameLocation, cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen);
void EmitArgLocationOperand(cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen);
void EmitPtrOperand(Value* value, cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen);
cmajor::systemx::assembler::Node* MakeRegOperand(Value* value, const Register& reg, CodeGenerator& codeGen);
cmajor::systemx::assembler::Node* MakeCalleeOperand(Value* value, const Register& reg, CodeGenerator& codeGen);
cmajor::systemx::assembler::Node* MakeTrapOperand(Value* value, CodeGenerator& codeGen);
void EmitParam(ParamInstruction& inst, CodeGenerator& codeGen);
void EmitLoad(const FrameLocation& frameLocation, const Register& reg, CodeGenerator& codeGen);
void EmitStore(const FrameLocation& frameLocation, const Register& reg, CodeGenerator& codeGen);
void EmitLoad(LoadInstruction& inst, CodeGenerator& codeGen);
void EmitStore(StoreInstruction& inst, CodeGenerator& codeGen);
void EmitNot(NotInstruction& inst, CodeGenerator& codeGen);
void EmitNeg(NegInstruction& inst, CodeGenerator& codeGen);
void EmitSignExtension(SignExtendInstruction& inst, CodeGenerator& codeGen);
void EmitZeroExtension(ZeroExtendInstruction& inst, CodeGenerator& codeGen);
void EmitSwitch(SwitchInstruction& inst, CodeGenerator& codeGen);
void EmitBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGen);
void EmitEqual(EqualInstruction& inst, CodeGenerator& codeGen);
void EmitLess(LessInstruction& inst, CodeGenerator& codeGen);
void EmitJmp(JmpInstruction& inst, CodeGenerator& codeGen);
void EmitBranch(BranchInstruction& inst, CodeGenerator& codeGen);
void EmitArg(ArgInstruction& inst, CodeGenerator& codeGen, cmajor::systemx::assembler::Instruction* targetInst, bool emitArgLocationOperand);
void EmitProcedureCall(ProcedureCallInstruction& inst, CodeGenerator& codeGen);
void EmitFunctionCall(FunctionCallInstruction& inst, CodeGenerator& codeGen);
void EmitRet(RetInstruction& inst, CodeGenerator& codeGen);
void EmitTrap(TrapInstruction& inst, CodeGenerator& codeGen);
void EmitNop(NoOperationInstruction& inst, CodeGenerator& codeGen);
void EmitElemAddr(ElemAddrInstruction& inst, CodeGenerator& codeGen);
void EmitPtrOffset(PtrOffsetInstruction& inst, CodeGenerator& codeGen);
void EmitPtrDiff(PtrDiffInstruction& inst, CodeGenerator& codeGen);
void EmitBitcast(BitcastInstruction& inst, CodeGenerator& codeGen);
void EmitPtrToInt(PtrToIntInstruction& inst, CodeGenerator& codeGen);
void EmitIntToPtr(IntToPtrInstruction& inst, CodeGenerator& codeGen);
void EmitFloatToInt(FloatToIntInstruction& inst, CodeGenerator& codeGen);
void EmitIntToFloat(IntToFloatInstruction& inst, CodeGenerator& codeGen);
void EmitTruncate(TruncateInstruction& inst, CodeGenerator& codeGen);
void EmitPrologue(CodeGenerator& codeGen);
void EmitEpilogue(CodeGenerator& codeGen);
void EmitBool(BoolValue& value, CodeGenerator& codeGen);
void EmitSByte(SByteValue& value, CodeGenerator& codeGen);
void EmitByte(ByteValue& value, CodeGenerator& codeGen);
void EmitShort(ShortValue& value, CodeGenerator& codeGen);
void EmitUShort(UShortValue& value, CodeGenerator& codeGen);
void EmitInt(IntValue& value, CodeGenerator& codeGen);
void EmitUInt(UIntValue& value, CodeGenerator& codeGen);
void EmitLong(LongValue& value, CodeGenerator& codeGen);
void EmitULong(ULongValue& value, CodeGenerator& codeGen);
void EmitFloat(FloatValue& value, CodeGenerator& codeGen);
void EmitDouble(DoubleValue& value, CodeGenerator& codeGen);
void EmitNull(CodeGenerator& codeGen);
void EmitAddress(AddressValue& value, CodeGenerator& codeGen);
void EmitSymbol(SymbolValue& value, CodeGenerator& codeGen);
void EmitString(StringValue& value, CodeGenerator& codeGen);
void EmitClsId(const std::string& typeId, CodeGenerator& codeGen);
void EmitSourceFileNameDebugInfo(const std::string& sourceFileName, int64_t id, CodeGenerator& codeGen);
void EmitFunctionDebugInfo(Function* function, int64_t frameSize, CodeGenerator& codeGen);
void EmitLineNumberInfo(const soul::ast::LineColLen& currentLineColLen, int32_t index, CodeGenerator& codeGen);
void EmitBeginTry(uint32_t tryBlockId, uint32_t parentTryBlockId, CodeGenerator& codeGen);
void EmitEndTry(uint32_t tryBlockId, CodeGenerator& codeGen);
void EmitCatch(uint32_t catchBlockId, uint32_t tryBlockId, uint64_t caughtTypeId1, uint64_t caughtTypeId2, CodeGenerator& codeGen);
void EmitBeginCleanup(uint32_t cleanupBlockId, uint32_t tryBlockId, CodeGenerator& codeGen);
void EmitEndCleanup(uint32_t cleanupBlockId, CodeGenerator& codeGen);
void ProcessInstructionMetadata(Instruction* inst, CodeGenerator& codeGen);

} // cmajor::systemx::intermediate
