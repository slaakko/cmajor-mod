// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.context;

import cmajor.systemx.intermediate.compile.unit;
import cmajor.systemx.intermediate.types;
import cmajor.systemx.intermediate.data;
import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.metadata;
import cmajor.systemx.intermediate.reg.allocator;
import soul.lexer;
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
    std::string ErrorLines(const soul::ast::SourcePos& sourcePos);
    void SetCompileUnitInfo(const std::string& compileUnitId_, MetadataRef* metadataRef);
    void AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs);
    void AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef);
    void AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs);
    void AddGlobalVariable(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& variableName, ConstantValue* initializer, bool once);
    void ResolveTypes();
    void ValidateData();
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
    ConstantValue* GetNullValue(const soul::ast::SourcePos& sourcePos, Type* type);
    ConstantValue* MakeArrayValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& elements);
    ConstantValue* MakeStructureValue(const soul::ast::SourcePos& sourcePos, const std::vector<ConstantValue*>& fieldValues);
    ConstantValue* MakeStringValue(const soul::ast::SourcePos& sourcePos, const std::string& value);
    ConstantValue* MakeStringArrayValue(const soul::ast::SourcePos& sourcePos, char prefix, const std::vector<ConstantValue*>& strings);
    ConstantValue* MakeConversionValue(const soul::ast::SourcePos& sourcePos, Type* type, ConstantValue* from);
    ConstantValue* MakeClsIdValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& clsIdStr);
    ConstantValue* MakeSymbolValue(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& symbol);
    ConstantValue* MakeNumericLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& strValue);
    ConstantValue* MakeAddressLiteral(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& id);
    Function* CurrentFunction() const;
    void SetCurrentFunction(Function* function);
    Function* AddFunctionDefinition(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId, bool once, MetadataRef* metadataRef);
    Function* AddFunctionDeclaration(const soul::ast::SourcePos& sourcePos, Type* type, const std::string& functionId);
    MetadataStruct* AddMetadataStruct(const soul::ast::SourcePos& sourcePos, int32_t id, Context* context);
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value);
    MetadataRef* CreateMetadataRef(const soul::ast::SourcePos& sourcePos, int32_t nodeId);
    void ResolveMetadataReferences();
    RegisterPool& GetRegisterPool() { return registerPool; }
    void ResetRegisterPool();
    soul::lexer::FileMap& GetFileMap() { return fileMap; }
private:
    CompileUnit compileUnit;
    Types types;
    Data data;
    Code code;
    Metadata metadata;
    RegisterPool registerPool;
    soul::lexer::FileMap fileMap;
};

} // cmajor::systemx::intermediate
