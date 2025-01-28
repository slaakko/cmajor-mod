// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol.table;

import cmajor.fault.tolerant.symbols.alias.type.symbol;
import cmajor.fault.tolerant.symbols.constant.symbol;
import cmajor.fault.tolerant.symbols.delegate.symbol;
import cmajor.fault.tolerant.symbols.enum_symbol;
import cmajor.fault.tolerant.symbols.variable.symbol;
import cmajor.fault.tolerant.symbols.template_symbol;
import cmajor.fault.tolerant.symbols.function.symbol;
import cmajor.fault.tolerant.symbols.class_symbol;
import cmajor.fault.tolerant.symbols.interface.symbol;
import cmajor.fault.tolerant.symbols.concept_symbol;
import cmajor.fault.tolerant.symbols.source;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.basic.type.symbol;
import cmajor.fault.tolerant.symbols.compound.type.symbol;
import cmajor.fault.tolerant.symbols.array.type.symbol;
import cmajor.fault.tolerant.symbols.namespace_symbol;
import cmajor.fault.tolerant.symbols.declaration.block.symbol;
import util;

namespace cmajor::fault::tolerant::symbols {

SymbolTable::SymbolTable() : globalNs(), container(&globalNs)
{
    globalNs.SetSymbolTable(this);
}

void SymbolTable::Import(const SymbolTable& that)
{
    if (that.name == "System.Core")
    {
        basicTypeMap = that.basicTypeMap;
        for (const auto& p : basicTypeMap)
        {
            BasicTypeSymbol* basicTypeSymbol = p.second;
            MapType(basicTypeSymbol);
        }
    }
    globalNs.Import(&that.globalNs, this);
    for (const auto& compoundTypePair : that.compoundTypeMap)
    {
        compoundTypeMap[compoundTypePair.first] = compoundTypePair.second;
    }
    for (auto arrayType : that.arrayTypeSet)
    {
        arrayTypeSet.insert(arrayType);
    }
    for (const auto& typePair : that.typeMap)
    {
        typeMap[typePair.first] = typePair.second;
    }
}

void SymbolTable::Write(SymbolWriter& writer)
{
    writer.BinaryStreamWriter().Write(name);
    int32_t n = sources.size();
    writer.BinaryStreamWriter().Write(n);
    for (const auto& source : sources)
    {
        source->Write(writer);
    }
    globalNs.Write(writer);
    int32_t nc = compoundTypes.size();
    writer.BinaryStreamWriter().Write(nc);
    for (const auto& compoundType : compoundTypes)
    {
        writer.Write(compoundType.get());
    }
    int32_t na = arrayTypes.size();
    writer.BinaryStreamWriter().Write(na);
    for (const auto& arrayType : arrayTypes)
    {
        writer.Write(arrayType.get());
    }
}

void SymbolTable::Read(SymbolReader& reader)
{
    reader.SetSymbolTable(this);
    name = reader.BinaryStreamReader().ReadUtf8String();
    int32_t n = reader.BinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        std::unique_ptr<Source> source(new Source());
        source->Read(reader);
        AddSource(source.release());
    }
    globalNs.Read(reader);
    int32_t nc = reader.BinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < nc; ++i)
    {
        Symbol* symbol = reader.ReadSymbol();
        if (symbol->IsCompoundTypeSymbol())
        {
            CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(symbol);
            MapType(compoundTypeSymbol);
            compoundTypes.push_back(std::unique_ptr<CompoundTypeSymbol>(compoundTypeSymbol));
        }
    }
    int32_t na = reader.BinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < na; ++i)
    {
        Symbol* symbol = reader.ReadSymbol();
        if (symbol->IsArrayTypeSymbol())
        {
            ArrayTypeSymbol* arrayTypeSymbol = static_cast<ArrayTypeSymbol*>(symbol);
            MapType(arrayTypeSymbol);
            arrayTypes.push_back(std::unique_ptr<ArrayTypeSymbol>(arrayTypeSymbol));
        }
    }
}

