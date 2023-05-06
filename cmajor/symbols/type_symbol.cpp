// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.type.symbol;

import cmajor.symbols.classes;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.modules;
import cmajor.symbols.value;
import cmajor.symbols.exception;
import soul.ast.source.pos;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

TypeSymbol::TypeSymbol(SymbolType symbolType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    ContainerSymbol(symbolType_, sourcePos_, sourceModuleId_, name_), typeId(util::nil_uuid())
{
}

void TypeSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    //Assert(!typeId.is_nil(), "type id not set");
    writer.GetBinaryStreamWriter().Write(typeId);
}

void TypeSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->AddTypeOrConceptSymbolToTypeIdMap(this);
}

TypeSymbol* TypeSymbol::AddConst(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::constDerivation);
    return GetRootModuleForCurrentThread()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, sourcePos, moduleId);
}

TypeSymbol* TypeSymbol::AddLvalueReference(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::lvalueRefDerivation);
    return GetRootModuleForCurrentThread()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, sourcePos, moduleId);
}

TypeSymbol* TypeSymbol::AddRvalueReference(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::rvalueRefDerivation);
    return GetRootModuleForCurrentThread()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, sourcePos, moduleId);
}

TypeSymbol* TypeSymbol::AddPointer(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    TypeDerivationRec typeDerivationRec;
    typeDerivationRec.derivations.push_back(Derivation::pointerDerivation);
    return GetRootModuleForCurrentThread()->GetSymbolTable().MakeDerivedType(this, typeDerivationRec, sourcePos, moduleId);
}

void* TypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    return emitter.CreateUnspecifiedDIType(util::ToUtf8(Name()));
}

const TypeDerivationRec& TypeSymbol::DerivationRec() const
{
    static TypeDerivationRec emptyDerivationRec;
    return emptyDerivationRec;
}

TypeSymbol* TypeSymbol::RemoveDerivations(const TypeDerivationRec& sourceDerivationRec, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (HasPointerDerivation(sourceDerivationRec.derivations)) return nullptr;
    return this;
}

bool TypeSymbol::IsRecursive(TypeSymbol* type, std::set<util::uuid>& tested)
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

void* TypeSymbol::GetDIType(cmajor::ir::Emitter& emitter)
{
    void* localDiType = emitter.GetDITypeByTypeId(TypeId());
    if (!localDiType)
    {
        if (IsClassTypeSymbol())
        {
            ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(this);
            emitter.MapClassPtr(classTypeSymbol->TypeId(), classTypeSymbol, util::ToUtf8(classTypeSymbol->FullName()));
            localDiType = classTypeSymbol->CreateDIForwardDeclaration(emitter);
            emitter.MapFwdDeclaration(localDiType, classTypeSymbol->TypeId());
            emitter.SetDITypeByTypeId(classTypeSymbol->TypeId(), localDiType, util::ToUtf8(classTypeSymbol->FullName()));
        }
        localDiType = CreateDIType(emitter);
        emitter.SetDITypeByTypeId(TypeId(), localDiType, util::ToUtf8(FullName()));
    }
    return localDiType;
}

uint64_t TypeSymbol::SizeInBits(cmajor::ir::Emitter& emitter)
{
    return emitter.GetSizeInBits(IrType(emitter));
}

uint32_t TypeSymbol::AlignmentInBits(cmajor::ir::Emitter& emitter)
{
    return emitter.GetAlignmentInBits(IrType(emitter));
}

void TypeSymbol::Check()
{
    ContainerSymbol::Check();
    if (typeId.is_nil())
    {
        throw SymbolCheckException("type symbol contains empty type id", GetSourcePos(), SourceModuleId());
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
