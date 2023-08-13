// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.concepts;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.resolver;
import cmajor.binder.bound.constraint;
import cmajor.binder.bound.function;
import cmajor.binder.overload.resolution;
import cmajor.binder.evaluator;
import cmajor.binder.bound.expression;
import util;

namespace cmajor::binder {

class ConceptIdResolver : public cmajor::ast::Visitor
{
public:
    ConceptIdResolver(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_);
    cmajor::symbols::ConceptSymbol* GetConcept() const { return conceptSymbol; }
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;
    cmajor::ast::IdentifierNode* IdNode() const { return idNode; }
private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::ContainerScope* containerScope;
    cmajor::symbols::ConceptGroupSymbol* conceptGroup;
    cmajor::symbols::ConceptSymbol* conceptSymbol;
    cmajor::symbols::NamespaceSymbol* ns;
    cmajor::ast::IdentifierNode* idNode;
};

ConceptIdResolver::ConceptIdResolver(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_) :
    boundCompileUnit(boundCompileUnit_), containerScope(containerScope_), conceptGroup(nullptr), conceptSymbol(nullptr), ns(nullptr), idNode(nullptr)
{
}

void ConceptIdResolver::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    conceptIdNode.Id()->Accept(*this);
    if (conceptGroup)
    {
        int arity = conceptIdNode.Arity();
        conceptSymbol = conceptGroup->GetConcept(arity);
        if (conceptIdNode.Id() && conceptSymbol)
        {
            MapIdentifierToSymbolDefinition(conceptIdNode.Id(), conceptSymbol);
        }
    }
    else
    {
        throw cmajor::symbols::Exception("concept symbol '" + conceptIdNode.Id()->ToString() + "' not found", conceptIdNode.GetSourcePos(), conceptIdNode.ModuleId());
    }
}

void ConceptIdResolver::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    idNode = &identifierNode;
    const std::u32string& name = identifierNode.Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::conceptGroupSymbol)
        {
            conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
        }
        else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::namespaceSymbol)
        {
            ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a concept", identifierNode.GetSourcePos(), identifierNode.ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("concept symbol '" + util::ToUtf8(name) + "' not found", identifierNode.GetSourcePos(), identifierNode.ModuleId());
    }
}

void ConceptIdResolver::Visit(cmajor::ast::DotNode& dotNode)
{
    dotNode.Subject()->Accept(*this);
    if (!ns)
    {
        throw cmajor::symbols::Exception("concept symbol '" + dotNode.ToString() + "' not found", dotNode.GetSourcePos(), dotNode.ModuleId());
    }
    cmajor::symbols::ContainerScope* containerScope = ns->GetContainerScope();
    const std::u32string& name = dotNode.MemberId()->Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_);
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::conceptGroupSymbol)
        {
            conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
        }
        else if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::namespaceSymbol)
        {
            ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a concept", dotNode.GetSourcePos(), dotNode.ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("concept symbol '" + util::ToUtf8(name) + "' not found", dotNode.GetSourcePos(), dotNode.ModuleId());
    }
}

cmajor::symbols::ConceptSymbol* ResolveConceptId(cmajor::ast::ConceptIdNode* conceptIdNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    ConceptIdResolver conceptIdResolver(boundCompileUnit, containerScope);
    conceptIdNode->Accept(conceptIdResolver);
    cmajor::symbols::ConceptSymbol* conceptSymbol = conceptIdResolver.GetConcept();
    if (conceptSymbol)
    {
        return conceptSymbol;
    }
    else
    {
        throw cmajor::symbols::Exception("concept symbol '" + conceptIdNode->Id()->ToString() + "' not found", conceptIdNode->GetSourcePos(), conceptIdNode->ModuleId());
    }
}

std::unique_ptr<BoundConcept> Instantiate(cmajor::symbols::ConceptSymbol* conceptSymbol, const std::vector<cmajor::symbols::TypeSymbol*>& typeArguments, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope,
    BoundFunction* currentFunction, std::unique_ptr<BoundConstraint>& boundConstraint, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, 
    std::unique_ptr<cmajor::symbols::Exception>& exception);

class ConstraintChecker : public cmajor::ast::Visitor
{
public:
    ConstraintChecker(cmajor::symbols::TypeSymbol* firstTypeArgument_, cmajor::symbols::TypeSymbol* secondTypeArgument_, BoundCompileUnit& boundCompileUnit_, 
        cmajor::symbols::ContainerScope* containerScope_, BoundFunction* currentFunction_,
        const soul::ast::SourcePos& span_, const util::uuid& moduleId, std::unique_ptr<cmajor::symbols::Exception>& exception_);
    ~ConstraintChecker();
    bool Result() const { return result; }
    std::unique_ptr<BoundConstraint> GetBoundConstraint() { return std::move(boundConstraint); }
    void Visit(cmajor::ast::AutoNode& autoNode) override;
    void Visit(cmajor::ast::BoolNode& boolNode) override;
    void Visit(cmajor::ast::SByteNode& sbyteNode) override;
    void Visit(cmajor::ast::ByteNode& byteNode) override;
    void Visit(cmajor::ast::ShortNode& shortNode) override;
    void Visit(cmajor::ast::UShortNode& ushortNode) override;
    void Visit(cmajor::ast::IntNode& intNode) override;
    void Visit(cmajor::ast::UIntNode& uintNode) override;
    void Visit(cmajor::ast::LongNode& longNode) override;
    void Visit(cmajor::ast::ULongNode& ulongNode) override;
    void Visit(cmajor::ast::FloatNode& floatNode) override;
    void Visit(cmajor::ast::DoubleNode& doubleNode) override;
    void Visit(cmajor::ast::CharNode& charNode) override;
    void Visit(cmajor::ast::WCharNode& wcharNode) override;
    void Visit(cmajor::ast::UCharNode& ucharNode) override;
    void Visit(cmajor::ast::VoidNode& voidNode) override;
    void Visit(cmajor::ast::ConstNode& constNode) override;
    void Visit(cmajor::ast::LValueRefNode& lvalueRefNode) override;
    void Visit(cmajor::ast::RValueRefNode& rvalueRefNode) override;
    void Visit(cmajor::ast::PointerNode& pointerNode) override;
    void Visit(cmajor::ast::ArrayNode& arrayNode) override;
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;

