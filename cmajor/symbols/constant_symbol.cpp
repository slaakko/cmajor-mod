// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.constant.symbol;

import soul.ast.source.pos;
import cmajor.symbols.exception;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.type.symbol;
import cmajor.symbols.value;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

ConstantSymbol::ConstantSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    Symbol(SymbolType::constantSymbol, sourcePos_, sourceModuleId_, name_), type(), evaluating(false), sizeOfValue(0), valuePos(0)
{
}

void ConstantSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
    bool hasComplexValue = value->IsComplexValue();
    writer.GetBinaryStreamWriter().Write(hasComplexValue);
    if (hasComplexValue)
    {
        /* TODO: Seppo, fix this
        uint32_t sizePos = writer.GetBinaryStreamWriter().Pos();
        uint32_t sizeOfValue = 0;
        writer.GetBinaryStreamWriter().Write(sizeOfValue);
        uint32_t startPos = writer.GetBinaryStreamWriter().Pos();
        value->Write(writer.GetBinaryStreamWriter());
        uint32_t endPos = writer.GetBinaryStreamWriter().Pos();
        sizeOfValue = endPos - startPos;
        writer.GetBinaryStreamWriter().Seek(sizePos);
        writer.GetBinaryStreamWriter().Write(sizeOfValue);
        writer.GetBinaryStreamWriter().Seek(endPos);
        */
    }
    else
    {
        WriteValue(value.get(), writer.GetBinaryStreamWriter());
    }
    writer.GetBinaryStreamWriter().Write(strValue);
}

void ConstantSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 0);
    bool hasComplexValue = reader.GetBinaryStreamReader().ReadBool();
    if (hasComplexValue)
    {
        /* TODO: Seppo, fix this
        sizeOfValue = reader.GetBinaryStreamReader().ReadUInt();
        valuePos = reader.GetBinaryStreamReader().Pos();
        reader.GetBinaryStreamReader().Skip(sizeOfValue);
        filePathReadFrom = reader.GetBinaryStreamReader().FileName();
        */
    }
    else
    {
        value = ReadValue(reader.GetBinaryStreamReader(), GetSourcePos(), SourceModuleId());
    }
    strValue = reader.GetBinaryStreamReader().ReadUtf32String();
}

Value* ConstantSymbol::GetValue()
{
    if (!value)
    {
        if (filePathReadFrom.empty())
        {
            throw Exception("internal error: could not read value: value file name not set", GetSourcePos(), SourceModuleId());
        }
        util::FileStream file(filePathReadFrom, util::OpenMode::read | util::OpenMode::binary);
        util::BufferedStream bufferedFile(file);
        util::BinaryStreamReader reader(bufferedFile);
        reader.GetStream().Seek(valuePos, util::Origin::seekSet);
        value.reset(type->MakeValue());
        if (!value)
        {
            throw Exception("internal error: could not read value because could not create value of type '" + util::ToUtf8(type->FullName()) + "'", GetSourcePos(), SourceModuleId());
        }
        value->Read(reader);
    }
    return value.get();
}

void ConstantSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    Assert(index == 0, "invalid emplace type index");
    type = typeSymbol;
}

void ConstantSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddConstant(this);
    }
}

void ConstantSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("type: " + util::ToUtf8(type->FullName()));
    formatter.WriteLine("value: " + value->ToString());
}

void ConstantSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constant cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

void ConstantSymbol::ComputeMangledName()
{
    Symbol::ComputeMangledName();
}


std::string ConstantSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("const ");
    syntax.append(util::ToUtf8(GetType()->DocName()));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName()));
    if (value)
    {
        syntax.append(" = ");
        std::string valueStr = value->ToString();
        if (GetType()->IsUnsignedType())
        {
            valueStr.append(1, 'u');
        }
        syntax.append(valueStr);
    }
    syntax.append(1, ';');
    return syntax;
}

void ConstantSymbol::SetValue(Value* value_)
{
    value.reset(value_);
}

void* ConstantSymbol::ArrayIrObject(cmajor::ir::Emitter& emitter, bool create)
{
    if (!type->IsArrayType())
    {
        throw Exception("internal error: array object expected", GetSourcePos(), SourceModuleId());
    }
    if (!value)
    {
        throw Exception("internal error: array value missing", GetSourcePos(), SourceModuleId());
    }
    if (value->GetValueType() != ValueType::arrayValue)
    {
        throw Exception("internal error: array value expected", GetSourcePos(), SourceModuleId());
    }
    ArrayValue* arrayValue = static_cast<ArrayValue*>(value.get());
    void* irArrayType = type->IrType(emitter);
    void* irArrayObject = emitter.GetOrInsertGlobal(util::ToUtf8(MangledName()), irArrayType);
    if (create)
    {
        void* arrayObjectGlobal = irArrayObject;
        emitter.SetInitializer(arrayObjectGlobal, arrayValue->IrValue(emitter));
    }
    return irArrayObject;
}

void* ConstantSymbol::StructureIrObject(cmajor::ir::Emitter& emitter, bool create)
{
    if (!type->IsClassTypeSymbol())
    {
        throw Exception("internal error: class type object expected", GetSourcePos(), SourceModuleId());
    }
    if (!value)
    {
        throw Exception("internal error: structured value missing", GetSourcePos(), SourceModuleId());
    }
    if (value->GetValueType() != ValueType::structuredValue)
    {
        throw Exception("internal error: structured value expected", GetSourcePos(), SourceModuleId());
    }
    StructuredValue* structuredValue = static_cast<StructuredValue*>(value.get());
    void* irStructureType = type->IrType(emitter);
    void* irStructureObject = emitter.GetOrInsertGlobal(util::ToUtf8(MangledName()), irStructureType);
    if (create)
    {
        void* structureObjectGlobal = irStructureObject;
        emitter.SetInitializer(structureObjectGlobal, structuredValue->IrValue(emitter));
    }
    return irStructureObject;
}

void ConstantSymbol::Check()
{
    Symbol::Check();
    if (!type)
    {
        throw SymbolCheckException("constant symbol has no type", GetSourcePos(), SourceModuleId());
    }
}

std::string ConstantSymbol::GetSymbolHelp() const
{
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(GetType()->FullName())).append(" ").append(util::ToUtf8(FullName()));
    return help;
}
} // namespace cmajor::symbols
