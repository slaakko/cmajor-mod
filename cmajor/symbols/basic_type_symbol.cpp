// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.basic.type.symbol;

import cmajor.ast.reader;
import cmajor.ast.writer;
import soul.ast.source.pos;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.exception;
import cmajor.symbols.function.symbol;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import util;
import std.core;

namespace cmajor::symbols {

BasicTypeSymbol::BasicTypeSymbol(SymbolType symbolType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    TypeSymbol(symbolType_, sourcePos_, sourceModuleId_, name_),
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
        //TODO:Assert(false, "invalid emplace function index");
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

void BasicTypeSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
}

void BasicTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!defaultConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no default constructor", GetSourcePos(), SourceModuleId());
    }
    if (!copyConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no copy constructor", GetSourcePos(), SourceModuleId());
    }
    if (!moveConstructor && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no move constructor", GetSourcePos(), SourceModuleId());
    }
    if (!copyAssignment && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no copy assignment", GetSourcePos(), SourceModuleId());
    }
    if (!moveAssignment && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no move assignment", GetSourcePos(), SourceModuleId());
    }
    if (!returnFun && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no return function", GetSourcePos(), SourceModuleId());
    }
    if (!equalityOp && !IsVoidType())
    {
        throw SymbolCheckException("basic type symbol has no equality comparison operation", GetSourcePos(), SourceModuleId());
    }
}

BoolTypeSymbol::BoolTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::boolTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType BoolTypeSymbol::GetValueType() const
{
    return ValueType::boolValue;
}

Value* BoolTypeSymbol::MakeValue() const
{
    return new BoolValue(GetSourcePos(), SourceModuleId(), false);
}

void* BoolTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForBool();
}

SByteTypeSymbol::SByteTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::sbyteTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType SByteTypeSymbol::GetValueType() const
{
    return ValueType::sbyteValue;
}

Value* SByteTypeSymbol::MakeValue() const
{
    return new SByteValue(GetSourcePos(), SourceModuleId(), 0);
}

void* SByteTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForSByte();
}

ByteTypeSymbol::ByteTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::byteTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType ByteTypeSymbol::GetValueType() const
{
    return ValueType::byteValue;
}

Value* ByteTypeSymbol::MakeValue() const
{
    return new ByteValue(GetSourcePos(), SourceModuleId(), 0);
}

void* ByteTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForByte();
}

ShortTypeSymbol::ShortTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::shortTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType ShortTypeSymbol::GetValueType() const
{
    return ValueType::shortValue;
}

Value* ShortTypeSymbol::MakeValue() const
{
    return new ShortValue(GetSourcePos(), SourceModuleId(), 0);
}

void* ShortTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForShort();
}

UShortTypeSymbol::UShortTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::ushortTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType UShortTypeSymbol::GetValueType() const
{
    return ValueType::ushortValue;
}

Value* UShortTypeSymbol::MakeValue() const
{
    return new UShortValue(GetSourcePos(), SourceModuleId(), 0);
}

void* UShortTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForUShort();
}

IntTypeSymbol::IntTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::intTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType IntTypeSymbol::GetValueType() const
{
    return ValueType::intValue;
}

Value* IntTypeSymbol::MakeValue() const
{
    return new IntValue(GetSourcePos(), SourceModuleId(), 0);
}

void* IntTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForInt();
}

UIntTypeSymbol::UIntTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::uintTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType UIntTypeSymbol::GetValueType() const
{
    return ValueType::uintValue;
}

Value* UIntTypeSymbol::MakeValue() const
{
    return new UIntValue(GetSourcePos(), SourceModuleId(), 0);
}

void* UIntTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForUInt();
}

LongTypeSymbol::LongTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::longTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType LongTypeSymbol::GetValueType() const
{
    return ValueType::longValue;
}

Value* LongTypeSymbol::MakeValue() const
{
    return new LongValue(GetSourcePos(), SourceModuleId(), 0);
}

void* LongTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForLong();
}

ULongTypeSymbol::ULongTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::ulongTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType ULongTypeSymbol::GetValueType() const
{
    return ValueType::ulongValue;
}

Value* ULongTypeSymbol::MakeValue() const
{
    return new ULongValue(GetSourcePos(), SourceModuleId(), 0);
}

void* ULongTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForULong();
}

FloatTypeSymbol::FloatTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::floatTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType FloatTypeSymbol::GetValueType() const
{
    return ValueType::floatValue;
}

Value* FloatTypeSymbol::MakeValue() const
{
    return new FloatValue(GetSourcePos(), SourceModuleId(), 0.0);
}

void* FloatTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForFloat();
}

DoubleTypeSymbol::DoubleTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::doubleTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType DoubleTypeSymbol::GetValueType() const
{
    return ValueType::doubleValue;
}

Value* DoubleTypeSymbol::MakeValue() const
{
    return new DoubleValue(GetSourcePos(), SourceModuleId(), 0.0);
}

void* DoubleTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForDouble();
}

CharTypeSymbol::CharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::charTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType CharTypeSymbol::GetValueType() const
{
    return ValueType::charValue;
}

Value* CharTypeSymbol::MakeValue() const
{
    return new CharValue(GetSourcePos(), SourceModuleId(), '\0');
}

void* CharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForChar();
}

WCharTypeSymbol::WCharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::wcharTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType WCharTypeSymbol::GetValueType() const
{
    return ValueType::wcharValue;
}

Value* WCharTypeSymbol::MakeValue() const
{
    return new WCharValue(GetSourcePos(), SourceModuleId(), '\0');
}

void* WCharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForWChar();
}

UCharTypeSymbol::UCharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::ucharTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

ValueType UCharTypeSymbol::GetValueType() const
{
    return ValueType::ucharValue;
}

Value* UCharTypeSymbol::MakeValue() const
{
    return new UCharValue(GetSourcePos(), SourceModuleId(), '\0');
}

void* UCharTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForUChar();
}

VoidTypeSymbol::VoidTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : BasicTypeSymbol(SymbolType::voidTypeSymbol, sourcePos_, sourceModuleId_, name_)
{
}

void* VoidTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateDITypeForVoid();
}
} // namespace cmajor::symbols
