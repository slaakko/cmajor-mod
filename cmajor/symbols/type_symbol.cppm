// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.type.symbol;

import cmajor.symbols.container.symbol;
import soul.ast.span;
import cmajor.ir.emitter;
import std.core;

export namespace cmajor::symbols {

struct TypeDerivationRec;
enum class ValueType : uint8_t;
class Value;
class TemplateParameterSymbol;
struct ConversionTableEntry;

class TypeSymbol : public ContainerSymbol
{
public:
    TypeSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsTypeSymbol() const override { return true; }
    bool IsAutoType() const { return GetSymbolType() == SymbolType::autoTypeSymbol; }
    bool IsIntType() const { return GetSymbolType() == SymbolType::intTypeSymbol; }
    virtual bool IsInComplete() const { return false; }
    virtual bool IsIntegralType() const { return false; }
    virtual bool IsFloatingPointType() const { return false; }
    virtual bool IsUnsignedType() const { return false; }
    virtual bool IsVoidType() const { return false; }
    std::string TypeString(Context* context) const override { return "type"; }
    virtual bool IsBasicTypeSymbol() const { return false; }
    virtual bool IsDelegateType() const { return false; }
    virtual bool IsClassDelegateType() const { return false; }
    virtual bool IsEnumeratedType() const { return false; }
    virtual bool IsCharacterType() const { return false; }
    virtual bool IsCharacterPointerType() const { return false; }
    virtual bool IsConstCharPtrPtrType() const { return false; }
    virtual const TypeSymbol* BaseType() const { return this; }
    virtual TypeSymbol* BaseType() { return this; }
    virtual TypeSymbol* PlainType(Context* context) { return this; }
    virtual TypeSymbol* RemoveConst(Context* context) { return this; }
    virtual TypeSymbol* RemoveReference(Context* context) { return this; }
    virtual TypeSymbol* RemovePointer(Context* context) { return this; }
    virtual TypeSymbol* RemovePtrOrRef(Context* context) { return this; }
    virtual TypeSymbol* AddConst(Context* context);
    virtual TypeSymbol* AddLvalueReference(Context* context);
    virtual TypeSymbol* AddRvalueReference(Context* context);
    virtual TypeSymbol* AddPointer(Context* context);
    virtual void* IrType(cmajor::ir::Emitter& emitter, Context* context)= 0;
    virtual void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) = 0;
    virtual void* CreateDIType(cmajor::ir::Emitter& emitter, Context* context);
    virtual bool IsConstType() const { return false; }
    virtual bool IsReferenceType() const { return false; }
    virtual bool IsLvalueReferenceType() const { return false; }
    virtual bool IsRvalueReferenceType() const { return false; }
    virtual bool IsArrayType() const { return false; }
    virtual bool IsStringFunctionContainer() const { return false; }
    virtual bool IsPointerType() const { return false; }
    virtual bool IsNullPtrType() const { return false; }
    virtual bool IsVoidPtrType() const { return false; }
    virtual bool IsPolymorphicType() { return false; }
    virtual bool IsSwitchConditionType() const { return false; }
    virtual int PointerCount() const { return 0; }
    virtual bool HasNontrivialDestructor() { return false; }
    virtual bool ContainsTemplateParameter() const { return false; }
    virtual bool CompletelyBound() { return IsBound(); }
    void SetTypeId(const util::uuid& typeId_) { typeId = typeId_; }
    const util::uuid& TypeId() const { Assert(!typeId.is_nil(), "type id not initialized"); return typeId; }
    bool TypeIdNotSet() const { return typeId.is_nil(); }
    virtual const TypeDerivationRec& DerivationRec() const;
    virtual TypeSymbol* RemoveDerivations(const TypeDerivationRec& sourceDerivationRec, Context* context);
    virtual TypeSymbol* Unify(TypeSymbol* that, Context* context) { return nullptr; }
    virtual TypeSymbol* UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap) { return nullptr; }
    virtual bool IsRecursive(TypeSymbol* type, std::unordered_set<util::uuid, util::UuidHash>& tested);
    virtual ValueType GetValueType() const;
    virtual Value* MakeValue() const { return nullptr; }
    std::u32string Id() const override;
    void* GetDIType(cmajor::ir::Emitter& emitter, Context* context);
    uint64_t SizeInBits(cmajor::ir::Emitter& emitter, Context* context);
    uint32_t AlignmentInBits(cmajor::ir::Emitter& emitter, Context* context);
    const char* ClassName() const override { return "TypeSymbol"; }
    void Check() override;
private:
    util::uuid typeId;
};

bool CompareTypesForEquality(const TypeSymbol* left, const TypeSymbol* right);

inline bool TypesEqual(const TypeSymbol* left, const TypeSymbol* right)
{
    if (left->TypeId() == right->TypeId()) return true;
    return CompareTypesForEquality(left, right);
}

} // namespace cmajor::symbols
