// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.derived.type.symbol;

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
    DerivedTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    DerivedTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_, TypeSymbol* baseType_, const TypeDerivationRec& derivationRec_);
    ~DerivedTypeSymbol();
    std::string TypeString() const override { return "derived_type"; }
    std::u32string SimpleName() const override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void ComputeTypeId();
    const TypeSymbol* BaseType() const override { return baseType; }
    TypeSymbol* BaseType() override { return baseType; }
    TypeSymbol* PlainType() override;
    TypeSymbol* RemoveReference() override;
    TypeSymbol* RemovePointer() override;
    TypeSymbol* RemovePtrOrRef() override;
    TypeSymbol* RemoveConst() override;
    TypeSymbol* AddConst() override;
    TypeSymbol* AddLvalueReference() override;
    TypeSymbol* AddRvalueReference() override;
    TypeSymbol* AddPointer() override;
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
    bool IsConstCharPtrPtrType() const override;
    int PointerCount() const override;
    const ContainerScope* GetArrowScope() const override;
    ContainerScope* GetArrowScope() override;
    bool ContainsTemplateParameter() const override { return baseType->ContainsTemplateParameter(); }
    const TypeDerivationRec& DerivationRec() const override { return derivationRec; }
    TypeSymbol* RemoveDerivations(const TypeDerivationRec& sourceDerivationRec) override;
    TypeSymbol* Unify(TypeSymbol* sourceType) override;
    bool IsRecursive(TypeSymbol* type, std::unordered_set<util::uuid, util::UuidHash>& tested) override;
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
    NullPtrType(const soul::ast::Span& span_, const std::u32string& name_);
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
