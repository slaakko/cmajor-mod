module;
#include <boost/uuid/uuid.hpp>
export module cmajor.symbols.derived.type.symbol;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/TypeSymbol.hpp>

import cmajor.symbols.type.symbol;
import std.core;

export namespace cmajor::symbols {

enum class Derivation : uint8_t
{
    none = 0, constDerivation = 1, lvalueRefDerivation = 2, rvalueRefDerivation = 3, pointerDerivation = 4, max
};

std::u32string DerivationStr(Derivation derivation);

struct TypeDerivationRec
{
    std::vector<Derivation> derivations;
    bool IsEmpty() const { return derivations.empty(); }
};

inline bool operator==(const TypeDerivationRec& left, const TypeDerivationRec& right)
{
    return left.derivations == right.derivations;
}

inline bool operator!=(const TypeDerivationRec& left, const TypeDerivationRec& right)
{
    return !(left == right);
}

std::u32string MakeDerivedTypeName(TypeSymbol* baseType, const TypeDerivationRec& derivationRec);

bool HasFrontConstDerivation(const std::vector<Derivation>& derivations);
bool HasReferenceDerivation(const std::vector<Derivation>& derivations);
bool HasLvalueReferenceDerivation(const std::vector<Derivation>& derivations);
bool HasRvalueReferenceDerivation(const std::vector<Derivation>& derivations);
bool HasReferenceOrConstDerivation(const std::vector<Derivation>& derivations);
bool HasPointerDerivation(const std::vector<Derivation>& derivations);
int CountPointerDerivations(const std::vector<Derivation>& derivations);

TypeDerivationRec MakePlainDerivationRec(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec RemoveReferenceDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec RemovePointerDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec RemoveConstDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec AddConstDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec AddLvalueReferenceDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec AddRvalueReferenceDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec AddPointerDerivation(const TypeDerivationRec& typeDerivationRec);
TypeDerivationRec UnifyDerivations(const TypeDerivationRec& left, const TypeDerivationRec& right);

class DerivedTypeSymbol : public TypeSymbol
{
public:
    DerivedTypeSymbol(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& sourceModuleId_, const std::u32string& name_);
    DerivedTypeSymbol(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& sourceModuleId_, const std::u32string& name_, TypeSymbol* baseType_, const TypeDerivationRec& derivationRec_);
    ~DerivedTypeSymbol();
    std::string TypeString() const override { return "derived_type"; }
    std::u32string SimpleName() const override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void ComputeTypeId();
    const TypeSymbol* BaseType() const override { return baseType; }
    TypeSymbol* BaseType() override { return baseType; }
    TypeSymbol* PlainType(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* RemoveReference(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* RemovePointer(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* RemoveConst(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* AddConst(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* AddLvalueReference(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* AddRvalueReference(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* AddPointer(const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsConstType() const override;
    bool IsReferenceType() const override;
    bool IsLvalueReferenceType() const override;
    bool IsRvalueReferenceType() const override;
    bool IsPointerType() const override;
    bool IsVoidPtrType() const override;
    bool IsCharacterPointerType() const override;
    int PointerCount() const override;
    const ContainerScope* GetArrowScope() const override;
    ContainerScope* GetArrowScope() override;
    bool ContainsTemplateParameter() const override { return baseType->ContainsTemplateParameter(); }
    const TypeDerivationRec& DerivationRec() const override { return derivationRec; }
    TypeSymbol* RemoveDerivations(const TypeDerivationRec& sourceDerivationRec, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    TypeSymbol* Unify(TypeSymbol* sourceType, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) override;
    //bool IsRecursive(TypeSymbol* type, std::unordered_set<boost::uuids::uuid, boost::hash<boost::uuids::uuid>>& tested) override;
    bool IsRecursive(TypeSymbol* type, std::set<boost::uuids::uuid>& tested) override;
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "DerivedTypeSymbol"; }
    void Check() override;
private:
    TypeSymbol* baseType;
    TypeDerivationRec derivationRec;
};

class NullPtrType : public TypeSymbol
{
public:
    NullPtrType(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "nullptr_type"; }
    bool IsPointerType() const override { return true; }
    bool IsNullPtrType() const override { return true; }
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
    ValueType GetValueType() const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "NullPtrTypeSymbol"; }
};
} // namespace cmajor::symbols