    void Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode) override;
    void Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode) override;
    void Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode) override;
    void Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode) override;
    void Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode) override;
    void Visit(cmajor::ast::IsConstraintNode& isConstraintNode) override;
    void Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode) override;
    void Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode) override;
    void Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode) override;
    void Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode) override;
    void Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode) override;
    void Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode) override;
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;

    void Visit(cmajor::ast::SameConstraintNode& sameConstraintNode) override;
    void Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode) override;
    void Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode) override;
    void Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode) override;
    void Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode) override;
    void Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode) override;
private:
    cmajor::symbols::TypeSymbol* firstTypeArgument;
    cmajor::symbols::TypeSymbol* secondTypeArgument;
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::ContainerScope* containerScope;
    BoundFunction* currentFunction;
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    cmajor::symbols::TypeSymbol* type;
    cmajor::symbols::TypeDerivationRec derivationRec;
    cmajor::symbols::ConceptGroupSymbol* conceptGroup;
    bool result;
    std::unique_ptr<BoundConstraint> boundConstraint;
    int fileScopesAdded;
    std::vector<std::unique_ptr<NamespaceTypeSymbol>> namespaceTypeSymbols;
    std::vector<std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>> boundTemplateParameters;
    std::unique_ptr<cmajor::symbols::Exception>& exception;
    cmajor::ast::IdentifierNode* idNode;
    void Reset();
    cmajor::symbols::TypeSymbol* GetType();
};

ConstraintChecker::ConstraintChecker(cmajor::symbols::TypeSymbol* firstTypeArgument_, cmajor::symbols::TypeSymbol* secondTypeArgument_, BoundCompileUnit& boundCompileUnit_, 
    cmajor::symbols::ContainerScope* containerScope_,
    BoundFunction* currentFunction_, const soul::ast::SourcePos& span_, const util::uuid& moduleId_, std::unique_ptr<cmajor::symbols::Exception>& exception_) :
    firstTypeArgument(firstTypeArgument_), secondTypeArgument(secondTypeArgument_), boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()),
    containerScope(containerScope_), currentFunction(currentFunction_), sourcePos(span_), moduleId(moduleId_), type(nullptr), derivationRec(), conceptGroup(nullptr), 
    result(false), boundConstraint(), fileScopesAdded(0), exception(exception_), idNode(nullptr)
{
}

ConstraintChecker::~ConstraintChecker()
{
    for (int i = 0; i < fileScopesAdded; ++i)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
}

void ConstraintChecker::Reset()
{
    type = nullptr;
    derivationRec = cmajor::symbols::TypeDerivationRec();
    conceptGroup = nullptr;
}

cmajor::symbols::TypeSymbol* ConstraintChecker::GetType()
{
    if (type && type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
        type = classGroup->GetClass(0);
    }
    if (type && !derivationRec.IsEmpty())
    {
        cmajor::symbols::TypeDerivationRec unifiedDerivationRec = UnifyDerivations(derivationRec, type->DerivationRec());
        if (!unifiedDerivationRec.derivations.empty())
        {
            type = boundCompileUnit.GetSymbolTable().MakeDerivedType(type->BaseType(), unifiedDerivationRec, sourcePos, moduleId);
        }
    }
    return type;
}

void ConstraintChecker::Visit(cmajor::ast::AutoNode& autoNode)
{
    type = symbolTable.GetTypeByName(U"auto");
}

void ConstraintChecker::Visit(cmajor::ast::BoolNode& boolNode)
{
    type = symbolTable.GetTypeByName(U"bool");
}

void ConstraintChecker::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    type = symbolTable.GetTypeByName(U"sbyte");
}

void ConstraintChecker::Visit(cmajor::ast::ByteNode& byteNode)
{
    type = symbolTable.GetTypeByName(U"byte");
}

void ConstraintChecker::Visit(cmajor::ast::ShortNode& shortNode)
{
    type = symbolTable.GetTypeByName(U"short");
}

void ConstraintChecker::Visit(cmajor::ast::UShortNode& ushortNode)
{
    type = symbolTable.GetTypeByName(U"ushort");
}

void ConstraintChecker::Visit(cmajor::ast::IntNode& intNode)
{
    type = symbolTable.GetTypeByName(U"int");
}

void ConstraintChecker::Visit(cmajor::ast::UIntNode& uintNode)
{
    type = symbolTable.GetTypeByName(U"uint");
}

void ConstraintChecker::Visit(cmajor::ast::LongNode& longNode)
{
    type = symbolTable.GetTypeByName(U"long");
}

void ConstraintChecker::Visit(cmajor::ast::ULongNode& ulongNode)
{
    type = symbolTable.GetTypeByName(U"ulong");
}

void ConstraintChecker::Visit(cmajor::ast::FloatNode& floatNode)
{
    type = symbolTable.GetTypeByName(U"float");
}

void ConstraintChecker::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    type = symbolTable.GetTypeByName(U"double");
}

void ConstraintChecker::Visit(cmajor::ast::CharNode& charNode)
{
    type = symbolTable.GetTypeByName(U"char");
}

void ConstraintChecker::Visit(cmajor::ast::WCharNode& wcharNode)
{
    type = symbolTable.GetTypeByName(U"wchar");
}

void ConstraintChecker::Visit(cmajor::ast::UCharNode& ucharNode)
{
    type = symbolTable.GetTypeByName(U"uchar");
}

void ConstraintChecker::Visit(cmajor::ast::VoidNode& voidNode)
{
    type = symbolTable.GetTypeByName(U"void");
}

void ConstraintChecker::Visit(cmajor::ast::ConstNode& constNode)
{
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::constDerivation);
    constNode.Subject()->Accept(*this);
}

