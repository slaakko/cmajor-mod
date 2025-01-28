// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.basic.type.operation;

import soul.ast.span;
import util;
import std.core;

namespace cmajor ::symbols {

BasicTypeUnaryPlusOperation::BasicTypeUnaryPlusOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeUnaryOperation<BasicTypeUnaryPlus>(SymbolType::basicTypeUnaryPlus, span_)
{
}

BasicTypeIntUnaryMinusOperation::BasicTypeIntUnaryMinusOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeUnaryOperation<BasicTypeIntUnaryMinus>(SymbolType::basicTypeIntUnaryMinus, span_)
{
}

BasicTypeFloatUnaryMinusOperation::BasicTypeFloatUnaryMinusOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeUnaryOperation<BasicTypeFloatUnaryMinus>(SymbolType::basicTypeFloatUnaryMinus, span_)
{
}

BasicTypeComplementOperation::BasicTypeComplementOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeUnaryOperation<BasicTypeComplement>(SymbolType::basicTypeComplement, span_)
{
}

BasicTypeNotOperation::BasicTypeNotOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeUnaryOperation<BasicTypeNot>(SymbolType::basicTypeNot, span_)
{
}

BasicTypeAddOperation::BasicTypeAddOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeAdd>(SymbolType::basicTypeAdd, span_)
{
}

BasicTypeFAddOperation::BasicTypeFAddOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeFAdd>(SymbolType::basicTypeFAdd, span_)
{
}

BasicTypeSubOperation::BasicTypeSubOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeSub>(SymbolType::basicTypeSub, span_)
{
}

BasicTypeFSubOperation::BasicTypeFSubOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeFSub>(SymbolType::basicTypeFSub, span_)
{
}

BasicTypeMulOperation::BasicTypeMulOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeMul>(SymbolType::basicTypeMul, span_)
{
}

BasicTypeFMulOperation::BasicTypeFMulOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeFMul>(SymbolType::basicTypeFMul, span_)
{
}

BasicTypeSDivOperation::BasicTypeSDivOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeSDiv>(SymbolType::basicTypeSDiv, span_)
{
}

BasicTypeUDivOperation::BasicTypeUDivOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeUDiv>(SymbolType::basicTypeUDiv, span_)
{
}

BasicTypeFDivOperation::BasicTypeFDivOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeFDiv>(SymbolType::basicTypeFDiv, span_)
{
}

BasicTypeSRemOperation::BasicTypeSRemOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeSRem>(SymbolType::basicTypeSRem, span_)
{
}

BasicTypeURemOperation::BasicTypeURemOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeURem>(SymbolType::basicTypeURem, span_)
{
}

BasicTypeAndOperation::BasicTypeAndOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeAnd>(SymbolType::basicTypeAnd, span_)
{
}

BasicTypeOrOperation::BasicTypeOrOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeOr>(SymbolType::basicTypeOr, span_)
{
}

BasicTypeXorOperation::BasicTypeXorOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeXor>(SymbolType::basicTypeXor, span_)
{
}

BasicTypeShlOperation::BasicTypeShlOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeShl>(SymbolType::basicTypeShl, span_)
{
}

BasicTypeAShrOperation::BasicTypeAShrOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeAShr>(SymbolType::basicTypeAShr, span_)
{
}

BasicTypeLShrOperation::BasicTypeLShrOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeBinaryOperation<BasicTypeLShr>(SymbolType::basicTypeLShr, span_)
{
}

BasicTypeDefaultInt1Operation::BasicTypeDefaultInt1Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultInt1>(SymbolType::defaultInt1, span_)
{
}

BasicTypeDefaultSInt8Operation::BasicTypeDefaultSInt8Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultSInt8>(SymbolType::defaultSInt8, span_)
{
}

BasicTypeDefaultUInt8Operation::BasicTypeDefaultUInt8Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultUInt8>(SymbolType::defaultUInt8, span_)
{
}

BasicTypeDefaultSInt16Operation::BasicTypeDefaultSInt16Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultSInt16>(SymbolType::defaultSInt16, span_)
{
}

BasicTypeDefaultUInt16Operation::BasicTypeDefaultUInt16Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultUInt16>(SymbolType::defaultUInt16, span_)
{
}

BasicTypeDefaultSInt32Operation::BasicTypeDefaultSInt32Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultSInt32>(SymbolType::defaultSInt32, span_)
{
}

BasicTypeDefaultUInt32Operation::BasicTypeDefaultUInt32Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultUInt32>(SymbolType::defaultUInt32, span_)
{
}

BasicTypeDefaultSInt64Operation::BasicTypeDefaultSInt64Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultSInt64>(SymbolType::defaultSInt64, span_)
{
}

BasicTypeDefaultUInt64Operation::BasicTypeDefaultUInt64Operation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultUInt64>(SymbolType::defaultUInt64, span_)
{
}

BasicTypeDefaultFloatOperation::BasicTypeDefaultFloatOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultFloat>(SymbolType::defaultFloat, span_)
{
}

BasicTypeDefaultDoubleOperation::BasicTypeDefaultDoubleOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultDouble>(SymbolType::defaultDouble, span_)
{
}

BasicTypeDefaultCharOperation::BasicTypeDefaultCharOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultChar>(SymbolType::defaultChar, span_)
{
}

BasicTypeDefaultWCharOperation::BasicTypeDefaultWCharOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultWChar>(SymbolType::defaultWChar, span_)
{
}

BasicTypeDefaultUCharOperation::BasicTypeDefaultUCharOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeDefaultCtor<DefaultUChar>(SymbolType::defaultUChar, span_)
{
}

BasicTypeCopyCtor::BasicTypeCopyCtor(const soul::ast::Span& span_, TypeSymbol* type) : FunctionSymbol(SymbolType::basicTypeCopyCtor, span_, U"@constructor")
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    ComputeName();
    if (type->IsBasicTypeSymbol())
    {
        BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(type);
        basicTypeSymbol->SetCopyConstructor(this);
    }
}

