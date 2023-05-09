// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.constraint;

import std.core;
import cmajor.symbols;
import cmajor.ir;
import cmajor.binder.bound.node;

export namespace cmajor::binder {

class BoundConstraint : public BoundNode
{
public:
    BoundConstraint(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundNodeType boundNodeType_);
    virtual bool Subsume(BoundConstraint* that) const = 0;
    virtual BoundConstraint* Clone() const = 0;
    virtual bool IsBinaryConstraint() const { return false; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
};

class BoundAtomicConstraint : public BoundConstraint
{
public:
    BoundAtomicConstraint(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, bool satisfied_);
    void Accept(BoundNodeVisitor& visitor) override;
    bool Subsume(BoundConstraint* that) const override;
    BoundConstraint* Clone() const override;
    bool Satisfied() const { return satisfied; }
    void SetConcept(cmajor::symbols::ConceptSymbol* conceptSymbol_) { conceptSymbol = conceptSymbol_; }
private:
    bool satisfied;
    cmajor::symbols::ConceptSymbol* conceptSymbol;
};

class BoundBinaryConstraint : public BoundConstraint
{
public:
    BoundBinaryConstraint(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundNodeType boundNodeType_, BoundConstraint* left_, BoundConstraint* right_);
    BoundBinaryConstraint(const BoundBinaryConstraint& that);
    bool IsBinaryConstraint() const override { return true; }
    BoundConstraint* Left() const { return left.get(); }
    BoundConstraint* Right() const { return right.get(); }
private:
    std::unique_ptr<BoundConstraint> left;
    std::unique_ptr<BoundConstraint> right;
};

class BoundDisjunctiveConstraint : public BoundBinaryConstraint
{
public:
    BoundDisjunctiveConstraint(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundConstraint* left_, BoundConstraint* right_);
    BoundDisjunctiveConstraint(const BoundDisjunctiveConstraint& that);
    bool Subsume(BoundConstraint* that) const override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundConstraint* Clone() const override;
};

class BoundConjunctiveConstraint : public BoundBinaryConstraint
{
public:
    BoundConjunctiveConstraint(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundConstraint* left_, BoundConstraint* right_);
    BoundConjunctiveConstraint(const BoundConjunctiveConstraint& that);
    bool Subsume(BoundConstraint* that) const override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundConstraint* Clone() const override;
};

class BoundConcept : public BoundNode
{
public:
    BoundConcept(cmajor::symbols::ConceptSymbol* conceptSymbol_, const std::vector<cmajor::symbols::TypeSymbol*>& typeArguments_, 
        const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    cmajor::symbols::ConceptSymbol* GetConceptSymbol() const { return conceptSymbol; }
    const std::vector<cmajor::symbols::TypeSymbol*>& TypeArguments() const { return typeArguments; }
    int Arity() const { return typeArguments.size(); }
    cmajor::symbols::TypeSymbol* CommonType() const { return commonType; }
    void SetCommonType(cmajor::symbols::TypeSymbol* commonType_) { commonType = commonType_; }
    void SetBoundConstraint(std::unique_ptr<BoundConstraint>&& boundConstraint_);
    BoundConstraint* GetBoundConstraint() { return boundConstraint.get(); }
    void AddBoundTemplateParameter(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>&& boundTemplateParameter);
private:
    std::u32string name;
    cmajor::symbols::ConceptSymbol* conceptSymbol;
    std::vector<cmajor::symbols::TypeSymbol*> typeArguments;
    cmajor::symbols::TypeSymbol* commonType;
    std::unique_ptr<BoundConstraint> boundConstraint;
    std::vector<std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>> boundTemplateParameters;
};

struct BoundConceptKey
{
    BoundConceptKey(cmajor::symbols::ConceptSymbol* conceptSymbol_, std::vector<cmajor::symbols::TypeSymbol*> typeArguments_) : 
        conceptSymbol(conceptSymbol_), typeArguments(typeArguments_) {}
    int Arity() const { return typeArguments.size(); }
    cmajor::symbols::ConceptSymbol* conceptSymbol;
    std::vector<cmajor::symbols::TypeSymbol*> typeArguments;
};

bool operator==(const BoundConceptKey& left, const BoundConceptKey& right);
inline bool operator!=(const BoundConceptKey& left, const BoundConceptKey& right)
{
    return !(left == right);
}

struct BoundConceptKeyHash
{
    bool operator()(const BoundConceptKey& boundConceptKey) const
    {
        size_t hashCode = util::HashValue(boundConceptKey.conceptSymbol->TypeId());
        int n = boundConceptKey.Arity();
        for (int i = 0; i < n; ++i)
        {
            hashCode ^= util::HashValue(boundConceptKey.typeArguments[i]->TypeId());
        }
        return hashCode;
    }
};

} // namespace cmajor::binder