void ConstraintChecker::Visit(cmajor::ast::LValueRefNode& lvalueRefNode)
{
    lvalueRefNode.Subject()->Accept(*this);
    if (cmajor::symbols::HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have reference to reference type", lvalueRefNode.GetSourcePos(), lvalueRefNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::lvalueRefDerivation);
}

void ConstraintChecker::Visit(cmajor::ast::RValueRefNode& rvalueRefNode)
{
    rvalueRefNode.Subject()->Accept(*this);
    if (cmajor::symbols::HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have reference to reference type", rvalueRefNode.GetSourcePos(), rvalueRefNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::rvalueRefDerivation);
}

void ConstraintChecker::Visit(cmajor::ast::PointerNode& pointerNode)
{
    pointerNode.Subject()->Accept(*this);
    if (cmajor::symbols::HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have pointer to reference type", pointerNode.GetSourcePos(), pointerNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::pointerDerivation);
}

void ConstraintChecker::Visit(cmajor::ast::ArrayNode& arrayNode)
{
    arrayNode.Subject()->Accept(*this);
    if (cmajor::symbols::HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have array of reference type", arrayNode.GetSourcePos(), arrayNode.ModuleId());
    }
    // todo: evaluate size
}

void ConstraintChecker::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    idNode = &identifierNode;
    Reset();
    const std::u32string& name = identifierNode.Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            type = static_cast<cmajor::symbols::TypeSymbol*>(symbol);
            if (type)
            {
                MapIdentifierToSymbolDefinition(&identifierNode, type);
            }
        }
        else
        {
            switch (symbol->GetSymbolType())
            {
            case cmajor::symbols::SymbolType::aliasTypeSymbol:
            {
                cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::symbols::AliasTypeSymbol*>(symbol);
                type = aliasTypeSymbol->GetType();
                if (type)
                {
                    MapIdentifierToSymbolDefinition(&identifierNode, type);
                }
                break;
            }
            case cmajor::symbols::SymbolType::boundTemplateParameterSymbol:
            {
                cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameterSymbol = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(symbol);
                type = boundTemplateParameterSymbol->GetType();
                if (type)
                {
                    MapIdentifierToSymbolDefinition(&identifierNode, type);
                }
                break;
            }
            case cmajor::symbols::SymbolType::classGroupTypeSymbol:
            {
                cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(symbol);
                type = classGroup->GetClass(0);
                if (type)
                {
                    MapIdentifierToSymbolDefinition(&identifierNode, type);
                }
                break;
            }
            case cmajor::symbols::SymbolType::conceptGroupSymbol:
            {
                conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
                break;
            }
            case cmajor::symbols::SymbolType::namespaceSymbol:
            {
                cmajor::symbols::NamespaceSymbol* ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
                MapIdentifierToSymbolDefinition(&identifierNode, ns);
                NamespaceTypeSymbol* namespaceTypeSymbol = new NamespaceTypeSymbol(ns);
                boundCompileUnit.GetSymbolTable().SetTypeIdFor(namespaceTypeSymbol);
                namespaceTypeSymbols.push_back(std::unique_ptr<NamespaceTypeSymbol>(namespaceTypeSymbol));
                type = namespaceTypeSymbol;
                cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
                cmajor::ast::NamespaceImportNode importNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, ns->FullName()));
                fileScope->InstallNamespaceImport(containerScope, &importNode);
                boundCompileUnit.AddFileScope(fileScope);
                ++fileScopesAdded;
                break;
            }
            default:
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(symbol->FullName()) + "' does not denote a type or a concept", 
                    symbol->GetSourcePos(), symbol->SourceModuleId());
            }
            }
        }
    }
    else
    {
        throw cmajor::symbols::Exception("type or concept symbol '" + util::ToUtf8(name) + "' not found", identifierNode.GetSourcePos(), identifierNode.ModuleId());
    }
}

void ConstraintChecker::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    type = ResolveType(&templateIdNode, boundCompileUnit, containerScope);
}

void ConstraintChecker::Visit(cmajor::ast::DotNode& dotNode)
{
    Reset();
    dotNode.Subject()->Accept(*this);
    cmajor::symbols::TypeSymbol* subjectType = GetType();
    if (!subjectType)
    {
        throw cmajor::symbols::Exception("symbol '" + dotNode.Subject()->ToString() + "' does not denote a type", dotNode.Subject()->GetSourcePos(), dotNode.Subject()->ModuleId());
    }
    cmajor::symbols::Scope* typeContainerScope = nullptr;
    if (subjectType->IsPointerType())
    {
        typeContainerScope = subjectType->GetContainerScope();
    }
    else
    {
        typeContainerScope = subjectType->BaseType()->GetContainerScope();
    }
    if (subjectType->GetSymbolType() == cmajor::symbols::SymbolType::namespaceTypeSymbol)
    {
        NamespaceTypeSymbol* nsTypeSymbol = static_cast<NamespaceTypeSymbol*>(subjectType);
        typeContainerScope = nsTypeSymbol->Ns()->GetContainerScope();
    }
    Reset();
    const std::u32string& name = dotNode.MemberId()->Str();
    cmajor::symbols::Symbol* symbol = typeContainerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base);
    if (symbol)
    {
        switch (symbol->GetSymbolType())
        {
        case cmajor::symbols::SymbolType::aliasTypeSymbol:
        {
            cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::symbols::AliasTypeSymbol*>(symbol);
            type = aliasTypeSymbol->GetType();
            if (idNode && type)
            {
                MapIdentifierToSymbolDefinition(idNode, type);
            }
            break;
        }
        case cmajor::symbols::SymbolType::boundTemplateParameterSymbol:
        {
            cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameterSymbol = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(symbol);
            type = boundTemplateParameterSymbol->GetType();
            if (idNode && type)
            {
                MapIdentifierToSymbolDefinition(idNode, type);
            }
            break;
        }
        case cmajor::symbols::SymbolType::conceptGroupSymbol:
        {
            conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
            break;
        }
        case cmajor::symbols::SymbolType::classGroupTypeSymbol:
        {
            cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(symbol);
            type = classGroup->GetClass(0);
            if (idNode && type)
            {
                MapIdentifierToSymbolDefinition(idNode, type);
            }
            break;
        }
        case cmajor::symbols::SymbolType::namespaceSymbol:
        {
            cmajor::symbols::NamespaceSymbol* ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
            if (idNode)
            {
                MapIdentifierToSymbolDefinition(idNode, ns);
            }
            NamespaceTypeSymbol* namespaceTypeSymbol = new NamespaceTypeSymbol(ns);
            boundCompileUnit.GetSymbolTable().SetTypeIdFor(namespaceTypeSymbol);
            namespaceTypeSymbols.push_back(std::unique_ptr<NamespaceTypeSymbol>(namespaceTypeSymbol));
            type = namespaceTypeSymbol;
            cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
            cmajor::ast::NamespaceImportNode importNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, ns->FullName()));
            fileScope->InstallNamespaceImport(containerScope, &importNode);
            boundCompileUnit.AddFileScope(fileScope);
            ++fileScopesAdded;
            break;
        }
        default:
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(symbol->FullName()) + "' does not denote a type or a concept", symbol->GetSourcePos(), symbol->SourceModuleId());
        }
        }
    }
    else
    {
        throw cmajor::symbols::Exception("type or concept symbol '" + util::ToUtf8(name) + "' not found", dotNode.GetSourcePos(), dotNode.ModuleId());
    }
}

