// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.type.resolver;


import cmajor.binder.type.binder;
import cmajor.binder.evaluator;
import cmajor.binder.class_template_repository;
import cmajor.binder.bound.compile.unit;
import util;

namespace cmajor::binder {

NamespaceTypeSymbol::NamespaceTypeSymbol(cmajor::symbols::NamespaceSymbol* ns_) : cmajor::symbols::TypeSymbol(cmajor::symbols::SymbolType::namespaceTypeSymbol, ns_->GetSourcePos(), ns_->SourceModuleId(), ns_->Name()), ns(ns_)
{
}

class TypeResolver : public cmajor::ast::Visitor
{
public:
    TypeResolver(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_, TypeResolverFlags flags_, cmajor::symbols::ClassTypeSymbol* currentClass_);
    cmajor::symbols::TypeSymbol* GetType() { return type; }
    const cmajor::symbols::TypeDerivationRec& DerivationRec() const { return derivationRec; }
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
private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::Module* module;
    cmajor::symbols::ContainerScope* containerScope;
    ClassTemplateRepository& classTemplateRepository;
    cmajor::symbols::TypeSymbol* type;
    cmajor::symbols::TypeDerivationRec derivationRec;
    std::unique_ptr<NamespaceTypeSymbol> nsTypeSymbol;
    TypeResolverFlags flags;
    cmajor::symbols::ClassTypeSymbol* currentClass;
    void ResolveSymbol(cmajor::ast::Node& node, cmajor::ast::IdentifierNode* idNode, cmajor::symbols::Symbol* symbol);
};

TypeResolver::TypeResolver(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_, TypeResolverFlags flags_, cmajor::symbols::ClassTypeSymbol* currentClass_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()), module(&boundCompileUnit.GetModule()),
    classTemplateRepository(boundCompileUnit.GetClassTemplateRepository()), containerScope(containerScope_),
    type(nullptr), derivationRec(), nsTypeSymbol(), flags(flags_), currentClass(currentClass_)
{
}

void TypeResolver::Visit(cmajor::ast::BoolNode& boolNode)
{
    type = symbolTable.GetTypeByName(U"bool");
}

void TypeResolver::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    type = symbolTable.GetTypeByName(U"sbyte");
}

void TypeResolver::Visit(cmajor::ast::ByteNode& byteNode)
{
    type = symbolTable.GetTypeByName(U"byte");
}

void TypeResolver::Visit(cmajor::ast::ShortNode& shortNode)
{
    type = symbolTable.GetTypeByName(U"short");
}

void TypeResolver::Visit(cmajor::ast::UShortNode& ushortNode)
{
    type = symbolTable.GetTypeByName(U"ushort");
}

void TypeResolver::Visit(cmajor::ast::IntNode& intNode)
{
    type = symbolTable.GetTypeByName(U"int");
}

void TypeResolver::Visit(cmajor::ast::UIntNode& uintNode)
{
    type = symbolTable.GetTypeByName(U"uint");
}

void TypeResolver::Visit(cmajor::ast::LongNode& longNode)
{
    type = symbolTable.GetTypeByName(U"long");
}

void TypeResolver::Visit(cmajor::ast::ULongNode& ulongNode)
{
    type = symbolTable.GetTypeByName(U"ulong");
}

void TypeResolver::Visit(cmajor::ast::FloatNode& floatNode)
{
    type = symbolTable.GetTypeByName(U"float");
}

void TypeResolver::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    type = symbolTable.GetTypeByName(U"double");
}

void TypeResolver::Visit(cmajor::ast::CharNode& charNode)
{
    type = symbolTable.GetTypeByName(U"char");
}

void TypeResolver::Visit(cmajor::ast::WCharNode& wcharNode)
{
    type = symbolTable.GetTypeByName(U"wchar");
}

void TypeResolver::Visit(cmajor::ast::UCharNode& ucharNode)
{
    type = symbolTable.GetTypeByName(U"uchar");
}

void TypeResolver::Visit(cmajor::ast::VoidNode& voidNode)
{
    type = symbolTable.GetTypeByName(U"void");
}

void TypeResolver::Visit(cmajor::ast::ConstNode& constNode)
{
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::constDerivation);
    constNode.Subject()->Accept(*this);
}

