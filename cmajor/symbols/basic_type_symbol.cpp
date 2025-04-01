// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.basic.type.symbol;

import cmajor.ast.reader;
import cmajor.ast.writer;
import soul.ast.span;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.exception;
import cmajor.symbols.function.symbol;
import cmajor.symbols.value;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import util;
import std.core;

namespace cmajor::symbols {

BasicTypeSymbol::BasicTypeSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(symbolType_, span_, name_),
    defaultConstructor(nullptr), copyConstructor(nullptr), moveConstructor(nullptr), copyAssignment(nullptr), moveAssignment(nullptr), returnFun(nullptr), equalityOp(nullptr)
{
}

void BasicTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    util::uuid defaultConstructorId = util::nil_uuid();
    if (defaultConstructor)
    {
        defaultConstructorId = defaultConstructor->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(defaultConstructorId);
    util::uuid copyConstructorId = util::nil_uuid();
    if (copyConstructor)
    {
        copyConstructorId = copyConstructor->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(copyConstructorId);
    util::uuid moveConstructorId = util::nil_uuid();
    if (moveConstructor)
    {
        moveConstructorId = moveConstructor->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(moveConstructorId);
    util::uuid copyAssignmentId = util::nil_uuid();
    if (copyAssignment)
    {
        copyAssignmentId = copyAssignment->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(copyAssignmentId);
    util::uuid moveAssignmentId = util::nil_uuid();
    if (moveAssignment)
    {
        moveAssignmentId = moveAssignment->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(moveAssignmentId);
    util::uuid returnId = util::nil_uuid();
    if (returnFun)
    {
        returnId = returnFun->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(returnId);
    util::uuid equalityOpId = util::nil_uuid();
    if (equalityOp)
    {
        equalityOpId = equalityOp->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(equalityOpId);
}

void BasicTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    util::uuid defaultConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(defaultConstructorId);
    if (!defaultConstructorId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, defaultConstructorId, 0);
    }
    util::uuid copyConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(copyConstructorId);
    if (!copyConstructorId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyConstructorId, 1);
    }
    util::uuid moveConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(moveConstructorId);
    if (!moveConstructorId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveConstructorId, 2);
    }
    util::uuid copyAssignmentId;
    reader.GetBinaryStreamReader().ReadUuid(copyAssignmentId);
    if (!copyAssignmentId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyAssignmentId, 3);
    }
    util::uuid moveAssignmentId;
    reader.GetBinaryStreamReader().ReadUuid(moveAssignmentId);
    if (!moveAssignmentId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveAssignmentId, 4);
    }
    util::uuid returnId;
    reader.GetBinaryStreamReader().ReadUuid(returnId);
    if (!returnId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, returnId, 5);
    }
    util::uuid equalityOpId;
    reader.GetBinaryStreamReader().ReadUuid(equalityOpId);
    if (!equalityOpId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, equalityOpId, 6);
    }
}

void BasicTypeSymbol::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    switch (index)
    {
    case 0: defaultConstructor = functionSymbol; break;
    case 1: copyConstructor = functionSymbol; break;
    case 2: moveConstructor = functionSymbol; break;
    case 3: copyAssignment = functionSymbol; break;
    case 4: moveAssignment = functionSymbol; break;
    case 5: returnFun = functionSymbol; break;
    case 6: equalityOp = functionSymbol; break;
    default:
    {
        Assert(false, "invalid emplace function index");
    }
    }
}

void BasicTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject())
    {
        collector->AddBasicType(this);
    }
}

void BasicTypeSymbol::Dump(util::CodeFormatter& formatter, Context* context)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
}

void BasicTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!defaultConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no default constructor", GetFullSpan());
    }
    if (!copyConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no copy constructor", GetFullSpan());
    }
    if (!moveConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no move constructor", GetFullSpan());
    }
    if (!copyAssignment && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no copy assignment", GetFullSpan());
    }
    if (!moveAssignment && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no move assignment", GetFullSpan());
    }
    if (!returnFun && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no return function", GetFullSpan());
    }
    if (!equalityOp && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no equality comparison operation", GetFullSpan());
    }
}

AutoTypeSymbol::AutoTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    BasicTypeSymbol(SymbolType::autoTypeSymbol, span_, name_)
{
}

void* AutoTypeSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    throw Exception("'auto' type has no IR type", GetFullSpan());
}

void* AutoTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context)
{
    throw Exception("'auto' type: cannot create IR value", GetFullSpan());
}

BoolTypeSymbol::BoolTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    BasicTypeSymbol(SymbolType::boolTypeSymbol, span_, name_)
{
}

ValueType BoolTypeSymbol::GetValueType() const
{
    return ValueType::boolValue;
}

Value* BoolTypeSymbol::MakeValue() const
{
    return new BoolValue(GetSpan(), false);
}

void* BoolTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForBool();
}