void ConstraintChecker::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    Reset();
    parenthesizedConstraintNode.Constraint()->Accept(*this);
}

void ConstraintChecker::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    bool exceptionWasSet = exception != nullptr;
    bool left = false;
    bool right = false;
    std::unique_ptr<BoundConstraint> leftBoundConstraint;
    std::unique_ptr<BoundConstraint> rightBoundConstraint;
    Reset();
    try
    {
        disjunctiveConstraintNode.Left()->Accept(*this);
        left = result;
        leftBoundConstraint = std::move(boundConstraint);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        left = false;
        leftBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
        if (!exception)
        {
            exception.reset(new cmajor::symbols::Exception(ex));
        }
    }
    catch (...)
    {
        left = false;
        leftBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
    }
    Reset();
    try
    {
        disjunctiveConstraintNode.Right()->Accept(*this);
        right = result;
        rightBoundConstraint = std::move(boundConstraint);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        right = false;
        rightBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
        if (!exception)
        {
            exception.reset(new cmajor::symbols::Exception(ex));
        }
    }
    catch (...)
    {
        right = false;
        rightBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
    }
    result = left || right;
    boundConstraint.reset(new BoundDisjunctiveConstraint(sourcePos, moduleId, leftBoundConstraint.release(), rightBoundConstraint.release()));
    if (result && !exceptionWasSet)
    {
        exception.reset();
    }
}

void ConstraintChecker::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    bool left = false;
    bool right = false;
    std::unique_ptr<BoundConstraint> leftBoundConstraint;
    std::unique_ptr<BoundConstraint> rightBoundConstraint;
    Reset();
    try
    {
        conjunctiveConstraintNode.Left()->Accept(*this);
        left = result;
        leftBoundConstraint = std::move(boundConstraint);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        left = false;
        leftBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
        if (!exception)
        {
            exception.reset(new cmajor::symbols::Exception(ex));
        }
    }
    catch (...)
    {
        left = false;
        leftBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
    }
    Reset();
    try
    {
        conjunctiveConstraintNode.Right()->Accept(*this);
        right = result;
        rightBoundConstraint = std::move(boundConstraint);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        right = false;
        rightBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
        if (!exception)
        {
            exception.reset(new cmajor::symbols::Exception(ex));
        }
    }
    catch (...)
    {
        right = false;
        rightBoundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
    }
    result = left && right;
    boundConstraint.reset(new BoundConjunctiveConstraint(sourcePos, moduleId, leftBoundConstraint.release(), rightBoundConstraint.release()));
}

void ConstraintChecker::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    Reset();
    whereConstraintNode.Constraint()->Accept(*this);
}

void ConstraintChecker::Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode)
{
    Reset();
    cmajor::ast::Node* invokeExprNode = predicateConstraintNode.InvokeExpr();
    std::unique_ptr<cmajor::symbols::Value> evaluationResult = Evaluate(invokeExprNode, symbolTable.GetTypeByName(U"bool"), containerScope, boundCompileUnit, false, currentFunction,
        predicateConstraintNode.GetSourcePos(), predicateConstraintNode.ModuleId());
    cmajor::symbols::BoolValue* boolResult = static_cast<cmajor::symbols::BoolValue*>(evaluationResult.get());
    result = boolResult->GetValue();
    boundConstraint.reset(new BoundAtomicConstraint(predicateConstraintNode.GetSourcePos(), predicateConstraintNode.ModuleId(), result));
}

void ConstraintChecker::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    Reset();
    isConstraintNode.TypeExpr()->Accept(*this);
    cmajor::symbols::TypeSymbol* leftType = GetType();
    if (!leftType)
    {
        throw cmajor::symbols::Exception("left operand of 'is' must be a type", isConstraintNode.TypeExpr()->GetSourcePos(), isConstraintNode.TypeExpr()->ModuleId());
    }
    Reset();
    isConstraintNode.ConceptOrTypeName()->Accept(*this);
    cmajor::symbols::TypeSymbol* rightType = GetType();
    if (rightType)
    {
        cmajor::symbols::TypeSymbol* leftPlainType = leftType->PlainType(sourcePos, moduleId);
        cmajor::symbols::TypeSymbol* rightPlainType = rightType->PlainType(sourcePos, moduleId);
        if (TypesEqual(leftPlainType, rightPlainType))
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
        else
        {
            result = false;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
        }
    }
    else if (conceptGroup)
    {
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(1);
        if (idNode && conceptSymbol)
        {
            MapIdentifierToSymbolDefinition(idNode, conceptSymbol);
        }
        std::vector<cmajor::symbols::TypeSymbol*> typeArguments;
        typeArguments.push_back(leftType);
        BoundConceptKey key(conceptSymbol, typeArguments);
        BoundConcept* boundConcept = boundCompileUnit.GetConceptRepository().GetBoundConcept(key);
        if (boundConcept)
        {
            result = true;
            boundConstraint.reset(boundConcept->GetBoundConstraint()->Clone());
            if (boundConcept->CommonType())
            {
                cmajor::symbols::BoundTemplateParameterSymbol* boundCommonTypeSymbol = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, U"CommonType");
                boundCommonTypeSymbol->SetType(boundConcept->CommonType());
                containerScope->Install(boundCommonTypeSymbol);
                boundConcept->AddBoundTemplateParameter(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundCommonTypeSymbol));
            }
        }
        else
        {
            std::unique_ptr<BoundConstraint> constraint;
            std::unique_ptr<BoundConcept> boundConcept = Instantiate(conceptSymbol, typeArguments, boundCompileUnit, containerScope, currentFunction, constraint, sourcePos, moduleId, exception);
            if (boundConcept)
            {
                result = true;
                boundConstraint.reset(constraint.release());
                boundCompileUnit.GetConceptRepository().AddBoundConcept(key, std::move(boundConcept));
            }
            else
            {
                result = false;
                boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
            }
        }
    }
}

