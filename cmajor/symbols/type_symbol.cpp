// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.type.symbol;

import cmajor.symbols.context;
import cmajor.symbols.classes;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.modules;
import cmajor.symbols.value;
import cmajor.symbols.exception;
import soul.ast.span;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

TypeSymbol::TypeSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_) :
    ContainerSymbol(symbolType_, span_, name_), typeId(util::nil_uuid())
{
}

void TypeSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    Assert(!typeId.is_nil(), "type id not set");
    writer.GetBinaryStreamWriter().Write(typeId);
}

void TypeSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->AddTypeOrConceptSymbolToTypeIdMap(this);
}

TypeSymbol* TypeSymbol::AddConst(Context* context)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::constDerivation);
    return context->RootModule()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, context);
}

TypeSymbol* TypeSymbol::AddLvalueReference(Context* context)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::lvalueRefDerivation);
    return context->RootModule()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, context);
}

TypeSymbol* TypeSymbol::AddRvalueReference(Context* context)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::rvalueRefDerivation);
    return context->RootModule()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, context);
}

TypeSymbol* TypeSymbol::AddPointer(Context* context)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::pointerDerivation);
    return context->RootModule()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, context);
}

void* TypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.CreateUnspecifiedDIType(util::ToUtf8(Name()));
}

const TypeDerivationRec& TypeSymbol::DerivationRec() const
{
    static TypeDerivationRec emptyDerivationRec;
    return emptyDerivationRec;
}

TypeSymbol* TypeSymbol::RemoveDerivations(const TypeDerivationRec& sourceDerivationRec, Context* context)
{
    if (HasPointerDerivation(sourceDerivationRec.derivations)) return nullptr;
    return this;
}

bool TypeSymbol::IsRecursive(TypeSymbol* type, std::unordered_set<util::uuid, util::UuidHash>& tested)
{
    if (tested.find(TypeId()) != tested.cend()) return TypesEqual(type, this);
    tested.insert(TypeId());
    return TypesEqual(type, this);
}

ValueType TypeSymbol::GetValueType() const
{
    return ValueType::none;
}

std::u32string TypeSymbol::Id() const
{
    return util::ToUtf32(util::ToString(TypeId()));
}

void* TypeSymbol::GetDIType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localDiType = emitter.GetDITypeByTypeId(TypeId());
    if (!localDiType)
    {
        if (IsClassTypeSymbol())
        {
            ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(this);
            emitter.MapClassPtr(classTypeSymbol->TypeId(), classTypeSymbol, util::ToUtf8(classTypeSymbol->FullName()));
            localDiType = classTypeSymbol->CreateDIForwardDeclaration(emitter, context);
            emitter.MapFwdDeclaration(localDiType, classTypeSymbol->TypeId());
            emitter.SetDITypeByTypeId(classTypeSymbol->TypeId(), localDiType, util::ToUtf8(classTypeSymbol->FullName()));
        }
        localDiType = CreateDIType(emitter, context);
        emitter.SetDITypeByTypeId(TypeId(), localDiType, util::ToUtf8(FullName()));
    }
    return localDiType;
}

uint64_t TypeSymbol::SizeInBits(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.GetSizeInBits(IrType(emitter, context));
}

uint32_t TypeSymbol::AlignmentInBits(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.GetAlignmentInBits(IrType(emitter, context));
}

void TypeSymbol::Check()
{
    ContainerSymbol::Check();
    if (typeId.is_nil())
    {
        throw SymbolCheckException("type symbol contains empty type id", GetFullSpan());
    }
}

bool CompareTypesForEquality(const TypeSymbol* left, const TypeSymbol* right)
{
    if (left->GetSymbolType() == SymbolType::derivedTypeSymbol && right->GetSymbolType() == SymbolType::derivedTypeSymbol)
    {
        const DerivedTypeSymbol* derivedLeft = static_cast<const DerivedTypeSymbol*>(left);
        const DerivedTypeSymbol* derivedRight = static_cast<const DerivedTypeSymbol*>(right);
        if (TypesEqual(derivedLeft->BaseType(), derivedRight->BaseType()) && derivedLeft->DerivationRec() == derivedRight->DerivationRec())
        {
            return true;
        }
    }
    return false;
}
} // namespace cmajor::symbols