void TypeResolver::Visit(cmajor::ast::LValueRefNode& lvalueRefNode)
{
    lvalueRefNode.Subject()->Accept(*this);
    if (HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have reference to reference type", lvalueRefNode.GetSourcePos(), lvalueRefNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::lvalueRefDerivation);
}

void TypeResolver::Visit(cmajor::ast::RValueRefNode& rvalueRefNode)
{
    rvalueRefNode.Subject()->Accept(*this);
    if (HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have reference to reference type", rvalueRefNode.GetSourcePos(), rvalueRefNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::rvalueRefDerivation);
}

void TypeResolver::Visit(cmajor::ast::PointerNode& pointerNode)
{
    pointerNode.Subject()->Accept(*this);
    if (HasReferenceDerivation(derivationRec.derivations))
    {
        throw cmajor::symbols::Exception("cannot have pointer to reference type", pointerNode.GetSourcePos(), pointerNode.ModuleId());
    }
    derivationRec.derivations.push_back(cmajor::symbols::Derivation::pointerDerivation);
}

void TypeResolver::Visit(cmajor::ast::ArrayNode& arrayNode)
{
    type = ResolveType(arrayNode.Subject(), boundCompileUnit, containerScope, currentClass);
    if (type->IsReferenceType())
    {
        throw cmajor::symbols::Exception("cannot have array of reference type", arrayNode.GetSourcePos(), arrayNode.ModuleId());
    }
    int64_t size = -1;
    if (arrayNode.Size())
    {
        std::unique_ptr<cmajor::symbols::Value> sizeValue = Evaluate(arrayNode.Size(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"long"), containerScope, boundCompileUnit, false, nullptr,
            arrayNode.GetSourcePos(), arrayNode.ModuleId());
        if (sizeValue->GetValueType() == cmajor::symbols::ValueType::longValue)
        {
            cmajor::symbols::LongValue* longSizeValue = static_cast<cmajor::symbols::LongValue*>(sizeValue.get());
            size = longSizeValue->GetValue();
        }
        else
        {
            throw cmajor::symbols::Exception("long type value expected ", arrayNode.Size()->GetSourcePos(), arrayNode.Size()->ModuleId());
        }
    }
    type = symbolTable.MakeArrayType(type, size, arrayNode.GetSourcePos(), arrayNode.ModuleId());
}

void TypeResolver::ResolveSymbol(cmajor::ast::Node& node, cmajor::ast::IdentifierNode* idNode, cmajor::symbols::Symbol* symbol)
{
    if (symbol->IsTypeSymbol())
    {
        type = static_cast<cmajor::symbols::TypeSymbol*>(symbol);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            cmajor::symbols::TypeSymbol* cmdocType = type;
            if (type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
            {
                cmajor::symbols::ClassGroupTypeSymbol* group = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
                cmdocType = group->GetClass(0);
            }
            symbolTable.MapSymbol(idNode, cmdocType);
        }
        cmajor::symbols::TypeSymbol* definitionType = type;
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
        {
            cmajor::symbols::ClassGroupTypeSymbol* group = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
            if (group->GetClass(0))
            {
                definitionType = group->GetClass(0);
            }
        }
        cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, definitionType);
    }
    else
    {
        switch (symbol->GetSymbolType())
        {
        case cmajor::symbols::SymbolType::typedefSymbol:
        {
            cmajor::symbols::TypedefSymbol* typedefSymbol = static_cast<cmajor::symbols::TypedefSymbol*>(symbol);
            if (typedefSymbol->IsProject() && !typedefSymbol->IsBound())
            {
                TypeBinder typeBinder(boundCompileUnit);
                typeBinder.SetContainerScope(typedefSymbol->Parent()->GetContainerScope());
                cmajor::ast::Node* node = symbolTable.GetNode(typedefSymbol);
                // Assert(node->GetNodeType() == cmajor::ast::NodeType::typedefNode, "typedef node expected"); TODO
                cmajor::ast::TypedefNode* typedefNode = static_cast<cmajor::ast::TypedefNode*>(node);
                typeBinder.BindTypedef(typedefSymbol, typedefNode, false);
            }
            type = typedefSymbol->GetType();
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
            {
                symbolTable.MapSymbol(idNode, typedefSymbol);
            }
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, typedefSymbol);
            break;
        }
        case cmajor::symbols::SymbolType::boundTemplateParameterSymbol:
        {
            cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameterSymbol = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(symbol);
            type = boundTemplateParameterSymbol->GetType();
            break;
        }
        case cmajor::symbols::SymbolType::namespaceSymbol:
        {
            cmajor::symbols::NamespaceSymbol* ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
            nsTypeSymbol.reset(new NamespaceTypeSymbol(ns));
            symbolTable.SetTypeIdFor(nsTypeSymbol.get());
            type = nsTypeSymbol.get();
            break;
        }
        default:
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(symbol->FullName()) + "' does not denote a type", node.GetSourcePos(), node.ModuleId(), symbol->GetSourcePos(), symbol->SourceModuleId());
        }
        }
    }
}