void ConstraintChecker::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    Reset();
    multiParamConstraintNode.ConceptId()->Accept(*this);
    cmajor::ast::IdentifierNode* id1 = idNode;
    if (conceptGroup)
    {
        int n = multiParamConstraintNode.TypeExprs().Count();
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(n);
        if (id1 && conceptSymbol)
        {
            MapIdentifierToSymbolDefinition(id1, conceptSymbol);
        }
        std::vector<cmajor::symbols::TypeSymbol*> typeArguments;
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* typeExprNode = multiParamConstraintNode.TypeExprs()[i];
            Reset();
            typeExprNode->Accept(*this);
            cmajor::symbols::TypeSymbol* resolvedType = GetType();
            if (resolvedType)
            {
                typeArguments.push_back(resolvedType);
            }
            else
            {
                throw cmajor::symbols::Exception("type parameter '" + typeExprNode->ToString() + "' is not bound to a type", typeExprNode->GetSourcePos(), typeExprNode->ModuleId());
            }
        }
        BoundConceptKey key(conceptSymbol, typeArguments);
        BoundConcept* boundConcept = boundCompileUnit.GetConceptRepository().GetBoundConcept(key);
        if (boundConcept)
        {
            result = true;
            boundConstraint.reset(boundConcept->GetBoundConstraint()->Clone());
            if (boundConcept->CommonType())
            {
                cmajor::symbols::BoundTemplateParameterSymbol* boundCommonTypeSymbol = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, U"CommonType");
                boundCommonTypeSymbol->SetType(boundConcept->CommonType());
                containerScope->Install(boundCommonTypeSymbol);
                boundConcept->AddBoundTemplateParameter(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundCommonTypeSymbol));
            }
        }
        else
        {
            std::unique_ptr<BoundConstraint> constraint;
            std::unique_ptr<BoundConcept> boundConcept = Instantiate(conceptSymbol, typeArguments, boundCompileUnit, containerScope, currentFunction, constraint, sourcePos, moduleId, exception);
            if (boundConcept)
            {
                result = true;
                boundConstraint.reset(constraint.release());
                boundCompileUnit.GetConceptRepository().AddBoundConcept(key, std::move(boundConcept));
            }
            else
            {
                result = false;
                boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
            }
        }
    }
    else
    {
        throw cmajor::symbols::Exception("symbol '" + multiParamConstraintNode.ConceptId()->ToString() + "' does not denote a concept", multiParamConstraintNode.ConceptId()->GetSourcePos(), multiParamConstraintNode.ConceptId()->ModuleId());
    }
}

void ConstraintChecker::Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode)
{
    Reset();
    typeNameConstraintNode.TypeId()->Accept(*this);
    cmajor::symbols::TypeSymbol* resolvedType = GetType();
    result = resolvedType != nullptr;
    boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, result));
}

void ConstraintChecker::Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode)
{
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, firstTypeArgument->AddPointer(sourcePos, moduleId))));
    std::vector<cmajor::symbols::TypeSymbol*> parameterTypes;
    int n = constructorConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = constructorConstraintNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope);
        parameterTypes.push_back(parameterType);
        arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, parameterType)));
    }
    std::vector<FunctionScopeLookup> lookups;
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, firstTypeArgument->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        OverloadResolutionFlags::dontInstantiate | OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
    if (!constructorCall)
    {
        std::string signature;
        signature.append(util::ToUtf8(firstTypeArgument->FullName()) + "." + util::ToUtf8(firstTypeArgument->Name()).append(1, '('));
        bool first = true;
        for (cmajor::symbols::TypeSymbol* parameterType : parameterTypes)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                signature.append(", ");
            }
            signature.append(util::ToUtf8(parameterType->FullName()));
        }
        signature.append(1, ')');
        std::string message = "constructor signature '" + signature + "' not found";
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        if (exception)
        {
            message.append(": ").append(exception->Message());
            references.push_back(std::make_pair(exception->Defined(), exception->DefinedModuleId()));
            references.insert(references.end(), exception->References().begin(), exception->References().end());
        }
        throw cmajor::symbols::Exception(message, sourcePos, moduleId, references);
    }
    else
    {
        result = true;
        boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
    }
}

void ConstraintChecker::Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode)
{
    result = true;
    boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
}

void ConstraintChecker::Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode)
{
    Reset();
    memberFunctionConstraintNode.TypeParamId()->Accept(*this);
    cmajor::symbols::TypeSymbol* firstType = GetType();
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, firstType->AddPointer(sourcePos, moduleId))));
    std::vector<cmajor::symbols::TypeSymbol*> parameterTypes;
    int n = memberFunctionConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = memberFunctionConstraintNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope);
        parameterTypes.push_back(parameterType);
        arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, parameterType)));
    }
    std::vector<FunctionScopeLookup> lookups;
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, firstType->BaseType()->ClassInterfaceOrNsScope()));
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<BoundFunctionCall> memberFunctionCall = ResolveOverload(memberFunctionConstraintNode.GroupId(), containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        OverloadResolutionFlags::dontInstantiate | OverloadResolutionFlags::dontThrow | OverloadResolutionFlags::noTemplates, templateArgumentTypes, exception);
    if (!memberFunctionCall)
    {
        std::string signature;
        signature.append(util::ToUtf8(firstType->FullName()) + "." + util::ToUtf8(memberFunctionConstraintNode.GroupId()).append(1, '('));
        bool first = true;
        for (cmajor::symbols::TypeSymbol* parameterType : parameterTypes)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                signature.append(", ");
            }
            signature.append(util::ToUtf8(parameterType->FullName()));
        }
        signature.append(1, ')');
        std::string message = "member function signature '" + signature + "' not found";
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        if (exception)
        {
            message.append(": ").append(exception->Message());
            references.push_back(std::make_pair(exception->Defined(), exception->DefinedModuleId()));
            references.insert(references.end(), exception->References().begin(), exception->References().end());
        }
        throw cmajor::symbols::Exception(message, sourcePos, moduleId, references);
    }
    else
    {
        result = true;
        boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
    }
}