BasicTypeCopyCtor::BasicTypeCopyCtor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::basicTypeCopyCtor, span_, name_)
{
}

void BasicTypeCopyCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
    Assert(genObjects.size() == 2, "copy constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
}

std::unique_ptr<Value> BasicTypeCopyCtor::ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::Span& span, Value* receiver) const
{
    Assert(argumentValues.size() == 1, "one source value expected");
    return std::unique_ptr<Value>(argumentValues[0]->Clone());
}

BasicTypeMoveCtor::BasicTypeMoveCtor(const soul::ast::Span& span_, TypeSymbol* type_) : FunctionSymbol(SymbolType::basicTypeMoveCtor, span_, U"@constructor"), type(type_)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(span_, U"that");
    thatParam->SetType(type->AddRvalueReference());
    AddMember(thatParam);
    ComputeName();
    if (type->IsBasicTypeSymbol())
    {
        BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(type);
        basicTypeSymbol->SetMoveConstructor(this);
    }
}

BasicTypeMoveCtor::BasicTypeMoveCtor(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::basicTypeMoveCtor, span_, name_), type(nullptr)
{
}

void BasicTypeMoveCtor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void BasicTypeMoveCtor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void BasicTypeMoveCtor::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        type = typeSymbol;
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void BasicTypeMoveCtor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
    Assert(genObjects.size() == 2, "move constructor needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
}

std::unique_ptr<Value> BasicTypeMoveCtor::ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::Span& span, Value* receiver) const
{
    Assert(argumentValues.size() == 1, "one source value expected");
    return std::unique_ptr<Value>(argumentValues[0]->Clone());
}

BasicTypeCopyAssignment::BasicTypeCopyAssignment(const soul::ast::Span& span_, TypeSymbol* type, TypeSymbol* voidType) : 
    FunctionSymbol(SymbolType::basicTypeCopyAssignment, span_, U"operator=")
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(span_, U"that");
    thatParam->SetType(type);
    AddMember(thatParam);
    SetReturnType(voidType);
    ComputeName();
    if (type->IsBasicTypeSymbol())
    {
        BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(type);
        basicTypeSymbol->SetCopyAssignment(this);
    }
}