SByteTypeSymbol::SByteTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::sbyteTypeSymbol, span_, name_)
{
}

ValueType SByteTypeSymbol::GetValueType() const
{
    return ValueType::sbyteValue;
}

Value* SByteTypeSymbol::MakeValue() const
{
    return new SByteValue(GetSpan(), 0);
}

void* SByteTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForSByte();
}

ByteTypeSymbol::ByteTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::byteTypeSymbol, span_, name_)
{
}

ValueType ByteTypeSymbol::GetValueType() const
{
    return ValueType::byteValue;
}

Value* ByteTypeSymbol::MakeValue() const
{
    return new ByteValue(GetSpan(), 0);
}

void* ByteTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForByte();
}

ShortTypeSymbol::ShortTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::shortTypeSymbol, span_, name_)
{
}

ValueType ShortTypeSymbol::GetValueType() const
{
    return ValueType::shortValue;
}

Value* ShortTypeSymbol::MakeValue() const
{
    return new ShortValue(GetSpan(), 0);
}

void* ShortTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForShort();
}

UShortTypeSymbol::UShortTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::ushortTypeSymbol, span_, name_)
{
}

ValueType UShortTypeSymbol::GetValueType() const
{
    return ValueType::ushortValue;
}

Value* UShortTypeSymbol::MakeValue() const
{
    return new UShortValue(GetSpan(), 0);
}

void* UShortTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForUShort();
}

IntTypeSymbol::IntTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::intTypeSymbol, span_, name_)
{
}

ValueType IntTypeSymbol::GetValueType() const
{
    return ValueType::intValue;
}

Value* IntTypeSymbol::MakeValue() const
{
    return new IntValue(GetSpan(), 0);
}

void* IntTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForInt();
}

UIntTypeSymbol::UIntTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::uintTypeSymbol, span_, name_)
{
}

ValueType UIntTypeSymbol::GetValueType() const
{
    return ValueType::uintValue;
}

Value* UIntTypeSymbol::MakeValue() const
{
    return new UIntValue(GetSpan(), 0);
}

void* UIntTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForUInt();
}

LongTypeSymbol::LongTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::longTypeSymbol, span_, name_)
{
}

ValueType LongTypeSymbol::GetValueType() const
{
    return ValueType::longValue;
}

Value* LongTypeSymbol::MakeValue() const
{
    return new LongValue(GetSpan(), 0);
}

void* LongTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForLong();
}

ULongTypeSymbol::ULongTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::ulongTypeSymbol, span_, name_)
{
}

ValueType ULongTypeSymbol::GetValueType() const
{
    return ValueType::ulongValue;
}

Value* ULongTypeSymbol::MakeValue() const
{
    return new ULongValue(GetSpan(), 0);
}

void* ULongTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForULong();
}

FloatTypeSymbol::FloatTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::floatTypeSymbol, span_, name_)
{
}

ValueType FloatTypeSymbol::GetValueType() const
{
    return ValueType::floatValue;
}

Value* FloatTypeSymbol::MakeValue() const
{
    return new FloatValue(GetSpan(), 0.0);
}

void* FloatTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForFloat();
}

DoubleTypeSymbol::DoubleTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::doubleTypeSymbol, span_, name_)
{
}

ValueType DoubleTypeSymbol::GetValueType() const
{
    return ValueType::doubleValue;
}

Value* DoubleTypeSymbol::MakeValue() const
{
    return new DoubleValue(GetSpan(), 0.0);
}

void* DoubleTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForDouble();
}

CharTypeSymbol::CharTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    BasicTypeSymbol(SymbolType::charTypeSymbol, span_, name_)
{
}

ValueType CharTypeSymbol::GetValueType() const
{
    return ValueType::charValue;
}

Value* CharTypeSymbol::MakeValue() const
{
    return new CharValue(GetSpan(), '\0');
}

void* CharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForChar();
}

WCharTypeSymbol::WCharTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    BasicTypeSymbol(SymbolType::wcharTypeSymbol, span_, name_)
{
}

ValueType WCharTypeSymbol::GetValueType() const
{
    return ValueType::wcharValue;
}

Value* WCharTypeSymbol::MakeValue() const
{
    return new WCharValue(GetSpan(), '\0');
}

void* WCharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForWChar();
}

UCharTypeSymbol::UCharTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    BasicTypeSymbol(SymbolType::ucharTypeSymbol, span_, name_)
{
}

ValueType UCharTypeSymbol::GetValueType() const
{
    return ValueType::ucharValue;
}

Value* UCharTypeSymbol::MakeValue() const
{
    return new UCharValue(GetSpan(), '\0');
}

void* UCharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForUChar();
}

VoidTypeSymbol::VoidTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::voidTypeSymbol, span_, name_)
{
}

void* VoidTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateDITypeForVoid();
}

} // namespace cmajor::symbols