void ConstraintChecker::Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode)
{
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, firstTypeArgument->AddPointer(sourcePos, moduleId))));
    std::vector<cmajor::symbols::TypeSymbol*> parameterTypes;
    std::vector<FunctionScopeLookup> lookups;
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, firstTypeArgument->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
    int n = functionConstraintNode.Parameters().Count();
    if (firstTypeArgument->IsPointerType() &&
        ((n == 0 &&
            (functionConstraintNode.GroupId() == U"operator*" ||
                functionConstraintNode.GroupId() == U"operator++" ||
                functionConstraintNode.GroupId() == U"operator--")) ||
            (n == 1 &&
                functionConstraintNode.GroupId() == U"operator[]")))
    {
        result = true;
        boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        return;
    }
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = functionConstraintNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope);
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        parameterTypes.push_back(parameterType);
        arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, parameterType)));
    }
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<BoundFunctionCall> functionCall = ResolveOverload(functionConstraintNode.GroupId(), containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        OverloadResolutionFlags::dontInstantiate | OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
    if (!functionCall)
    {
        arguments.clear();
        parameterTypes.clear();
        arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, firstTypeArgument->AddPointer(sourcePos, moduleId))));
        std::vector<cmajor::symbols::TypeSymbol*> parameterTypes;
        int n = functionConstraintNode.Parameters().Count();
        for (int i = 1; i < n; ++i)
        {
            cmajor::ast::ParameterNode* parameterNode = functionConstraintNode.Parameters()[i];
            cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope);
            parameterTypes.push_back(parameterType);
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, parameterType)));
        }
        std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
        std::unique_ptr<cmajor::symbols::Exception> exception;
        std::unique_ptr<BoundFunctionCall> functionCall = ResolveOverload(functionConstraintNode.GroupId(), containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
            OverloadResolutionFlags::dontInstantiate | OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
        if (!functionCall)
        {
            arguments.clear();
            lookups.clear();
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            parameterTypes.clear();
            int n = functionConstraintNode.Parameters().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::ParameterNode* parameterNode = functionConstraintNode.Parameters()[i];
                cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope);
                lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, parameterType->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
                parameterTypes.push_back(parameterType);
                arguments.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(sourcePos, moduleId, parameterType)));
            }
            std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
            std::unique_ptr<cmajor::symbols::Exception> exception;
            std::unique_ptr<BoundFunctionCall> functionCall = ResolveOverload(functionConstraintNode.GroupId(), containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
                OverloadResolutionFlags::dontInstantiate | OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
            if (!functionCall)
            {
                std::string signature;
                signature.append(util::ToUtf8(functionConstraintNode.GroupId())).append(1, '(');
                bool first = true;
                for (cmajor::symbols::TypeSymbol* parameterType : parameterTypes)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        signature.append(", ");
                    }
                    signature.append(util::ToUtf8(parameterType->FullName()));
                }
                signature.append(1, ')');
                std::string message = "function signature '" + signature + "' not found";
                std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
                if (exception)
                {
                    message.append(": ").append(exception->Message());
                    references.push_back(std::make_pair(exception->Defined(), exception->DefinedModuleId()));
                    references.insert(references.end(), exception->References().begin(), exception->References().end());
                }
                throw cmajor::symbols::Exception(message, sourcePos, moduleId, references);
            }
            else
            {
                result = true;
                boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
            }
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        result = true;
        boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
    }
}

void ConstraintChecker::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    Reset();
    conceptIdNode.Id()->Accept(*this);
    if (conceptGroup)
    {
        int n = conceptIdNode.Arity();
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(n);
        if (idNode && conceptSymbol)
        {
            MapIdentifierToSymbolDefinition(idNode, conceptSymbol);
        }
        std::vector<cmajor::symbols::TypeSymbol*> typeArguments;
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* typeParameterNode = conceptIdNode.TypeParameters()[i];
            Reset();
            typeParameterNode->Accept(*this);
            cmajor::symbols::TypeSymbol* resolvedType = GetType();
            if (resolvedType)
            {
                typeArguments.push_back(resolvedType);
            }
            else
            {
                throw cmajor::symbols::Exception("type parameter " + std::to_string(i) + " does not denote a type", sourcePos, moduleId, conceptIdNode.GetSourcePos(), conceptIdNode.ModuleId());
            }
        }
        BoundConceptKey key(conceptSymbol, typeArguments);
        BoundConcept* boundConcept = boundCompileUnit.GetConceptRepository().GetBoundConcept(key);
        if (boundConcept)
        {
            result = true;
            boundConstraint = std::unique_ptr<BoundConstraint>(boundConcept->GetBoundConstraint()->Clone());
            if (boundConcept->CommonType())
            {
                cmajor::symbols::BoundTemplateParameterSymbol* boundCommonTypeSymbol = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, U"CommonType");
                boundCommonTypeSymbol->SetType(boundConcept->CommonType());
                containerScope->Install(boundCommonTypeSymbol);
                boundConcept->AddBoundTemplateParameter(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundCommonTypeSymbol));
            }
        }
        else
        {
            std::unique_ptr<BoundConstraint> constraint;
            std::unique_ptr<BoundConcept> boundConcept = Instantiate(conceptSymbol, typeArguments, boundCompileUnit, containerScope, currentFunction, constraint, sourcePos, moduleId, exception);
            if (boundConcept)
            {
                result = true;
                boundConstraint.reset(constraint.release());
                boundCompileUnit.GetConceptRepository().AddBoundConcept(key, std::move(boundConcept));
            }
            else
            {
                result = false;
                boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, false));
            }
        }
    }
    else
    {
        throw cmajor::symbols::Exception(conceptIdNode.Id()->ToString() + " does not denote a concept", conceptIdNode.Id()->GetSourcePos(), conceptIdNode.Id()->ModuleId());
    }
}

void ConstraintChecker::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    Reset();
    conceptNode.Id()->Accept(*this);
    if (!conceptGroup)
    {
        throw cmajor::symbols::Exception("symbol '" + conceptNode.Id()->ToString() + "' does not denote a concept", conceptNode.Id()->GetSourcePos(), conceptNode.Id()->ModuleId());
    }
    int arity = conceptNode.Arity();
    cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(arity);
    if (idNode && conceptSymbol)
    {
        MapIdentifierToSymbolDefinition(idNode, conceptSymbol);
    }
    if (conceptNode.Refinement())
    {
        Reset();
        conceptNode.Refinement()->Accept(*this);
        if (!result) return;
    }
    int n = conceptNode.Constraints().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ConstraintNode* constraintNode = conceptNode.Constraints()[i];
        Reset();
        constraintNode->Accept(*this);
        if (!result) return;
    }
    result = true;
    BoundAtomicConstraint* atomicConstraint = new BoundAtomicConstraint(sourcePos, moduleId, true);
    atomicConstraint->SetConcept(conceptSymbol);
    boundConstraint.reset(atomicConstraint);
}

