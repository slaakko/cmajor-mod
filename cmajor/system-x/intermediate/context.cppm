// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.context;

import cmajor.systemx.intermediate.compile.unit;
import cmajor.systemx.intermediate.types;
import cmajor.systemx.intermediate.value;
import cmajor.systemx.intermediate.data;
import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.metadata;
import cmajor.systemx.intermediate.reg.allocator;
import soul.lexer;
import soul.ast.span;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::systemx::intermediate {

    export namespace context {
    }

class RegisterPool;

class Context
{
public:
    Context();
    CompileUnit& GetCompileUnit() { return compileUnit; }
    Types& GetTypes() { return types; }
    Data& GetData() { return data; }
    Code& GetCode() { return code; }
    Metadata& GetMetadata() { return metadata; }
    void SetFilePath(const std::string& filePath_);
    const std::string& FilePath() const;
    void Write(const std::string& intermediateFilePath);
    std::string ErrorLines(const soul::ast::SourcePos& sourcePos);
    void SetCompileUnitInfo(const std::string& compileUnitId_, MetadataRef* metadataRef);
    void AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* metadataRef);
    void AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef, MetadataRef* metadataRef);
    void AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs, 
        MetadataRef* metadataRef);
    void AddGlobalVariable(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& variableName, ConstantValue* initializer, bool once);
    GlobalVariable* GetOrInsertGlobal(const std::string& name, Type* type);
    void ResolveTypes();
    void ResolveType(TypeRef& typeRef);
    ConstantValue* GetTrueValue();
    ConstantValue* GetFalseValue();
    ConstantValue* GetBooleanLiteral(const soul::ast::SourcePos& sourcePos, Type* type, bool value);
    ConstantValue* GetSByteValue(int8_t value);
    ConstantValue* GetByteValue(uint8_t value);
    ConstantValue* GetShortValue(int16_t value);
    ConstantValue* GetUShortValue(uint16_t value);
    ConstantValue* GetIntValue(int32_t value);
    ConstantValue* GetUIntValue(uint32_t value);
    ConstantValue* GetLongValue(int64_t value);
    ConstantValue* GetULongValue(uint64_t value);
    ConstantValue* GetFloatValue(float value);
    ConstantValue* GetDoubleValue(double value);
    ConstantValue* GetCharValue(char value);
    ConstantValue* GetWCharValue(char16_t value);
    ConstantValue* GetUCharValue(char32_t value);
    ConstantValue* GetNullValue(const soul::ast::SourcePos& sourcePos, Type* type);
    ConstantValue* MakeArrayValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& elements, const std::string& prefix);
    ConstantValue* MakeStructureValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& fieldValues);
    ConstantValue* MakeStringValue(const soul::ast::SourcePos& sourcePos, const std::string& value, bool crop);
    ConstantValue* MakeStringArrayValue(const soul::ast::SourcePos& sourcePos, char prefix, const std::vector<ConstantValue*>& strings);
    ConstantValue* MakeConversionValue(const soul::ast::SourcePos& sourcePos, Type* type, ConstantValue* from);
    ConstantValue* MakeClsIdValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& clsIdStr, bool crop);
    ConstantValue* MakeSymbolValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& symbol);
    ConstantValue* MakeNumericLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& strValue);
    ConstantValue* MakeAddressLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& id, bool resolve);
    Function* CurrentFunction() const;
    void SetCurrentFunction(Function* function);
    Function* AddFunctionDefinition(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId, bool once, bool main, bool inline_, 
        MetadataRef* metadataRef);
    Function* AddFunctionDeclaration(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId);
    Function* GetOrInsertFunction(const std::string& name, Type* type);
    Instruction* CreateNot(Value* arg);
    Instruction* CreateNeg(Value* arg);
    Instruction* CreateAdd(Value* left, Value* right);
    Instruction* CreateSub(Value* left, Value* right);
    Instruction* CreateMul(Value* left, Value* right);
    Instruction* CreateDiv(Value* left, Value* right);
    Instruction* CreateMod(Value* left, Value* right);
    Instruction* CreateAnd(Value* left, Value* right);
    Instruction* CreateOr(Value* left, Value* right);
    Instruction* CreateXor(Value* left, Value* right);
    Instruction* CreateShl(Value* left, Value* right);
    Instruction* CreateShr(Value* left, Value* right);
    Instruction* CreateEqual(Value* left, Value* right);
    Instruction* CreateLess(Value* left, Value* right);
    Instruction* CreateSignExtend(Value* arg, Type* destType);
    Instruction* CreateZeroExtend(Value* arg, Type* destType);
    Instruction* CreateTruncate(Value* arg, Type* destType);
    Instruction* CreateBitCast(Value* arg, Type* destType);
    Instruction* CreateIntToFloat(Value* arg, Type* destType);
    Instruction* CreateFloatToInt(Value* arg, Type* destType);
    Instruction* CreateIntToPtr(Value* arg, Type* destType);
    Instruction* CreatePtrToInt(Value* arg, Type* destType);
    Instruction* CreateParam(Type* type);
    Instruction* CreateLocal(Type* type);
    Instruction* CreateLoad(Value* ptr);
    Instruction* CreateStore(Value* value, Value* ptr);
    Instruction* CreateArg(Value* arg);
    Instruction* CreateElemAddr(Value* ptr, Value* index);
    Instruction* CreatePtrOffset(Value* ptr, Value* offset);
    Instruction* CreatePtrDiff(Value* leftPtr, Value* rightPtr);
    Instruction* CreateCall(Value* function);
    Instruction* CreateRet(Value* value);
    Instruction* CreateJump(BasicBlock* dest);
    Instruction* CreateBranch(Value* cond, BasicBlock* trueDest, BasicBlock* falseDest);
    Instruction* CreateSwitch(Value* cond, BasicBlock* defaultDest);
    Instruction* CreateTrap(const std::vector<Value*>& args);
    Instruction* CreateNop();
    void SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen);
    int32_t GetLineColLenIndex(const soul::ast::LineColLen& lineColLen) const;
    void AddLineInfo(Instruction* inst);
    MetadataStruct* AddMetadataStruct(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context);
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value, bool crop);
    MetadataArray* CreateMetadataArray();
    MetadataRef* CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId);
    void ResolveMetadataReferences();
    MetadataRef* GetMetadataRefForSourceFile(const std::string& sourceFileName);
    RegisterPool& GetRegisterPool() { return registerPool; }
    void ResetRegisterPool();
    soul::lexer::FileMap& GetFileMap() { return fileMap; }
    void SetCurrentBasicBlock(BasicBlock* bb) { currentBasicBlock = bb; }
    BasicBlock* GetCurrentBasicBlock() const { return currentBasicBlock; }
private:
    CompileUnit compileUnit;
    Types types;
    Data data;
    Code code;
    Metadata metadata;
    RegisterPool registerPool;
    soul::lexer::FileMap fileMap;
    BasicBlock* currentBasicBlock;
    soul::ast::LineColLen currentLineColLen;
    std::map<soul::ast::LineColLen, MetadataRef*> lineColLenInfoMap;
    std::map<std::string, MetadataRef*> sourceFileMap;
};

} // cmajor::systemx::intermediate