BasicTypeCopyAssignment::BasicTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::basicTypeCopyAssignment, span_, name_)
{
}

void BasicTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
    Assert(genObjects.size() == 2, "copy assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

BasicTypeMoveAssignment::BasicTypeMoveAssignment(const soul::ast::Span& span_, TypeSymbol* type_, TypeSymbol* voidType) :
    FunctionSymbol(SymbolType::basicTypeMoveAssignment, span_, U"operator="), type(type_)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(span_, U"this");
    thisParam->SetType(type->AddPointer());
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(span_, U"that");
    thatParam->SetType(type->AddRvalueReference());
    AddMember(thatParam);
    SetReturnType(voidType);
    ComputeName();
    if (type->IsBasicTypeSymbol())
    {
        BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(type);
        basicTypeSymbol->SetMoveAssignment(this);
    }
}

BasicTypeMoveAssignment::BasicTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::basicTypeMoveAssignment, span_, name_), type(nullptr)
{
}

void BasicTypeMoveAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void BasicTypeMoveAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 1);
}

void BasicTypeMoveAssignment::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        type = typeSymbol;
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void BasicTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
    Assert(genObjects.size() == 2, "move assignment needs two objects");
    genObjects[1]->Load(emitter, cmajor::ir::OperationFlags::none);
    void* rvalueRefValue = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateLoad(type->IrType(emitter), rvalueRefValue)); // TODO
    genObjects[0]->Store(emitter, cmajor::ir::OperationFlags::none);
}

BasicTypeReturn::BasicTypeReturn(const soul::ast::Span& span_, TypeSymbol* type) : FunctionSymbol(SymbolType::basicTypeReturn, span_, U"@return")
{
    SetGroupName(U"@return");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* valueParam = new ParameterSymbol(span_, U"value");
    valueParam->SetType(type);
    AddMember(valueParam);
    SetReturnType(type);
    ComputeName();
    if (type->IsBasicTypeSymbol())
    {
        BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(type);
        basicTypeSymbol->SetReturnFun(this);
    }
}

BasicTypeReturn::BasicTypeReturn(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::basicTypeReturn, span_, name_)
{
}

void BasicTypeReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags)
{
    Assert(genObjects.size() == 1, "return needs one object");
    genObjects[0]->Load(emitter, cmajor::ir::OperationFlags::none);
}

BasicTypeIntegerEqualityOperation::BasicTypeIntegerEqualityOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, span_)
{
}

BasicTypeFloatingEqualityOperation::BasicTypeFloatingEqualityOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeComparisonOperation<BasicTypeFloatingEquality>(SymbolType::basicTypeFloatingEquality, span_)
{
}

BasicTypeUnsignedIntegerLessThanOperation::BasicTypeUnsignedIntegerLessThanOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, span_)
{
}

BasicTypeSignedIntegerLessThanOperation::BasicTypeSignedIntegerLessThanOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeComparisonOperation<BasicTypeSignedIntegerLessThan>(SymbolType::basicTypeSignedIntegerLessThan, span_)
{
}

BasicTypeFloatingLessThanOperation::BasicTypeFloatingLessThanOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeComparisonOperation<BasicTypeFloatingLessThan>(SymbolType::basicTypeFloatingLessThan, span_)
{
}

BasicTypeImplicitSignExtensionOperation::BasicTypeImplicitSignExtensionOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, span_)
{
}

BasicTypeImplicitZeroExtensionOperation::BasicTypeImplicitZeroExtensionOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, span_)
{
}

BasicTypeExplicitSignExtensionOperation::BasicTypeExplicitSignExtensionOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, span_)
{
}

BasicTypeExplicitZeroExtensionOperation::BasicTypeExplicitZeroExtensionOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, span_)
{
}

