// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.context;

import cmajor.masm.assembly;
import cmajor.masm.intermediate.compile_unit;
import cmajor.masm.intermediate.types;
import cmajor.masm.intermediate.data;
import cmajor.masm.intermediate.code;
import cmajor.masm.intermediate.metadata;
import soul.lexer;
import soul.ast.span;
import std.core;

export namespace cmajor::masm::intermediate {

namespace context { }

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
    void SetFileId(int32_t fileId_) { fileId = fileId_; }
    int32_t FileId() const { return fileId; }
    std::string ErrorLines(const soul::ast::LineColLen& lineColLen);
    void AddStructureType(const soul::ast::Span& span, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs);
    void AddArrayType(const soul::ast::Span& span, int32_t typeId, int64_t size, const TypeRef& elementTypeRef);
    void AddFunctionType(const soul::ast::Span& span, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs);
    void AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer);
    void ResolveTypes();
    void ValidateData();
    void ResolveType(TypeRef& typeRef);
    Value* GetTrueValue();
    Value* GetFalseValue();
    Value* GetBooleanLiteral(const soul::ast::Span& span, Type* type, bool value);
    Value* GetSByteValue(int8_t value);
    Value* GetByteValue(uint8_t value);
    Value* GetShortValue(int16_t value);
    Value* GetUShortValue(uint16_t value);
    Value* GetIntValue(int32_t value);
    Value* GetUIntValue(uint32_t value);
    Value* GetLongValue(int64_t value);
    Value* GetULongValue(uint64_t value);
    Value* GetFloatValue(float value);
    Value* GetDoubleValue(double value);
    Value* GetNullValue(const soul::ast::Span& span, Type* type);
    Value* MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements);
    Value* MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues);
    Value* MakeStringValue(const soul::ast::Span& span, const std::string& value);
    Value* MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& strings);
    Value* MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from);
    Value* MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr);
    Value* MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol);
    Value* MakeIntegerLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue);
    Value* MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id);
    Function* CurrentFunction() const;
    void SetCurrentFunction(Function* function);
    Function* AddFunctionDefinition(const soul::ast::Span& span, Type* type, const std::string& functionId, bool inline_, cmajor::masm::intermediate::MetadataRef* metadataRef);
    Function* AddFunctionDeclaration(const soul::ast::Span& span, Type* type, const std::string& functionId);
    MetadataStruct* AddMetadataStruct(const soul::ast::Span& span, int32_t id);
    MetadataBool* CreateMetadataBool(bool value);
    MetadataLong* CreateMetadataLong(int64_t value);
    MetadataString* CreateMetadataString(const std::string& value, bool crop);
    MetadataRef* CreateMetadataRef(const soul::ast::Span& span, int32_t nodeId);
    void ResolveMetadataReferences();
    soul::lexer::FileMap& GetFileMap() { return fileMap; }
    cmajor::masm::assembly::Context* AssemblyContext() { return &assemblyContext; }
    void Write(const std::string& intermediateFilePath);
    void SetInlineDepth(int inlineDepth_) { inlineDepth = inlineDepth_; }
    int InlineDepth() const { return inlineDepth; }
private:
    CompileUnit compileUnit;
    Types types;
    Data data;
    Code code;
    Metadata metadata;
    soul::lexer::FileMap fileMap;
    int32_t fileId;
    cmajor::masm::assembly::Context assemblyContext;
    int inlineDepth;
    std::unique_ptr<cmajor::masm::intermediate::MetadataRef> metadataRef;
};

} // cmajor::masm::intermediate