void SymbolTable::AddSource(Source* source)
{
    source->SetIndex(sources.size());
    sources.push_back(std::unique_ptr<Source>(source));
    sourceMap[source->FilePath()] = source;
}

Source* SymbolTable::GetSource(int index) const
{
    if (index >= 0 && index < sources.size())
    {
        return sources[index].get();
    }
    else
    {
        throw std::runtime_error("SymbolTable '" + name + "': invalid source index");
    }
}

void SymbolTable::ResetSource(int index, Source* source)
{
    if (index >= 0 && index < sources.size())
    {
        sources[index].reset(source);
        source->SetIndex(index);
    }
    else
    {
        throw std::runtime_error("SymbolTable '" + name + "': invalid source index");
    }
}

Source* SymbolTable::GetSource(const std::string& filePath) const
{
    auto it = sourceMap.find(filePath);
    if (it != sourceMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::BeginContainer(ContainerSymbol* container_)
{
    containerStack.push(container);
    container = container_;
}

void SymbolTable::EndContainer()
{
    container = containerStack.top();
    containerStack.pop();
}

bool SymbolTable::BeginNamespace(cmajor::fault::tolerant::ast::IdentifierNode* namespaceName, Source* source, ErrorContainer* errors, bool map)
{
    if (namespaceName->Str().empty())
    {
        BeginContainer(&globalNs);
    }
    else
    {
        ScopeLookup scopeLookup = ScopeLookup::this_;
        if (map)
        {
            scopeLookup = scopeLookup | ScopeLookup::map;
        }
        Symbol* symbol = container->Scope()->Lookup(namespaceName, scopeLookup, this);
        if (symbol)
        {
            if (symbol->IsNamespaceSymbol())
            {
                NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(symbol);
                BeginContainer(ns);
            }
            else
            {
                if (errors)
                {
                    errors->AddError(std::runtime_error("symbol '" + util::ToUtf8(symbol->Name()) + "' does not denote a namespace"));
                }
                return false;
            }
        }
        else
        {
            NamespaceSymbol* ns = new NamespaceSymbol(namespaceName->Str());
            if (source)
            {
                source->AddSymbol(ns);
            }
            container->AddSymbol(ns, this);
            BeginContainer(ns);
        }
    }
    return true;
}

void SymbolTable::EndNamespace()
{
    EndContainer();
}

void SymbolTable::AddAliasType(cmajor::fault::tolerant::ast::UsingAliasNode& node, Source* source)
{
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(node.Identifier()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.GetSpecifiers();
    if (specifiersNode)
    {
        aliasTypeSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(aliasTypeSymbol);
    MapNode(&node, aliasTypeSymbol);
    MapNode(node.Identifier(), aliasTypeSymbol);
    MapSymbol(aliasTypeSymbol, &node);
    container->AddSymbol(aliasTypeSymbol, this);
}

void SymbolTable::AddAliasType(cmajor::fault::tolerant::ast::TypedefAliasNode& node, Source* source)
{
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(node.Identifier()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.GetSpecifiers();
    if (specifiersNode)
    {
        aliasTypeSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(aliasTypeSymbol);
    MapNode(&node, aliasTypeSymbol);
    MapNode(node.Identifier(), aliasTypeSymbol);
    MapSymbol(aliasTypeSymbol, &node);
    container->AddSymbol(aliasTypeSymbol, this);
}

void SymbolTable::AddConstant(cmajor::fault::tolerant::ast::ConstantNode& node, Source* source)
{
    ConstantSymbol* constantSymbol = new ConstantSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        constantSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(constantSymbol);
    MapNode(&node, constantSymbol);
    MapNode(node.Id(), constantSymbol);
    MapSymbol(constantSymbol, &node);
    container->AddSymbol(constantSymbol, this);
}

void SymbolTable::BeginDelegate(cmajor::fault::tolerant::ast::DelegateNode& node, Source* source)
{
    DelegateSymbol* delegateSymbol = new DelegateSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        delegateSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(delegateSymbol);
    MapNode(&node, delegateSymbol);
    MapNode(node.Id(), delegateSymbol);
    MapSymbol(delegateSymbol, &node);
    container->AddSymbol(delegateSymbol, this);
    BeginContainer(delegateSymbol);
}

void SymbolTable::EndDelegate()
{
    EndContainer();
}

void SymbolTable::BeginClassDelegate(cmajor::fault::tolerant::ast::ClassDelegateNode& node, Source* source)
{
    ClassDelegateSymbol* classDelegateSymbol = new ClassDelegateSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        classDelegateSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(classDelegateSymbol);
    MapNode(&node, classDelegateSymbol);
    MapNode(node.Id(), classDelegateSymbol);
    MapSymbol(classDelegateSymbol, &node);
    container->AddSymbol(classDelegateSymbol, this);
    BeginContainer(classDelegateSymbol);
}

void SymbolTable::EndClassDelegate()
{
    EndContainer();
}

void SymbolTable::BeginEnumType(cmajor::fault::tolerant::ast::EnumTypeNode& node, Source* source)
{
    EnumTypeSymbol* enumTypeSymbol = new EnumTypeSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        enumTypeSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(enumTypeSymbol);
    MapNode(&node, enumTypeSymbol);
    MapNode(node.Id(), enumTypeSymbol);
    MapSymbol(enumTypeSymbol, &node);
    container->AddSymbol(enumTypeSymbol, this);
    BeginContainer(enumTypeSymbol);
}

void SymbolTable::EndEnumType()
{
    EndContainer();
}

void SymbolTable::AddEnumConstant(cmajor::fault::tolerant::ast::EnumConstantNode& node, Source* source)
{
    EnumConstantSymbol* enumConstantSymbol = new EnumConstantSymbol(node.Id()->Str(), node.GetSpan());
    source->AddSymbol(enumConstantSymbol);
    MapNode(&node, enumConstantSymbol);
    MapNode(node.Id(), enumConstantSymbol);
    MapSymbol(enumConstantSymbol, &node);
    container->AddSymbol(enumConstantSymbol, this);
}

void SymbolTable::BeginDeclarationBlock(cmajor::fault::tolerant::ast::CompoundStatementNode& node, Source* source, int& count)
{
    DeclarationBlockSymbol* declarationBlockSymbol = new DeclarationBlockSymbol(u"@block" + util::ToUtf16(std::to_string(count)), node.GetSpan());
    ++count;
    source->AddSymbol(declarationBlockSymbol);
    MapNode(&node, declarationBlockSymbol);
    MapSymbol(declarationBlockSymbol, &node);
    container->AddSymbol(declarationBlockSymbol, this);
    BeginContainer(declarationBlockSymbol);
}

void SymbolTable::EndDeclarationBlock()
{
    EndContainer();
}

void SymbolTable::AddParameter(cmajor::fault::tolerant::ast::ParameterNode& node, int& paramIndex, Source* source)
{
    std::u16string paramName;
    if (node.Name())
    {
        paramName = node.Name()->Str();
    }
    else
    {
        paramName = u"@p" + util::ToUtf16(std::to_string(paramIndex++));
    }
    ParameterSymbol* parameterSymbol = new ParameterSymbol(paramName, node.GetSpan());
    source->AddSymbol(parameterSymbol);
    MapNode(&node, parameterSymbol);
    if (node.Name())
    {
        MapNode(node.Name(), parameterSymbol);
    }
    MapSymbol(parameterSymbol, &node);
    container->AddSymbol(parameterSymbol, this);
}

void SymbolTable::AddLocalVariable(cmajor::fault::tolerant::ast::ConstructionStatementNode& node, Source* source)
{
    LocalVariableSymbol* localVariableSymbol = new LocalVariableSymbol(node.Id()->Str(), node.GetSpan());
    source->AddSymbol(localVariableSymbol);
    MapNode(&node, localVariableSymbol);
    MapNode(node.Id(), localVariableSymbol);
    MapSymbol(localVariableSymbol, &node);
    container->AddSymbol(localVariableSymbol, this);
}

void SymbolTable::AddMemberVariable(cmajor::fault::tolerant::ast::MemberVariableNode& node, Source* source)
{
    MemberVariableSymbol* memberVariableSymbol = new MemberVariableSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        memberVariableSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(memberVariableSymbol);
    MapNode(&node, memberVariableSymbol);
    MapNode(node.Id(), memberVariableSymbol);
    MapSymbol(memberVariableSymbol, &node);
    container->AddSymbol(memberVariableSymbol, this);
}

void SymbolTable::AddGlobalVariable(cmajor::fault::tolerant::ast::GlobalVariableNode& node, Source* source)
{
    GlobalVariableSymbol* globalVariableSymbol = new GlobalVariableSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        globalVariableSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(globalVariableSymbol);
    MapNode(&node, globalVariableSymbol);
    MapNode(node.Id(), globalVariableSymbol);
    MapSymbol(globalVariableSymbol, &node);
    container->AddSymbol(globalVariableSymbol, this);
}

void SymbolTable::AddTemplateParameter(cmajor::fault::tolerant::ast::TemplateParameterNode& node, Source* source)
{
    TemplateParameterSymbol* templateParameterSymbol = new TemplateParameterSymbol(node.Identifier()->Str(), node.GetSpan());
    source->AddSymbol(templateParameterSymbol);
    MapNode(&node, templateParameterSymbol);
    MapNode(node.Identifier(), templateParameterSymbol);
    MapSymbol(templateParameterSymbol, &node);
    container->AddSymbol(templateParameterSymbol, this);
}

void SymbolTable::BeginFunction(cmajor::fault::tolerant::ast::FunctionNode& node, Source* source)
{
    FunctionSymbol* functionSymbol = new FunctionSymbol(node.FunctionGroupId()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        functionSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(functionSymbol);
    MapNode(&node, functionSymbol);
    MapNode(node.FunctionGroupId(), functionSymbol);
    MapSymbol(functionSymbol, &node);
    container->AddSymbol(functionSymbol, this);
    BeginContainer(functionSymbol);
}

void SymbolTable::EndFunction()
{
    EndContainer();
}

void SymbolTable::BeginStaticConstructor(cmajor::fault::tolerant::ast::StaticConstructorNode& node, Source* source)
{
    StaticConstructorSymbol* staticConstructorSymbol = new StaticConstructorSymbol(node.ClassName()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        staticConstructorSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(staticConstructorSymbol);
    MapNode(&node, staticConstructorSymbol);
    MapNode(node.ClassName(), staticConstructorSymbol);
    MapSymbol(staticConstructorSymbol, &node);
    container->AddSymbol(staticConstructorSymbol, this);
    BeginContainer(staticConstructorSymbol);
}

void SymbolTable::EndStaticConstructor()
{
    EndContainer();
}

void SymbolTable::BeginConstructor(cmajor::fault::tolerant::ast::ConstructorNode& node, Source* source)
{
    ConstructorSymbol* constructorSymbol = new ConstructorSymbol(node.ClassName()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        constructorSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(constructorSymbol);
    MapNode(&node, constructorSymbol);
    MapNode(node.ClassName(), constructorSymbol);
    MapSymbol(constructorSymbol, &node);
    container->AddSymbol(constructorSymbol, this);
    BeginContainer(constructorSymbol);
}

void SymbolTable::EndConstructor()
{
    EndContainer();
}

void SymbolTable::BeginDestructor(cmajor::fault::tolerant::ast::DestructorNode& node, Source* source)
{
    DestructorSymbol* destructorSymbol = new DestructorSymbol(u"~" + node.ClassName()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        destructorSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(destructorSymbol);
    MapNode(&node, destructorSymbol);
    MapNode(node.ClassName(), destructorSymbol);
    MapSymbol(destructorSymbol, &node);
    container->AddSymbol(destructorSymbol, this);
    BeginContainer(destructorSymbol);
}

void SymbolTable::EndDestructor()
{
    EndContainer();
}

void SymbolTable::BeginMemberFunction(cmajor::fault::tolerant::ast::MemberFunctionNode& node, Source* source)
{
    MemberFunctionSymbol* memberFunctionSymbol = new MemberFunctionSymbol(node.FunctionGroupId()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        memberFunctionSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(memberFunctionSymbol);
    MapNode(&node, memberFunctionSymbol);
    MapNode(node.FunctionGroupId(), memberFunctionSymbol);
    MapSymbol(memberFunctionSymbol, &node);
    container->AddSymbol(memberFunctionSymbol, this);
    BeginContainer(memberFunctionSymbol);
}

void SymbolTable::EndMemberFunction()
{
    EndContainer();
}

void SymbolTable::BeginConversionFunction(cmajor::fault::tolerant::ast::ConversionFunctionNode& node, Source* source)
{
    ConversionFunctionSymbol* conversionFunctionSymbol = new ConversionFunctionSymbol(node.Name(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        conversionFunctionSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(conversionFunctionSymbol);
    MapNode(&node, conversionFunctionSymbol);
    MapSymbol(conversionFunctionSymbol, &node);
    container->AddSymbol(conversionFunctionSymbol, this);
    BeginContainer(conversionFunctionSymbol);
}

void SymbolTable::EndConversionFunction()
{
    EndContainer();
}

void SymbolTable::BeginClass(cmajor::fault::tolerant::ast::ClassNode& node, Source* source)
{
    ClassTypeSymbol* classTypeSymbol = new ClassTypeSymbol(node.Identifier()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        classTypeSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(classTypeSymbol);
    MapNode(&node, classTypeSymbol);
    MapNode(node.Identifier(), classTypeSymbol);
    MapSymbol(classTypeSymbol, &node);
    container->AddSymbol(classTypeSymbol, this);
    BeginContainer(classTypeSymbol);
}

void SymbolTable::EndClass()
{
    EndContainer();
}

void SymbolTable::BeginInterface(cmajor::fault::tolerant::ast::InterfaceNode& node, Source* source)
{
    InterfaceTypeSymbol* interfaceTypeSymbol = new InterfaceTypeSymbol(node.Id()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        interfaceTypeSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(interfaceTypeSymbol);
    MapNode(&node, interfaceTypeSymbol);
    MapNode(node.Id(), interfaceTypeSymbol);
    MapSymbol(interfaceTypeSymbol, &node);
    container->AddSymbol(interfaceTypeSymbol, this);
    BeginContainer(interfaceTypeSymbol);
}

void SymbolTable::EndInterface()
{
    EndContainer();
}

void SymbolTable::BeginConcept(cmajor::fault::tolerant::ast::ConceptNode& node, Source* source)
{
    ConceptSymbol* conceptSymbol = new ConceptSymbol(node.ConceptId()->Str(), node.GetSpan());
    cmajor::fault::tolerant::ast::SpecifiersNode* specifiersNode = node.Specifiers();
    if (specifiersNode)
    {
        conceptSymbol->SetSpecifiers(specifiersNode->GetSpecifiers());
    }
    source->AddSymbol(conceptSymbol);
    MapNode(&node, conceptSymbol);
    MapNode(node.ConceptId(), conceptSymbol);
    MapSymbol(conceptSymbol, &node);
    container->AddSymbol(conceptSymbol, this);
    BeginContainer(conceptSymbol);
}

void SymbolTable::EndConcept()
{
    EndContainer();
}

void SymbolTable::AddTypeParameter(cmajor::fault::tolerant::ast::IdentifierNode& node, Source* source)
{
    TypeParameterSymbol* typeParameterSymbol = new TypeParameterSymbol(node.Str(), node.GetSpan());
    source->AddSymbol(typeParameterSymbol);
    MapNode(&node, typeParameterSymbol);
    MapSymbol(typeParameterSymbol, &node);
    container->AddSymbol(typeParameterSymbol, this);
}

void SymbolTable::MapNode(cmajor::fault::tolerant::ast::Node* node, Symbol* symbol)
{
    nodeSymbolMap[node] = symbol;
}

Symbol* SymbolTable::GetMappedSymbol(cmajor::fault::tolerant::ast::Node* node) const
{
    auto it = nodeSymbolMap.find(node);
    if (it != nodeSymbolMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::MapSymbol(Symbol* symbol, cmajor::fault::tolerant::ast::Node* node)
{
    symbolNodeMap[symbol] = node;
}

cmajor::fault::tolerant::ast::Node* SymbolTable::GetMappedNode(Symbol* symbol) const
{
    auto it = symbolNodeMap.find(symbol);
    if (it != symbolNodeMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::MapType(TypeSymbol* type)
{
    typeMap[type->Id()] = type;
}

TypeSymbol* SymbolTable::GetType(const util::uuid& id) const
{
    auto it = typeMap.find(id);
    if (it != typeMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::AddBasicTypeToGlobalNamespace(BasicTypeSymbol* basicType)
{
    globalNs.AddSymbol(basicType, this); 
}

void SymbolTable::AddBasicType(BasicTypeSymbol* basicType)
{
    basicTypeMap[basicType->Kind()] = basicType;
}

BasicTypeSymbol* SymbolTable::GetBasicType(SymbolKind symbolKind) const
{
    auto it = basicTypeMap.find(symbolKind);
    if (it != basicTypeMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

TypeSymbol* SymbolTable::MakeCompoundType(TypeSymbol* baseType, const Derivations& derivations)
{
    Derivations drv = derivations;
    if (baseType->IsCompoundTypeSymbol())
    {
        CompoundTypeSymbol* compoundTypeSymbol = static_cast<CompoundTypeSymbol*>(baseType);
        drv = Merge(drv, compoundTypeSymbol->GetDerivations());
        baseType = compoundTypeSymbol->BaseType();
    }
    if (drv.IsEmpty())
    {
        return baseType;
    }
    std::vector<CompoundTypeSymbol*>& compoundTypeVec = compoundTypeMap[baseType];
    for (const auto& compoundType : compoundTypeVec)
    {
        if (compoundType->GetDerivations() == drv)
        {
            return compoundType;
        }
    }
    CompoundTypeSymbol* compoundType = new CompoundTypeSymbol(baseType, drv);
    compoundTypeVec.push_back(compoundType);
    compoundTypes.push_back(std::unique_ptr<CompoundTypeSymbol>(compoundType));
    MapType(compoundType);
    return compoundType;
}

void SymbolTable::MakeCompoundTypeMap()
{
    for (const auto& compoundType : compoundTypes)
    {
        if (compoundType->BaseType())
        {
            std::vector<CompoundTypeSymbol*>& compoundTypeVec = compoundTypeMap[compoundType->BaseType()];
            if (std::find(compoundTypeVec.begin(), compoundTypeVec.end(), compoundType.get()) == compoundTypeVec.end())
            {
                compoundTypeVec.push_back(compoundType.get());
            }
        }
    }
}

TypeSymbol* SymbolTable::MakeArrayType(TypeSymbol* elementType, int64_t size)
{
    std::unique_ptr<ArrayTypeSymbol> symbol(new ArrayTypeSymbol(elementType, size));
    auto it = arrayTypeSet.find(symbol.get());
    if (it != arrayTypeSet.cend())
    {
        return *it;
    }
    ArrayTypeSymbol* sym = symbol.get();
    arrayTypeSet.insert(sym);
    arrayTypes.push_back(std::move(symbol));
    MapType(sym);
    return sym;
}

void SymbolTable::MakeArrayTypeSet()
{
    for (const auto& arrayType : arrayTypes)
    {
        ArrayTypeSymbol* at = arrayType.get();
        arrayTypeSet.insert(at);
        MapType(at);
    }
}

void SymbolTable::Compact()
{
    globalNs.Compact();
}

void InitCoreSymbolTable(SymbolTable* symbolTable)
{
    symbolTable->AddBasicTypeToGlobalNamespace(new AutoTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new BoolTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new SByteTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new ByteTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new ShortTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new UShortTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new IntTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new UIntTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new LongTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new ULongTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new FloatTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new DoubleTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new CharTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new WCharTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new UCharTypeSymbol());
    symbolTable->AddBasicTypeToGlobalNamespace(new VoidTypeSymbol());
}

} // namespace cmajor::fault::tolerant::symbols