void ConstraintChecker::Visit(cmajor::ast::SameConstraintNode& sameConstraintNode)
{
    if (firstTypeArgument && secondTypeArgument)
    {
        bool same = TypesEqual(firstTypeArgument, secondTypeArgument);
        if (!same)
        {
            throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is not same type as '" + 
                util::ToUtf8(secondTypeArgument->FullName()) + "'", sourcePos, moduleId);
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("the same type constraint needs two type arguments", sourcePos, moduleId);
    }
}

void ConstraintChecker::Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode)
{
    if (firstTypeArgument && secondTypeArgument)
    {
        bool derived = false;
        if (firstTypeArgument->IsClassTypeSymbol() && secondTypeArgument->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* firstClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstTypeArgument);
            cmajor::symbols::ClassTypeSymbol* secondClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(secondTypeArgument);
            derived = firstClassType->HasBaseClass(secondClassType);
        }
        if (!derived)
        {
            throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is not derived from '" + util::ToUtf8(secondTypeArgument->FullName()) + "'", sourcePos, moduleId);
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("the derivded type constraint needs two type arguments", sourcePos, moduleId);
    }
}

void ConstraintChecker::Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode)
{
    if (firstTypeArgument && secondTypeArgument)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = boundCompileUnit.GetConversion(firstTypeArgument, secondTypeArgument, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
        if (!conversion || conversion->GetConversionType() == cmajor::symbols::ConversionType::explicit_)
        {
            throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is not implicitly convertible to '" + 
                util::ToUtf8(secondTypeArgument->FullName()) + "'", sourcePos, moduleId);
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("the convertible constraint needs two type arguments", sourcePos, moduleId);
    }
}

void ConstraintChecker::Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode)
{
    if (firstTypeArgument && secondTypeArgument)
    {
        ArgumentMatch argumentMatch;
        cmajor::symbols::FunctionSymbol* conversion = boundCompileUnit.GetConversion(firstTypeArgument, secondTypeArgument, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
        if (!conversion || conversion->GetConversionType() != cmajor::symbols::ConversionType::explicit_)
        {
            throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is not explicitly convertible to '" + 
                util::ToUtf8(secondTypeArgument->FullName()) + "'", sourcePos, moduleId);
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("the explicitly convertible constraint needs two type arguments", sourcePos, moduleId);
    }
}

void ConstraintChecker::Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode)
{
    if (firstTypeArgument && secondTypeArgument)
    {
        cmajor::symbols::BoundTemplateParameterSymbol* commonType = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, U"CommonType");
        bool same = cmajor::symbols::TypesEqual(firstTypeArgument, secondTypeArgument);
        if (same)
        {
            commonType->SetType(firstTypeArgument);
        }
        else
        {
            ArgumentMatch argumentMatch;
            cmajor::symbols::FunctionSymbol* conversion = boundCompileUnit.GetConversion(firstTypeArgument, secondTypeArgument, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (conversion && conversion->GetConversionType() == cmajor::symbols::ConversionType::implicit_)
            {
                commonType->SetType(secondTypeArgument);
            }
            else
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* conversion = boundCompileUnit.GetConversion(secondTypeArgument, firstTypeArgument, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (conversion && conversion->GetConversionType() == cmajor::symbols::ConversionType::implicit_)
                {
                    commonType->SetType(firstTypeArgument);
                }
                else
                {
                    throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is not same or convertible to type '" + 
                        util::ToUtf8(secondTypeArgument->FullName()) + "' or vice versa", sourcePos, moduleId);
                }
            }
        }
        containerScope->Install(commonType);
        boundTemplateParameters.push_back(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(commonType));
        result = true;
        boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
    }
    else
    {
        throw cmajor::symbols::Exception("the common constraint needs two type arguments", sourcePos, moduleId);
    }
}

void ConstraintChecker::Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode)
{
    if (firstTypeArgument)
    {
        bool referenceType = firstTypeArgument->IsReferenceType();
        if (referenceType)
        {
            throw cmajor::symbols::Exception("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' is a reference type", sourcePos, moduleId);
        }
        else
        {
            result = true;
            boundConstraint.reset(new BoundAtomicConstraint(sourcePos, moduleId, true));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("the nonreference type constraint needs one type argument", sourcePos, moduleId);
    }
}

std::unique_ptr<BoundConcept> Instantiate(cmajor::symbols::ConceptSymbol* conceptSymbol, const std::vector<cmajor::symbols::TypeSymbol*>& typeArguments, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope,
    BoundFunction* currentFunction, std::unique_ptr<BoundConstraint>& boundConstraint, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, 
    std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    cmajor::ast::ConceptNode* conceptNode = conceptSymbol->GetConceptNode();
    if (!conceptNode)
    {
        cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNode(conceptSymbol);
        Assert(node->IsConceptNode(), "concept node expected"); 
        conceptNode = static_cast<cmajor::ast::ConceptNode*>(node);
    }
    int n = conceptSymbol->Arity();
    if (n != typeArguments.size())
    {
        throw cmajor::symbols::Exception("number of type arguments does not match number of template parameters of concept symbol", sourcePos, moduleId, conceptSymbol->GetSourcePos(), conceptSymbol->SourceModuleId());
    }
    cmajor::symbols::ContainerScope instantiationScope;
    instantiationScope.SetParentScope(containerScope);
    std::vector<std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>> boundTemplateParameters;
    cmajor::symbols::TypeSymbol* firstTypeArgument = nullptr;
    cmajor::symbols::TypeSymbol* secondTypeArgument = nullptr;
    for (int i = 0; i < n; ++i)
    {
        cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = conceptSymbol->TemplateParameters()[i];
        cmajor::symbols::TypeSymbol* typeArgument = typeArguments[i];
        if (typeArgument->RemoveConst(sourcePos, moduleId)->IsBasicTypeSymbol())
        {
            typeArgument = typeArgument->RemoveConst(sourcePos, moduleId);
        }
        if (i == 0)
        {
            firstTypeArgument = typeArgument;
        }
        else if (i == 1)
        {
            secondTypeArgument = typeArgument;
        }
        cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameter = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, templateParameterSymbol->Name());
        boundTemplateParameter->SetType(typeArgument);
        boundTemplateParameters.push_back(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundTemplateParameter));
        instantiationScope.Install(boundTemplateParameter);
    }
    ConstraintChecker checker(firstTypeArgument, secondTypeArgument, boundCompileUnit, &instantiationScope, currentFunction, sourcePos, moduleId, exception);
    try
    {
        conceptNode->Accept(checker);
        bool result = checker.Result();
        boundConstraint = std::move(checker.GetBoundConstraint());
        if (result)
        {
            BoundConcept* boundConcept = new BoundConcept(conceptSymbol, typeArguments, sourcePos, moduleId);
            boundConcept->SetBoundConstraint(std::unique_ptr<BoundConstraint>(boundConstraint->Clone()));
            cmajor::symbols::Symbol* commonTypeSymbol = instantiationScope.Lookup(U"CommonType");
            if (commonTypeSymbol)
            {
                if (commonTypeSymbol->GetSymbolType() != cmajor::symbols::SymbolType::boundTemplateParameterSymbol)
                {
                    throw cmajor::symbols::Exception("'CommonType' symbol found from concept instantiation scope is not bound template parameter", sourcePos, moduleId, commonTypeSymbol->GetSourcePos(), commonTypeSymbol->SourceModuleId());
                }
                cmajor::symbols::BoundTemplateParameterSymbol* commonType = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(commonTypeSymbol);
                cmajor::symbols::BoundTemplateParameterSymbol* commonTypeClone = new cmajor::symbols::BoundTemplateParameterSymbol(sourcePos, moduleId, U"CommonType");
                commonTypeClone->SetType(commonType->GetType());
                boundConcept->AddBoundTemplateParameter(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(commonTypeClone));
                containerScope->Install(commonTypeClone);
                boundConcept->SetCommonType(commonType->GetType());
            }
            return std::unique_ptr<BoundConcept>(boundConcept);
        }
        else
        {
            return std::unique_ptr<BoundConcept>(nullptr);
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::string message;
        if (typeArguments.size() == 1)
        {
            message.append("type '" + util::ToUtf8(firstTypeArgument->FullName()) + "' does not fulfill the requirements of concept '");
        }
        else
        {
            message.append("types (");
            bool first = true;
            for (cmajor::symbols::TypeSymbol* typeArgument : typeArguments)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    message.append(", ");
                }
                message.append("'" + util::ToUtf8(typeArgument->FullName()) + "'");
            }
            message.append(") do not fulfill the requirements of concept '");
        }
        message.append(util::ToUtf8(conceptSymbol->FullName())).append("' because:\n");
        message.append(ex.Message());
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(conceptSymbol->GetSourcePos(), conceptSymbol->SourceModuleId()));
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception(message, sourcePos, moduleId, references);
    }
}

