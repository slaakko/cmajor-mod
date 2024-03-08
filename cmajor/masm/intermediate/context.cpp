// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.context;

import cmajor.masm.intermediate.compile_unit;
import cmajor.masm.intermediate.error;
import cmajor.masm.intermediate.lexer;
import util;

namespace cmajor::masm::intermediate {

Context::Context() : compileUnit()
{
    compileUnit.SetContext(this);
    types.SetContext(this);
    data.SetContext(this);
    code.SetContext(this);
}

void Context::SetFilePath(const std::string& filePath_)
{
    compileUnit.SetFilePath(filePath_);
}

const std::string& Context::FilePath() const
{
    return compileUnit.FilePath();
}

std::string Context::ErrorLines(const soul::ast::LineColLen& lineColLen)
{
    if (lineColLen.IsValid())
    {
        std::string errorLines = util::ToUtf8(fileMap.GetFileLine(fileId, lineColLen.line));
        errorLines.append("\n").append(lineColLen.col - 1, ' ').append(lineColLen.len, '^').append("\n");
        return errorLines;
    }
    else
    {
        return std::string();
    }
}

void Context::AddStructureType(const soul::ast::Span& span, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs)
{
    types.AddStructureType(span, typeId, fieldTypeRefs);
}

void Context::AddArrayType(const soul::ast::Span& span, int32_t typeId, int64_t size, const TypeRef& elementTypeRef)
{
    types.AddArrayType(span, typeId, size, elementTypeRef);
}

void Context::AddFunctionType(const soul::ast::Span& span, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs)
{
    types.AddFunctionType(span, typeId, returnTypeRef, paramTypeRefs);
}

void Context::AddGlobalVariable(const soul::ast::Span& span, Type* type, const std::string& variableName, Value* initializer)
{
    data.AddGlobalVariable(span, type, variableName, initializer, this);
}

void Context::ResolveTypes()
{
    types.Resolve(this);
}

void Context::ResolveType(TypeRef& typeRef)
{
    types.ResolveType(typeRef, this);
}

Value* Context::GetTrueValue()
{
    return data.GetTrueValue(types);
}

Value* Context::GetFalseValue()
{
    return data.GetFalseValue(types);
}

Value* Context::GetBooleanLiteral(const soul::ast::Span& span, Type* type, bool value)
{
    if (type->IsBooleanType())
    {
        if (value)
        {
            return GetTrueValue();
        }
        else
        {
            return GetFalseValue();
        }
    }
    else
    {
        Error("error making literal value: Boolean type expected", span, this);
    }
    return nullptr;
}

Value* Context::GetSByteValue(int8_t value)
{
    return data.GetSByteValue(value, types);
}

Value* Context::GetByteValue(uint8_t value)
{
    return data.GetByteValue(value, types);
}

Value* Context::GetShortValue(int16_t value)
{
    return data.GetShortValue(value, types);
}

Value* Context::GetUShortValue(uint16_t value)
{
    return data.GetUShortValue(value, types);
}

Value* Context::GetIntValue(int32_t value)
{
    return data.GetIntValue(value, types);
}

Value* Context::GetUIntValue(uint32_t value)
{
    return data.GetUIntValue(value, types);
}

Value* Context::GetLongValue(int64_t value)
{
    return data.GetLongValue(value, types);
}

Value* Context::GetULongValue(uint64_t value)
{
    return data.GetULongValue(value, types);
}

Value* Context::GetFloatValue(float value)
{
    return data.GetFloatValue(value, types);
}

Value* Context::GetDoubleValue(double value)
{
    return data.GetDoubleValue(value, types);
}

Value* Context::GetNullValue(const soul::ast::Span& span, Type* type)
{
    if (type->IsPointerType())
    {
        return data.GetNullValue(type);
    }
    else
    {
        Error("error making null literal value: pointer type expected", span, this);
    }
    return nullptr;
}

Value* Context::MakeArrayValue(const soul::ast::Span& span, const std::vector<Value*>& elements)
{
    return data.MakeArrayValue(span, elements);
}

Value* Context::MakeStructureValue(const soul::ast::Span& span, const std::vector<Value*>& fieldValues)
{
    return data.MakeStructureValue(span, fieldValues);
}

Value* Context::MakeStringValue(const soul::ast::Span& span, const std::string& value)
{
    return data.MakeStringValue(span, value);
}

Value* Context::MakeStringArrayValue(const soul::ast::Span& span, char prefix, const std::vector<Value*>& strings)
{
    return data.MakeStringArrayValue(span, prefix, strings);
}

Value* Context::MakeConversionValue(const soul::ast::Span& span, Type* type, Value* from)
{
    return data.MakeConversionValue(span, type, from);
}

Value* Context::MakeClsIdValue(const soul::ast::Span& span, Type* type, const std::string& clsIdStr)
{
    return data.MakeClsIdValue(span, type, clsIdStr);
}

Value* Context::MakeSymbolValue(const soul::ast::Span& span, Type* type, const std::string& symbol)
{
    return data.MakeSymbolValue(span, type, symbol);
}

Value* Context::MakeNumericLiteral(const soul::ast::Span& span, Type* type, const std::string& strValue)
{
    return data.MakeNumericLiteral(span, type, strValue, types, this);
}

Value* Context::MakeAddressLiteral(const soul::ast::Span& span, Type* type, const std::string& id)
{
    return data.MakeAddressLiteral(span, type, id, this);
}

Function* Context::CurrentFunction() const
{
    return code.CurrentFunction();
}

void Context::ValidateData()
{
}

void Context::SetCurrentFunction(Function* function)
{
    code.SetCurrentFunction(function);
}

Function* Context::AddFunctionDefinition(const soul::ast::Span& span, Type* type, const std::string& functionId)
{
    if (type->IsPointerType())
    {
        type = type->RemovePointer(span, this);
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        return code.AddFunctionDefinition(span, functionType, functionId, this);
    }
    else
    {
        Error("error adding function '" + functionId + "' definition: invalid type '" + type->Name() + "': function type expected", span, this);
    }
    return nullptr;
}

Function* Context::AddFunctionDeclaration(const soul::ast::Span& span, Type* type, const std::string& functionId)
{
    if (type->IsPointerType())
    {
        type = type->RemovePointer(span, this);
    }
    if (type->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(type);
        return code.AddFunctionDeclaration(span, functionType, functionId);
    }
    else
    {
        Error("error adding function '" + functionId + "' declaration: invalid type '" + type->Name() + "': function type expected", span, this);
    }
    return nullptr;
}

} // cmajor::masm::intermediate