BasicTypeTruncationOperation::BasicTypeTruncationOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, span_)
{
}

BasicTypeBitCastOperation::BasicTypeBitCastOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, span_)
{
}

BasicTypeImplicitUnsignedIntToFloatingOperation::BasicTypeImplicitUnsignedIntToFloatingOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, span_)
{
}

BasicTypeImplicitSignedIntToFloatingOperation::BasicTypeImplicitSignedIntToFloatingOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, span_)
{
}

BasicTypeExplicitUnsignedIntToFloatingOperation::BasicTypeExplicitUnsignedIntToFloatingOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, span_)
{
}

BasicTypeExplicitSignedIntToFloatingOperation::BasicTypeExplicitSignedIntToFloatingOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeExplicitSignedIntToFloating, span_)
{
}

BasicTypeFloatingToUnsignedIntOperation::BasicTypeFloatingToUnsignedIntOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, span_)
{
}

BasicTypeFloatingToSignedIntOperation::BasicTypeFloatingToSignedIntOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, span_)
{
}

BasicTypeFloatingExtensionOperation::BasicTypeFloatingExtensionOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeImplicitConversion<BasicTypeFloatingExtension>(SymbolType::basicTypeFloatingExtension, span_)
{
}

BasicTypeFloatingTruncationOperation::BasicTypeFloatingTruncationOperation(const soul::ast::Span& span_, const std::u32string& name_) :
    BasicTypeExplicitConversion<BasicTypeFloatingTruncation>(SymbolType::basicTypeFloatingTruncation, span_)
{
}