bool CheckConstraint(cmajor::ast::ConstraintNode* constraint, const cmajor::ast::NodeList<cmajor::ast::Node>& usingNodes, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    const std::vector<cmajor::symbols::TemplateParameterSymbol*>& templateParameters, 
    const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMap,
    std::unique_ptr<BoundConstraint>& boundConstraint, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, cmajor::symbols::FunctionSymbol* viableFunction, 
    std::unique_ptr<cmajor::symbols::Exception>& conceptCheckException)
{
    bool fileScopeAdded = false;
    try
    {
        std::unique_ptr<cmajor::symbols::FileScope> fileScope(new cmajor::symbols::FileScope());
        int nu = usingNodes.Count();
        for (int i = 0; i < nu; ++i)
        {
            cmajor::ast::Node* usingNode = usingNodes[i];
            switch (usingNode->GetNodeType())
            {
            case cmajor::ast::NodeType::aliasNode:
            {
                cmajor::ast::AliasNode* aliasNode = static_cast<cmajor::ast::AliasNode*>(usingNode);
                cmajor::symbols::TypeSymbol* type = ResolveType(aliasNode, boundCompileUnit, containerScope);
                fileScope->InstallAlias(aliasNode, type); 
                break;
            }
            case cmajor::ast::NodeType::namespaceImportNode:
            {
                cmajor::ast::NamespaceImportNode* importNode = static_cast<cmajor::ast::NamespaceImportNode*>(usingNode);
                fileScope->InstallNamespaceImport(containerScope, importNode);
                break;
            }
            default:
            {
                throw cmajor::symbols::Exception("unknown using node type", usingNode->GetSourcePos(), usingNode->ModuleId());
            }
            }
        }
        cmajor::symbols::ContainerScope constraintCheckScope;
        constraintCheckScope.SetParentScope(containerScope);
        std::vector<std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>> boundTemplateParameters;
        cmajor::symbols::TypeSymbol* firstTypeArgument = nullptr;
        cmajor::symbols::TypeSymbol* secondTypeArgument = nullptr;
        int n = templateParameters.size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = templateParameters[i];
            auto it = templateParameterMap.find(templateParameterSymbol);
            if (it != templateParameterMap.cend())
            {
                cmajor::symbols::TypeSymbol* templateArgumentType = it->second;
                if (i == 0)
                {
                    firstTypeArgument = templateArgumentType;
                }
                else if (i == 1)
                {
                    secondTypeArgument = templateArgumentType;
                }
                cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameterSymbol = new cmajor::symbols::BoundTemplateParameterSymbol(
                    sourcePos, moduleId, templateParameterSymbol->Name());
                boundTemplateParameterSymbol->SetType(templateArgumentType);
                boundTemplateParameters.push_back(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundTemplateParameterSymbol));
                constraintCheckScope.Install(boundTemplateParameterSymbol);
            }
            else
            {
                throw cmajor::symbols::Exception("template parameter symbol '" + util::ToUtf8(templateParameterSymbol->Name()) + 
                    "' not found from template parameter map", sourcePos, moduleId, viableFunction->GetSourcePos(), viableFunction->SourceModuleId());
            }
        }
        boundCompileUnit.AddFileScope(fileScope.release());
        fileScopeAdded = true;
        ConstraintChecker constraintChecker(firstTypeArgument, secondTypeArgument, boundCompileUnit, &constraintCheckScope, currentFunction, sourcePos, moduleId, conceptCheckException);
        constraint->Accept(constraintChecker);
        boundCompileUnit.RemoveLastFileScope();
        bool result = constraintChecker.Result();
        boundConstraint = std::move(constraintChecker.GetBoundConstraint());
        return result;
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        if (fileScopeAdded)
        {
            boundCompileUnit.RemoveLastFileScope();
        }
        conceptCheckException.reset(new cmajor::symbols::Exception(ex));
        return false;
    }
}

} // namespace cmajor::binder