void TypeResolver::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    boundCompileUnit.SetLatestIdentifier(&identifierNode);
    std::u32string name = identifierNode.Str();
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
        ResolveSymbol(identifierNode, &identifierNode, symbol);
    }
    else
    {
        throw cmajor::symbols::Exception("type symbol '" + util::ToUtf8(name) + "' not found", identifierNode.GetSourcePos(), identifierNode.ModuleId());
    }
}

void TypeResolver::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    cmajor::ast::IdentifierNode* prevId = boundCompileUnit.GetLatestIdentifier();
    boundCompileUnit.SetLatestIdentifier(nullptr);
    int arity = templateIdNode.TemplateArguments().Count();
    cmajor::symbols::TypeSymbol* primaryTemplateType = ResolveType(templateIdNode.Primary(), boundCompileUnit, containerScope, TypeResolverFlags::resolveClassGroup, currentClass);
    if (primaryTemplateType->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(primaryTemplateType);
        cmajor::symbols::ClassTypeSymbol* classType = classGroup->GetClass(arity);
        if (classType)
        {
            primaryTemplateType = classType;
        }
        else
        {
            throw cmajor::symbols::Exception("primary class template with arity '" + std::to_string(arity) + "' not found", classGroup->GetSourcePos(), classGroup->SourceModuleId());
        }
    }
    if (!primaryTemplateType->IsClassTypeSymbol())
    {
        throw cmajor::symbols::Exception("class type symbol expected", templateIdNode.Primary()->GetSourcePos(), templateIdNode.Primary()->ModuleId());
    }
    cmajor::symbols::ClassTypeSymbol* classTemplate = static_cast<cmajor::symbols::ClassTypeSymbol*>(primaryTemplateType);
    if (!classTemplate->IsClassTemplate())
    {
        throw cmajor::symbols::Exception("class template expected", templateIdNode.Primary()->GetSourcePos(), templateIdNode.Primary()->ModuleId());
    }
    cmajor::ast::IdentifierNode* idNode = boundCompileUnit.GetLatestIdentifier();
    boundCompileUnit.SetLatestIdentifier(prevId);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(idNode, classTemplate);
    }
    if (idNode)
    {
        cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, classTemplate);
    }
    if (classTemplate->IsProject() && !classTemplate->IsBound())
    {
        TypeBinder typeBinder(boundCompileUnit);
        cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(classTemplate);
        if (node && node->GetNodeType() == cmajor::ast::NodeType::classNode)
        {
            typeBinder.AddUsingNodesToCurrentCompileUnit(static_cast<cmajor::ast::ClassNode*>(node));
        }
    }
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    int n = arity;
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::IdentifierNode* prevId = boundCompileUnit.GetLatestIdentifier();
        boundCompileUnit.SetLatestIdentifier(nullptr);
        cmajor::symbols::TypeSymbol* templateArgumentType = ResolveType(templateIdNode.TemplateArguments()[i], boundCompileUnit, containerScope, currentClass);
        templateArgumentTypes.push_back(templateArgumentType);
        cmajor::ast::IdentifierNode* idNode = boundCompileUnit.GetLatestIdentifier();
        if (idNode && cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, templateArgumentType);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, templateArgumentType);
        }
        boundCompileUnit.SetLatestIdentifier(prevId);
    }
    int m = classTemplate->TemplateParameters().size();
    if (n < m)
    {
        classTemplateRepository.ResolveDefaultTemplateArguments(templateArgumentTypes, classTemplate, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization = symbolTable.MakeClassTemplateSpecialization(classTemplate, templateArgumentTypes,
        templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
    if (classTemplateSpecialization->GetModule()->IsImmutable())
    {
        std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().Lock());
        classTemplateSpecialization = boundCompileUnit.GetSymbolTable().CopyClassTemplateSpecialization(classTemplateSpecialization);
    }
    if (!classTemplateSpecialization->IsBound())
    {
        classTemplateRepository.BindClassTemplateSpecialization(classTemplateSpecialization, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
    }
    type = classTemplateSpecialization;
}

void TypeResolver::Visit(cmajor::ast::DotNode& dotNode)
{
    dotNode.Subject()->Accept(*this);
    cmajor::symbols::Scope* scope = nullptr;
    if (type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
        if (currentClass && classGroup->Name() == currentClass->Name())
        {
            type = currentClass;
        }
        else
        {
            type = classGroup->GetClass(0);
        }
        if (!type)
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(type->FullName()) + "' does not denote a class type, an array type or a namespace", dotNode.GetSourcePos(), dotNode.ModuleId(),
                type->GetSourcePos(), type->SourceModuleId());
        }
    }
    if (type->GetSymbolType() == cmajor::symbols::SymbolType::namespaceTypeSymbol)
    {
        NamespaceTypeSymbol* nsType = static_cast<NamespaceTypeSymbol*>(type);
        scope = nsType->Ns()->GetContainerScope();
    }
    else if (type->IsClassTypeSymbol() ||
        type->IsArrayType() ||
        type->IsEnumeratedType() ||
        (flags & TypeResolverFlags::createMemberSymbols) != TypeResolverFlags::none && type->GetSymbolType() == cmajor::symbols::SymbolType::templateParameterSymbol)
    {
        scope = type->GetContainerScope();
    }
    else
    {
        throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(type->FullName()) + "' does not denote a class type, an array type or a namespace", dotNode.GetSourcePos(), dotNode.ModuleId(),
            type->GetSourcePos(), type->SourceModuleId());
    }
    std::u32string name = dotNode.MemberId()->Str();
    cmajor::symbols::Symbol* symbol = scope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base);
    if (symbol)
    {
        ResolveSymbol(dotNode, dotNode.MemberId(), symbol);
    }
    else
    {
        if ((flags & TypeResolverFlags::createMemberSymbols) != TypeResolverFlags::none && type->GetSymbolType() == cmajor::symbols::SymbolType::templateParameterSymbol)
        {
            cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = new cmajor::symbols::TemplateParameterSymbol(dotNode.GetSourcePos(), dotNode.ModuleId(), name);
            templateParameterSymbol->SetModule(module);
            symbolTable.SetTypeIdFor(templateParameterSymbol);
            type->AddMember(templateParameterSymbol);
            ResolveSymbol(dotNode, dotNode.MemberId(), templateParameterSymbol);
        }
        else
        {
            throw cmajor::symbols::Exception("type symbol '" + util::ToUtf8(name) + "' not found", dotNode.GetSourcePos(), dotNode.ModuleId());
        }
    }
}

cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    return ResolveType(typeExprNode, boundCompileUnit, containerScope, TypeResolverFlags::none, nullptr);
}

cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, cmajor::symbols::ClassTypeSymbol* currentClass)
{
    return ResolveType(typeExprNode, boundCompileUnit, containerScope, TypeResolverFlags::none, currentClass);
}

cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, TypeResolverFlags flags)
{
    return ResolveType(typeExprNode, boundCompileUnit, containerScope, flags, nullptr);
}

cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, TypeResolverFlags flags, cmajor::symbols::ClassTypeSymbol* currentClass)
{
    cmajor::symbols::Module* module = &boundCompileUnit.GetModule();
    bool resolveClassGroup = (flags & TypeResolverFlags::resolveClassGroup) != TypeResolverFlags::none;
    TypeResolver typeResolver(boundCompileUnit, containerScope, flags, currentClass);
    typeExprNode->Accept(typeResolver);
    cmajor::symbols::TypeSymbol* type = typeResolver.GetType();
    if (resolveClassGroup && type && type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        return type;
    }
    if (type && type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
        if (currentClass && classGroup->Name() == currentClass->GroupName())
        {
            type = currentClass;
        }
        else
        {
            type = classGroup->GetClass(0);
        }
    }
    if (!type || type->IsInComplete())
    {
        throw cmajor::symbols::Exception("incomplete type expression", typeExprNode->GetSourcePos(), typeExprNode->ModuleId());
    }
    cmajor::symbols::TypeDerivationRec derivationRec = UnifyDerivations(typeResolver.DerivationRec(), type->DerivationRec());
    if (!derivationRec.derivations.empty())
    {
        return boundCompileUnit.GetSymbolTable().MakeDerivedType(type->BaseType(), derivationRec, typeExprNode->GetSourcePos(), typeExprNode->ModuleId());
    }
    return type;
}

} // namespace cmajor::binder