void MakeSignedIntegerTypeOperations(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, TypeSymbol* type)
{
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeUnaryPlus>(SymbolType::basicTypeUnaryPlus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeIntUnaryMinus>(SymbolType::basicTypeIntUnaryMinus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeComplement>(SymbolType::basicTypeComplement, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeAdd>(SymbolType::basicTypeAdd, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeSub>(SymbolType::basicTypeSub, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeMul>(SymbolType::basicTypeMul, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeSDiv>(SymbolType::basicTypeSDiv, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeSRem>(SymbolType::basicTypeSRem, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeAnd>(SymbolType::basicTypeAnd, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeOr>(SymbolType::basicTypeOr, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeXor>(SymbolType::basicTypeXor, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeShl>(SymbolType::basicTypeShl, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeAShr>(SymbolType::basicTypeAShr, rootSpan, type));
}

void MakeUnsignedIntegerTypeOperations(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, TypeSymbol* type)
{
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeUnaryPlus>(SymbolType::basicTypeUnaryPlus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeIntUnaryMinus>(SymbolType::basicTypeIntUnaryMinus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeComplement>(SymbolType::basicTypeComplement, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeAdd>(SymbolType::basicTypeAdd, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeSub>(SymbolType::basicTypeSub, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeMul>(SymbolType::basicTypeMul, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeUDiv>(SymbolType::basicTypeUDiv, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeURem>(SymbolType::basicTypeURem, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeAnd>(SymbolType::basicTypeAnd, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeOr>(SymbolType::basicTypeOr, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeXor>(SymbolType::basicTypeXor, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeShl>(SymbolType::basicTypeShl, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeLShr>(SymbolType::basicTypeLShr, rootSpan, type));
}

void MakeFloatingPointTypeOperations(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, TypeSymbol* type)
{
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeUnaryPlus>(SymbolType::basicTypeUnaryPlus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeFloatUnaryMinus>(SymbolType::basicTypeFloatUnaryMinus, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeFAdd>(SymbolType::basicTypeFAdd, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeFSub>(SymbolType::basicTypeFSub, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeFMul>(SymbolType::basicTypeFMul, rootSpan, type));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeBinaryOperation<BasicTypeFDiv>(SymbolType::basicTypeFDiv, rootSpan, type));
}

void MakeBasicTypeConversions(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, BoolTypeSymbol* boolType, SByteTypeSymbol* sbyteType, 
    ByteTypeSymbol* byteType, ShortTypeSymbol* shortType, UShortTypeSymbol* ushortType,
    IntTypeSymbol* intType, UIntTypeSymbol* uintType, LongTypeSymbol* longType, ULongTypeSymbol* ulongType,
    FloatTypeSymbol* floatType, DoubleTypeSymbol* doubleType,
    CharTypeSymbol* charType, WCharTypeSymbol* wcharType, UCharTypeSymbol* ucharType)
{
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2sbyte", boolType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2byte", boolType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2short", boolType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2ushort", boolType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2int", boolType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2uint", boolType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2long", boolType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2ulong", boolType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"bool2float", boolType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"bool2double", boolType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2char", boolType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2wchar", boolType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"bool2uchar", boolType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"sbyte2bool", sbyteType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"sbyte2byte", sbyteType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"sbyte2short", 1, sbyteType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"sbyte2ushort", sbyteType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"sbyte2int", 2, sbyteType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"sbyte2uint", sbyteType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"sbyte2long", 3, sbyteType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"sbyte2ulong", sbyteType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"sbyte2float", 4, sbyteType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"sbyte2double", 5, sbyteType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"sbyte2char", sbyteType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"sbyte2wchar", sbyteType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"sbyte2uchar", sbyteType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"byte2bool", byteType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"byte2sbyte", byteType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2short", 1, byteType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2ushort", 2, byteType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2int", 3, byteType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2uint", 4, byteType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2long", 5, byteType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"byte2long", 6, byteType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"byte2float", 7, byteType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"byte2double", 8, byteType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"byte2char", byteType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"byte2wchar", byteType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"byte2uchar", byteType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"short2bool", shortType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"short2sbyte", shortType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"short2byte", shortType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"short2ushort", shortType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"short2int", 1, shortType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"short2uint", shortType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"short2long", 2, shortType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"short2ulong", shortType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"short2float", 3, shortType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"short2double", 4, shortType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"short2char", shortType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"short2wchar", shortType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"short2uchar", shortType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ushort2bool", ushortType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ushort2sbyte", ushortType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ushort2byte", ushortType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"ushort2short", ushortType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"ushort2int", 1, ushortType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"ushort2uint", 2, ushortType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"ushort2long", 3, ushortType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"ushort2ulong", 4, ushortType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"ushort2float", 5, ushortType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"ushort2double", 6, ushortType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ushort2char", ushortType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"ushort2wchar", ushortType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"ushort2uchar", ushortType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2bool", intType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2sbyte", intType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2byte", intType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2short", intType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2ushort", intType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"int2uint", intType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeImplicitSignExtension, rootSpan, U"int2long", 1, intType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeSignExtension>(SymbolType::basicTypeExplicitSignExtension, rootSpan, U"int2ulong", intType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"int2float", 2, intType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"int2double", 3, intType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2char", intType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"int2wchar", intType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"int2uchar", intType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2bool", uintType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2sbyte", uintType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2byte", uintType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2short", uintType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2ushort", uintType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"uint2int", uintType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"uint2long", 1, uintType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"uint2ulong", 2, uintType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"uint2float", 3, uintType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"uint2double", 4, uintType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2char", uintType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uint2wchar", uintType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"uint2uchar", uintType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2bool", longType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"longsbyte", longType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2byte", longType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2short", longType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2ushort", longType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2int", longType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2uint", longType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"long2ulong", longType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"long2float", 1, longType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeSignedIntToFloating>(SymbolType::basicTypeImplicitSignedIntToFloating, rootSpan, U"long2double", 2, longType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2char", longType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2wchar", longType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"long2uchar", longType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2bool", ulongType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2sbyte", ulongType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2byte", ulongType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2short", ulongType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2ushort", ulongType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2int", ulongType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2uint", ulongType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"ulong2long", ulongType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"ulong2float", 1, ulongType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeImplicitUnsignedIntToFloating, rootSpan, U"ulong2double", 2, ulongType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2char", ulongType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2wchar", ulongType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"ulong2uchar", ulongType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2bool", floatType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"float2sbyte", floatType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2byte", floatType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"float2short", floatType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2ushort", floatType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"float2int", floatType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2uint", floatType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"float2long", floatType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2ulong", floatType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeFloatingExtension>(SymbolType::basicTypeFloatingExtension, rootSpan, U"float2double", 1, floatType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2char", floatType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2wchar", floatType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"float2uchar", floatType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2bool", doubleType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"double2sbyte", doubleType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2byte", doubleType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"double2short", doubleType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2ushort", doubleType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"double2int", doubleType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2uint", doubleType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToSignedInt>(SymbolType::basicTypeFloatingToSignedInt, rootSpan, U"double2long", doubleType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2ulong", doubleType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingTruncation>(SymbolType::basicTypeFloatingTruncation, rootSpan, U"double2float", doubleType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2char", doubleType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2wchar", doubleType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeFloatingToUnsignedInt>(SymbolType::basicTypeFloatingToUnsignedInt, rootSpan, U"double2uchar", doubleType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"char2bool", charType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"char2sbyte", charType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"char2byte", charType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2short", charType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2ushort", charType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2int", charType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2uint", charType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2long", charType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"char2ulong", charType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"char2float", charType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"char2double", charType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"char2wchar", 1, charType, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"char2uchar", 2, charType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"wchar2bool", wcharType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"wchar2sbyte", wcharType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"wchar2byte", wcharType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"wchar2short", wcharType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"wchar2ushort", wcharType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"wchar2int", wcharType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"wchar2uint", wcharType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"wchar2long", wcharType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"wchar2ulong", wcharType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"wchar2float", wcharType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"wchar2double", wcharType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"wchar2char", wcharType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeImplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeImplicitZeroExtension, rootSpan, U"wchar2uchar", 1, wcharType, ucharType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2bool", ucharType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2sbyte", ucharType, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2byte", ucharType, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2short", ucharType, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2ushort", ucharType, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"uchar2int", ucharType, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeBitCast>(SymbolType::basicTypeBitCast, rootSpan, U"uchar2uint", ucharType, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"uchar2long", ucharType, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeZeroExtension>(SymbolType::basicTypeExplicitZeroExtension, rootSpan, U"uchar2ulong", ucharType, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"uchar2float", ucharType, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeUnsignedIntToFloating>(SymbolType::basicTypeExplicitUnsignedIntToFloating, rootSpan, U"uchar2double", ucharType, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2char", ucharType, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeExplicitConversion<BasicTypeTruncation>(SymbolType::basicTypeTruncation, rootSpan, U"uchar2wchar", ucharType, wcharType));
}

void MakeBasicTypeOperations(SymbolTable& symbolTable, const soul::ast::Span& rootSpan,
    BoolTypeSymbol* boolType, SByteTypeSymbol* sbyteType, ByteTypeSymbol* byteType, ShortTypeSymbol* shortType, UShortTypeSymbol* ushortType, IntTypeSymbol* intType, UIntTypeSymbol* uintType,
    LongTypeSymbol* longType, ULongTypeSymbol* ulongType, FloatTypeSymbol* floatType, DoubleTypeSymbol* doubleType, CharTypeSymbol* charType, WCharTypeSymbol* wcharType, UCharTypeSymbol* ucharType,
    VoidTypeSymbol* voidType)
{
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultInt1>(SymbolType::defaultInt1, rootSpan, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, boolType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, boolType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, boolType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeUnaryOperation<BasicTypeNot>(SymbolType::basicTypeNot, rootSpan, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultSInt8>(SymbolType::defaultSInt8, rootSpan, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, sbyteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, sbyteType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, sbyteType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, sbyteType));
    MakeSignedIntegerTypeOperations(symbolTable, rootSpan, sbyteType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, sbyteType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeSignedIntegerLessThan>(SymbolType::basicTypeSignedIntegerLessThan, rootSpan, sbyteType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultUInt8>(SymbolType::defaultUInt8, rootSpan, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, byteType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, byteType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, byteType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, byteType));
    MakeUnsignedIntegerTypeOperations(symbolTable, rootSpan, byteType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, byteType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, byteType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultSInt16>(SymbolType::defaultSInt16, rootSpan, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, shortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, shortType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, shortType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, shortType));
    MakeSignedIntegerTypeOperations(symbolTable, rootSpan, shortType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, shortType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeSignedIntegerLessThan>(SymbolType::basicTypeSignedIntegerLessThan, rootSpan, shortType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultUInt16>(SymbolType::defaultUInt16, rootSpan, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, ushortType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, ushortType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, ushortType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, ushortType));
    MakeUnsignedIntegerTypeOperations(symbolTable, rootSpan, ushortType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, ushortType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, ushortType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultSInt32>(SymbolType::defaultSInt32, rootSpan, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, intType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, intType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, intType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, intType));
    MakeSignedIntegerTypeOperations(symbolTable, rootSpan, intType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, intType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeSignedIntegerLessThan>(SymbolType::basicTypeSignedIntegerLessThan, rootSpan, intType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultUInt32>(SymbolType::defaultUInt32, rootSpan, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, uintType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, uintType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, uintType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, uintType));
    MakeUnsignedIntegerTypeOperations(symbolTable, rootSpan, uintType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, uintType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, uintType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultSInt64>(SymbolType::defaultSInt64, rootSpan, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, longType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, longType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, longType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, longType));
    MakeSignedIntegerTypeOperations(symbolTable, rootSpan, longType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, longType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeSignedIntegerLessThan>(SymbolType::basicTypeSignedIntegerLessThan, rootSpan, longType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultUInt64>(SymbolType::defaultUInt64, rootSpan, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, ulongType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, ulongType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, ulongType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, ulongType));
    MakeUnsignedIntegerTypeOperations(symbolTable, rootSpan, ulongType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, ulongType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, ulongType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultFloat>(SymbolType::defaultFloat, rootSpan, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, floatType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, floatType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, floatType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, floatType));
    MakeFloatingPointTypeOperations(symbolTable, rootSpan, floatType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeFloatingEquality>(SymbolType::basicTypeFloatingEquality, rootSpan, floatType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeFloatingLessThan>(SymbolType::basicTypeFloatingLessThan, rootSpan, floatType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultDouble>(SymbolType::defaultDouble, rootSpan, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, doubleType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, doubleType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, doubleType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, doubleType));
    MakeFloatingPointTypeOperations(symbolTable, rootSpan, doubleType);
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeFloatingEquality>(SymbolType::basicTypeFloatingEquality, rootSpan, doubleType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeFloatingLessThan>(SymbolType::basicTypeFloatingLessThan, rootSpan, doubleType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultChar>(SymbolType::defaultChar, rootSpan, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, charType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, charType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, charType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, charType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, charType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultWChar>(SymbolType::defaultWChar, rootSpan, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, wcharType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, wcharType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, wcharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, wcharType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, wcharType, boolType));

    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeDefaultCtor<DefaultUChar>(SymbolType::defaultUChar, rootSpan, ucharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyCtor(rootSpan, ucharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveCtor(rootSpan, ucharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeCopyAssignment(rootSpan, ucharType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeMoveAssignment(rootSpan, ucharType, voidType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeReturn(rootSpan, ucharType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeIntegerEquality>(SymbolType::basicTypeIntegerEquality, rootSpan, ucharType, boolType));
    symbolTable.AddFunctionSymbolToGlobalScope(new BasicTypeComparisonOperation<BasicTypeUnsignedIntegerLessThan>(SymbolType::basicTypeUnsignedIntegerLessThan, rootSpan, ucharType, boolType));

    MakeBasicTypeConversions(symbolTable, rootSpan, boolType, sbyteType, byteType, shortType, ushortType, intType, uintType, longType, ulongType, floatType, doubleType, charType, wcharType, ucharType);
}
} // namespace cmajor::symbols
