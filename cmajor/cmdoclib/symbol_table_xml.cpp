// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmdoclib.symbol_table_xml;

import std.filesystem;
import cmdoclib.util;
import cmdoclib.input;
import cmdoclib.global;
import util;
import cmajor.ast;
import soul.xml.dom;
import soul.xml.dom.parser;

namespace cmdoclib {

soul::xml::Element* CreateSymbolElement(const std::string& elementName, cmajor::symbols::Symbol& symbol)
{
    soul::xml::Element* symbolElement(soul::xml::MakeElement(elementName));
    symbolElement->SetAttribute("name", util::ToUtf8(symbol.CodeName()));
    symbolElement->SetAttribute("id", util::ToUtf8(symbol.Id()));
    return symbolElement;
}

bool IsNonemptyNamespace(soul::xml::Element* nsElement) 
{
    soul::xml::Node* child = nsElement->FirstChild();
    while (child)
    {
        if (child->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(child);
            if (element->Name() == "namespaces" || element->Name() == "namespace")
            {
                if (IsNonemptyNamespace(element))
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
        child = child->Next();
    }
    return false;
}

std::vector<cmajor::symbols::NamespaceSymbol*> GetNamespaces(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::NamespaceSymbol*> namespaceSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::namespaceSymbol)
        {
            namespaceSymbols.push_back(static_cast<cmajor::symbols::NamespaceSymbol*>(member.get()));
        }
    }
    std::sort(namespaceSymbols.begin(), namespaceSymbols.end(), ByCodeName());
    return namespaceSymbols;
}

std::vector<cmajor::symbols::ConceptSymbol*> GetConcepts(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::ConceptSymbol*> conceptSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::conceptSymbol && member->IsProject())
        {
            conceptSymbols.push_back(static_cast<cmajor::symbols::ConceptSymbol*>(member.get()));
        }
    }
    std::sort(conceptSymbols.begin(), conceptSymbols.end(), ByCodeName());
    return conceptSymbols;
}

std::vector<cmajor::symbols::ClassTypeSymbol*> GetClasses(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::ClassTypeSymbol*> classSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol && member->IsProject())
        {
            cmajor::symbols::ClassTypeSymbol* cls = static_cast<cmajor::symbols::ClassTypeSymbol*>(member.get());
            if (cls->IsClassTemplate())
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* prototype = cls->Prototype();
                if (!prototype)
                {
                    throw std::runtime_error("prototype not set");
                }
                cls = prototype;
            }
            classSymbols.push_back(cls);
        }
    }
    std::sort(classSymbols.begin(), classSymbols.end(), ByCodeName());
    return classSymbols;
}

std::vector<cmajor::symbols::InterfaceTypeSymbol*> GetInterfaces(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::InterfaceTypeSymbol*> interfaceSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol && member->IsProject())
        {
            cmajor::symbols::InterfaceTypeSymbol* intf = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(member.get());
            interfaceSymbols.push_back(intf);
        }
    }
    std::sort(interfaceSymbols.begin(), interfaceSymbols.end(), ByCodeName());
    return interfaceSymbols;
}

std::vector<cmajor::symbols::FunctionSymbol*> GetConstructorsAndDestructors(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::FunctionSymbol*> ctorsAndDtors;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if ((member->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol ||
            member->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol ||
            member->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol) && member->IsProject())
        {
            cmajor::symbols::FunctionSymbol* ctorOrDtor = static_cast<cmajor::symbols::FunctionSymbol*>(member.get());
            if (ctorOrDtor->HasSource())
            {
                ctorsAndDtors.push_back(ctorOrDtor);
            }
        }
    }
    std::sort(ctorsAndDtors.begin(), ctorsAndDtors.end(), ByCodeName());
    return ctorsAndDtors;
}

std::vector<cmajor::symbols::FunctionSymbol*> GetFunctions(cmajor::symbols::ContainerSymbol& container, const std::string& moduleName)
{
    std::vector<cmajor::symbols::FunctionSymbol*> functions;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol ||
            member->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol ||
            member->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
        {
            continue;
        }
        if (member->IsFunctionSymbol() && member->IsProject())
        {
            cmajor::symbols::FunctionSymbol* fun = static_cast<cmajor::symbols::FunctionSymbol*>(member.get());
            if (fun->HasSource())
            {
                if (fun->GetSymbolType() == cmajor::symbols::SymbolType::functionSymbol && !fun->IsFunctionTemplate() && fun->IsTemplateSpecialization())
                {
                    continue;
                }
                functions.push_back(fun);
            }
            else if (moduleName == "System.Core" && util::ToUtf8(container.FullName()) == "System.Meta")
            {
                functions.push_back(fun);
            }
        }
    }
    std::sort(functions.begin(), functions.end(), ByCodeName());
    return functions;
}

std::vector<cmajor::symbols::DelegateTypeSymbol*> GetDelegates(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::DelegateTypeSymbol*> delegateSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol && member->IsProject())
        {
            delegateSymbols.push_back(static_cast<cmajor::symbols::DelegateTypeSymbol*>(member.get()));
        }
    }
    std::sort(delegateSymbols.begin(), delegateSymbols.end(), ByCodeName());
    return delegateSymbols;
}

std::vector<cmajor::symbols::ClassDelegateTypeSymbol*> GetClassDelegates(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::ClassDelegateTypeSymbol*> classDelegateSymbols;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol && member->IsProject())
        {
            classDelegateSymbols.push_back(static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(member.get()));
        }
    }
    std::sort(classDelegateSymbols.begin(), classDelegateSymbols.end(), ByCodeName());
    return classDelegateSymbols;
}

std::vector<cmajor::symbols::AliasTypeSymbol*> GetTypedefs(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::AliasTypeSymbol*> typedefs;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::aliasTypeSymbol && member->IsProject())
        {
            typedefs.push_back(static_cast<cmajor::symbols::AliasTypeSymbol*>(member.get()));
        }
    }
    std::sort(typedefs.begin(), typedefs.end(), ByCodeName());
    return typedefs;
}

std::vector<cmajor::symbols::EnumTypeSymbol*> GetEnumTypes(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::EnumTypeSymbol*> enumTypes;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol && member->IsProject())
        {
            enumTypes.push_back(static_cast<cmajor::symbols::EnumTypeSymbol*>(member.get()));
        }
    }
    std::sort(enumTypes.begin(), enumTypes.end(), ByCodeName());
    return enumTypes;
}

std::vector<cmajor::symbols::EnumConstantSymbol*> GetEnumConstants(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::EnumConstantSymbol*> enumConstants;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::enumConstantSymbol && member->IsProject())
        {
            enumConstants.push_back(static_cast<cmajor::symbols::EnumConstantSymbol*>(member.get()));
        }
    }
    return enumConstants;
}

std::vector<cmajor::symbols::ConstantSymbol*> GetConstants(cmajor::symbols::ContainerSymbol& container)
{
    std::vector<cmajor::symbols::ConstantSymbol*> constants;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->GetSymbolType() == cmajor::symbols::SymbolType::constantSymbol && member->IsProject())
        {
            constants.push_back(static_cast<cmajor::symbols::ConstantSymbol*>(member.get()));
        }
    }
    std::sort(constants.begin(), constants.end(), ByCodeName());
    return constants;
}

std::vector<cmajor::symbols::VariableSymbol*> GetVariables(cmajor::symbols::ContainerSymbol& container, bool sort, bool skipFirst)
{
    std::vector<cmajor::symbols::VariableSymbol*> variables;
    bool first = true;
    for (const std::unique_ptr<cmajor::symbols::Symbol>& member : container.Members())
    {
        if (member->IsVariableSymbol() && member->IsProject() && member->GetSymbolType() != cmajor::symbols::SymbolType::localVariableSymbol)
        {
            if (first)
            {
                first = false;
                if (skipFirst)
                {
                    continue;
                }
            }
            variables.push_back(static_cast<cmajor::symbols::VariableSymbol*>(member.get()));
        }
    }
    if (sort)
    {
        std::sort(variables.begin(), variables.end(), ByCodeName());
    }
    return variables;
}

class SymbolTableXmlBuilder : cmajor::ast::Visitor
{
public:
    SymbolTableXmlBuilder(cmajor::symbols::Context* context_, const std::string& moduleName_, cmajor::symbols::SymbolTable& symbolTable_, std::map<int, File>& fileMap_, 
        const std::string& modulePrefix_, const std::string& extModulePrefix_);
    void WriteDocument(const std::string& symbolTableXmlFilePath);
    bool AddNamespace(cmajor::symbols::NamespaceSymbol& ns);
    void AddConcept(cmajor::symbols::ConceptSymbol& conceptSymbol);
    void AddClass(cmajor::symbols::ClassTypeSymbol& cls);
    void AddInterface(cmajor::symbols::InterfaceTypeSymbol& interface);
    void AddCtorOrDtor(cmajor::symbols::FunctionSymbol& function);
    void AddFunction(cmajor::symbols::FunctionSymbol& function);
    void AddDelegate(cmajor::symbols::DelegateTypeSymbol& delegate);
    void AddClassDelegate(cmajor::symbols::ClassDelegateTypeSymbol& classDelegate);
    void AddTypedef(cmajor::symbols::AliasTypeSymbol& typedef_);
    void AddEnumType(cmajor::symbols::EnumTypeSymbol& enumType);
    void AddEnumConstant(cmajor::symbols::EnumConstantSymbol& enumConstant);
    void AddConstant(cmajor::symbols::ConstantSymbol& constant);
    void AddVariable(cmajor::symbols::VariableSymbol& variable);
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
    void Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode) override;
    void Visit(cmajor::ast::AxiomNode& axiomNode) override;
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;
    void Visit(cmajor::ast::ParameterNode& parameterNode) override;
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
private:
    cmajor::symbols::Context* context;
    std::string moduleName;
    cmajor::symbols::SymbolTable& symbolTable;
    std::map<int, File>& fileMap;
    std::string modulePrefix;
    std::string extModulePrefix;
    std::vector<soul::xml::Document*> moduleXmlDocuments;
    std::unique_ptr<soul::xml::Document> symbolTableXmlDocument;
    soul::xml::Element* symbolTableElement;
    soul::xml::Element* currentElement;
    std::set<std::string> typeIdSet;
    std::vector<cmajor::symbols::TypeSymbol*> types;
    void GenerateTypes();
    std::string GetOrInsertType(cmajor::symbols::TypeSymbol* type);
    void AddChildren(cmajor::symbols::ContainerSymbol& container);
};

SymbolTableXmlBuilder::SymbolTableXmlBuilder(cmajor::symbols::Context* context_, const std::string& moduleName_, cmajor::symbols::SymbolTable& symbolTable_, 
    std::map<int, File>& fileMap_, const std::string& modulePrefix_, const std::string& extModulePrefix_) :
    context(context_), moduleName(moduleName_), symbolTableXmlDocument(new soul::xml::Document()),
    symbolTableElement(soul::xml::MakeElement("symbolTable")), currentElement(symbolTableElement), symbolTable(symbolTable_),
    fileMap(fileMap_), modulePrefix(modulePrefix_), extModulePrefix(extModulePrefix_)
{
    symbolTableElement->SetAttribute("module", moduleName);
    symbolTableElement->SetAttribute("id", moduleName);
    std::vector<File*> files;
    for (auto& p : fileMap)
    {
        files.push_back(&p.second);
    }
    std::sort(files.begin(), files.end(), FileByName());
    soul::xml::Element* filesElement(soul::xml::MakeElement("files"));
    for (File* file : files)
    {
        soul::xml::Element* fileElement(soul::xml::MakeElement("file"));
        fileElement->SetAttribute("name", file->name);
        fileElement->SetAttribute("path", util::Path::Combine("file", util::Path::GetFileName(file->htmlFilePath)));
        filesElement->AppendChild((fileElement));
    }
    symbolTableElement->AppendChild(filesElement);
}

void SymbolTableXmlBuilder::WriteDocument(const std::string& symbolTableXmlFilePath)
{
    GenerateTypes();
    symbolTableXmlDocument->AppendChild(symbolTableElement);
    std::ofstream symbolTableXmlFile(symbolTableXmlFilePath);
    util::CodeFormatter formatter(symbolTableXmlFile);
    formatter.SetIndentSize(1);
    symbolTableXmlDocument->Write(formatter);
}

bool SymbolTableXmlBuilder::AddNamespace(cmajor::symbols::NamespaceSymbol& ns)
{
    bool added = false;
    soul::xml::Element* nsElement = CreateSymbolElement("namespace", ns);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(ns.Id()) + ".html");
    nsElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(ns.Id()) + ".html");
    nsElement->SetAttribute("extPath", extPath);
    soul::xml::Element* prevElement = currentElement;
    currentElement = nsElement;
    AddChildren(ns);
    if (IsNonemptyNamespace(nsElement))
    {
        prevElement->AppendChild(nsElement);
        added = true;
    }
    currentElement = prevElement;
    return added;
}

void SymbolTableXmlBuilder::AddConcept(cmajor::symbols::ConceptSymbol& conceptSymbol)
{
    soul::xml::Element* conceptElement = CreateSymbolElement("concept", conceptSymbol);
    if (conceptSymbol.RefinedConcept())
    {
        conceptElement->SetAttribute("refines", util::ToUtf8(conceptSymbol.RefinedConcept()->Id()));
        AddConceptRefinement(conceptSymbol.RefinedConcept(), &conceptSymbol);
    }
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(conceptSymbol.Id()) + ".html");
    conceptElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(conceptSymbol.Id()) + ".html");
    conceptElement->SetAttribute("extPath", extPath);
    int n = static_cast<int>(conceptSymbol.TemplateParameters().size());
    if (n > 0)
    {
        soul::xml::Element* typeParametersElement(soul::xml::MakeElement("typeParameters"));
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Element* typeParameterElement = CreateSymbolElement("typeParameter", *conceptSymbol.TemplateParameters()[i]);
            std::string docPath = conceptElement->GetAttribute("docPath") + "#" + typeParameterElement->GetAttribute("id");
            typeParameterElement->SetAttribute("docPath", docPath);
            typeParametersElement->AppendChild(typeParameterElement);
        }
        conceptElement->AppendChild(typeParametersElement);
    }
    if (conceptSymbol.HasSource())
    {
        File& file = fileMap[conceptSymbol.GetFullSpan().fileIndex];
        conceptElement->SetAttribute("fileName", file.name);
        conceptElement->SetAttribute("filePath", file.htmlFilePath);
        conceptElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(conceptSymbol.GetFullSpan())));
    }
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&conceptSymbol);
    cmajor::ast::ConceptNode* conceptNode = nullptr;
    if (node && node->GetNodeType() == cmajor::ast::NodeType::conceptNode)
    {
        conceptNode = static_cast<cmajor::ast::ConceptNode*>(node);
        conceptElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(conceptNode->GetSpecifiers()));
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = conceptElement;
    if (conceptNode)
    {
        int nc = conceptNode->Constraints().Count();
        if (nc > 0)
        {
            soul::xml::Element* constraintsElement(soul::xml::MakeElement("constraints"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = constraintsElement;
            for (int i = 0; i < nc; ++i)
            {
                cmajor::ast::ConstraintNode* constraint = conceptNode->Constraints()[i];
                constraint->Accept(*this);
            }
            prevElement->AppendChild(constraintsElement);
            currentElement = prevElement;
        }
        int na = conceptNode->Axioms().Count();
        if (na > 0)
        {
            soul::xml::Element* axiomsElement(soul::xml::MakeElement("axioms"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = axiomsElement;
            for (int i = 0; i < na; ++i)
            {
                cmajor::ast::AxiomNode* axiom = conceptNode->Axioms()[i];
                axiom->Accept(*this);
            }
            prevElement->AppendChild(axiomsElement);
            currentElement = prevElement;
        }
    }
    AddChildren(conceptSymbol);
    prevElement->AppendChild(conceptElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddClass(cmajor::symbols::ClassTypeSymbol& cls)
{
    soul::xml::Element* classElement = CreateSymbolElement("class", cls);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(cls.Id()) + ".html");
    classElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(cls.Id()) + ".html");
    classElement->SetAttribute("extPath", extPath);
    File& file = fileMap[cls.GetFullSpan().fileIndex];
    classElement->SetAttribute("fileName", file.name);
    classElement->SetAttribute("filePath", file.htmlFilePath);
    classElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(cls.GetFullSpan())));
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&cls);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::classNode)
    {
        cmajor::ast::ClassNode* classNode = static_cast<cmajor::ast::ClassNode*>(node);
        classElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(classNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("class node expected");
    }
    if (cls.GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(&cls);
        if (specialization->IsPrototype())
        {
            soul::xml::Element* templateParametersElement(soul::xml::MakeElement("templateParameters"));
            int n = static_cast<int>(specialization->TemplateArgumentTypes().size());
            for (int i = 0; i < n; ++i)
            {
                cmajor::symbols::TypeSymbol* templateArgumentType = specialization->TemplateArgumentTypes()[i];
                if (templateArgumentType->GetSymbolType() == cmajor::symbols::SymbolType::templateParameterSymbol)
                {
                    cmajor::symbols::TemplateParameterSymbol* templateParameter = static_cast<cmajor::symbols::TemplateParameterSymbol*>(templateArgumentType);
                    soul::xml::Element* templateParameterElement = CreateSymbolElement("templateParameter", *templateParameter);
                    std::string docPath = classElement->GetAttribute("docPath") + "#" + util::ToUtf8(templateParameter->Id());
                    templateParameterElement->SetAttribute("docPath", docPath);
                    if (templateParameter->HasDefault())
                    {
                        GetOrInsertType(templateParameter->DefaultType());
                        templateParameterElement->SetAttribute("default", util::ToUtf8(templateParameter->DefaultType()->Id()));
                    }
                    templateParametersElement->AppendChild(templateParameterElement);
                }
                else
                {
                    throw std::runtime_error("template parameter expected");
                }
            }
            classElement->AppendChild(templateParametersElement);
        }
        else
        {
            throw std::runtime_error("prototype expected");
        }
    }
    if (cls.BaseClass())
    {
        cmajor::symbols::ClassTypeSymbol* baseClass = cls.BaseClass();
        std::string baseClassSpecializationName;
        if (baseClass->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(baseClass);
            baseClass = specialization->GetClassTemplate();
            baseClassSpecializationName = util::ToUtf8(baseClass->Ns(context)->FullName());
            if (!baseClassSpecializationName.empty())
            {
                baseClassSpecializationName.append(1, '.');
            }
            baseClassSpecializationName.append(util::ToUtf8(specialization->Name()));
        }
        std::string baseClassRef = GetOrInsertType(baseClass);
        classElement->SetAttribute("baseClass", baseClassRef);
        if (!baseClassSpecializationName.empty())
        {
            classElement->SetAttribute("baseClassSpecializationName", baseClassSpecializationName);
        }
        AddDerivedClass(baseClass, &cls);
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = classElement;
    AddChildren(cls);
    if (cls.Constraint())
    {
        cls.Constraint()->Accept(*this);
    }
    prevElement->AppendChild(classElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddInterface(cmajor::symbols::InterfaceTypeSymbol& interface)
{
    soul::xml::Element* interfaceElement = CreateSymbolElement("interface", interface);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(interface.Id()) + ".html");
    interfaceElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(interface.Id()) + ".html");
    interfaceElement->SetAttribute("extPath", extPath);
    File& file = fileMap[interface.GetFullSpan().fileIndex];
    interfaceElement->SetAttribute("fileName", file.name);
    interfaceElement->SetAttribute("filePath", file.htmlFilePath);
    interfaceElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(interface.GetFullSpan())));
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&interface);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::interfaceNode)
    {
        cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
        interfaceElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(interfaceNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("interface node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = interfaceElement;
    AddChildren(interface);
    prevElement->AppendChild(interfaceElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddCtorOrDtor(cmajor::symbols::FunctionSymbol& function)
{
    soul::xml::Element* ctorDtorElement = CreateSymbolElement("ctorDtor", function);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.Id()));
    ctorDtorElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.Id()));
    ctorDtorElement->SetAttribute("extPath", extPath);
    File& file = fileMap[function.GetFullSpan().fileIndex];
    ctorDtorElement->SetAttribute("fileName", file.name);
    ctorDtorElement->SetAttribute("filePath", file.htmlFilePath);
    ctorDtorElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(function.GetFullSpan())));
    if (function.GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol)
    {
        ctorDtorElement->SetAttribute("kind", "Static Constructor");
    }
    else if (function.GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
    {
        ctorDtorElement->SetAttribute("kind", "Constructor");
    }
    else if (function.GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
    {
        ctorDtorElement->SetAttribute("kind", "Destructor");
    }
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&function);
    if (node && node->IsFunctionNode())
    {
        cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
        ctorDtorElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(functionNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("function node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = ctorDtorElement;
    AddChildren(function);
    prevElement->AppendChild(ctorDtorElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddFunction(cmajor::symbols::FunctionSymbol& function)
{
    soul::xml::Element* functionElement = CreateSymbolElement("function", function);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.Id()));
    if (function.Parent()->Id().empty() || function.Parent()->Id() == U"ns_")
    {
        docPath = util::Path::Combine(modulePrefix, "../index.html#" + util::ToUtf8(function.Id()));
    }
    functionElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.Id()));
    if (function.Parent()->Id().empty() || function.Parent()->Id() == U"ns_")
    {
        extPath = util::Path::Combine(extModulePrefix, "../index.html#" + util::ToUtf8(function.Id()));
    }
    functionElement->SetAttribute("extPath", extPath);
    if (function.HasSource())
    {
        File& file = fileMap[function.GetFullSpan().fileIndex];
        functionElement->SetAttribute("fileName", file.name);
        functionElement->SetAttribute("filePath", file.htmlFilePath);
        functionElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(function.GetFullSpan())));
    }
    if (function.GetSymbolType() == cmajor::symbols::SymbolType::functionSymbol)
    {
        functionElement->SetAttribute("kind", "Function");
    }
    else if (function.GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol)
    {
        if (function.IsStatic())
        {
            functionElement->SetAttribute("kind", "Static Member Function");
        }
        else
        {
            functionElement->SetAttribute("kind", "Member Function");
        }
    }
    else if (function.GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
    {
        functionElement->SetAttribute("kind", "Conversion Function");
    }
    if (function.GetFlag(cmajor::symbols::FunctionSymbolFlags::includeConstraint))
    {
        functionElement->SetAttribute("includeConstraint", "true");
    }
    if (function.HasSource())
    {
        cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&function);
        if (node && node->IsFunctionNode())
        {
            cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
            functionElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(functionNode->GetSpecifiers()));
        }
        else
        {
            throw std::runtime_error("function node expected");
        }
    }
    if (function.IsConst())
    {
        functionElement->SetAttribute("const", "true");
    }
    int n = static_cast<int>(function.TemplateParameters().size());
    if (n > 0)
    {
        soul::xml::Element* templateParametersElement(soul::xml::MakeElement("templateParameters"));
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Element* templateParameterElement = CreateSymbolElement("templateParameter", *function.TemplateParameters()[i]);
            std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.TemplateParameters()[i]->Id()));
            templateParameterElement->SetAttribute("docPath", docPath);
            std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(function.Parent()->Id()) + ".html#" + util::ToUtf8(function.TemplateParameters()[i]->Id()));
            templateParameterElement->SetAttribute("extPath", extPath);
            templateParametersElement->AppendChild(templateParameterElement);
        }
        functionElement->AppendChild(templateParametersElement);
    }
    cmajor::symbols::TypeSymbol* returnType = function.ReturnType();
    Assert(returnType, "nonnull return type expected");
    std::string returnTypeId = GetOrInsertType(returnType);
    functionElement->SetAttribute("returnType", returnTypeId);
    soul::xml::Element* prevElement = currentElement;
    currentElement = functionElement;
    AddChildren(function);
    if (function.Constraint())
    {
        function.Constraint()->Accept(*this);
    }
    prevElement->AppendChild(functionElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddDelegate(cmajor::symbols::DelegateTypeSymbol& delegate)
{
    soul::xml::Element* delegateElement = CreateSymbolElement("delegate", delegate);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(delegate.Parent()->Id()) + ".html#" + util::ToUtf8(delegate.Id()));
    delegateElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(delegate.Parent()->Id()) + ".html#" + util::ToUtf8(delegate.Id()));
    delegateElement->SetAttribute("extPath", extPath);
    File& file = fileMap[delegate.GetFullSpan().fileIndex];
    delegateElement->SetAttribute("fileName", file.name);
    delegateElement->SetAttribute("filePath", file.htmlFilePath);
    delegateElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(delegate.GetFullSpan())));
    cmajor::symbols::TypeSymbol* returnType = delegate.ReturnType();
    Assert(returnType, "nonnull return type expected");
    std::string returnTypeId = GetOrInsertType(returnType);
    delegateElement->SetAttribute("returnType", returnTypeId);
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&delegate);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::delegateNode)
    {
        cmajor::ast::DelegateNode* delegateNode = static_cast<cmajor::ast::DelegateNode*>(node);
        delegateElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(delegateNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("delegate node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = delegateElement;
    AddChildren(delegate);
    prevElement->AppendChild(delegateElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddClassDelegate(cmajor::symbols::ClassDelegateTypeSymbol& classDelegate)
{
    soul::xml::Element* classDelegateElement = CreateSymbolElement("classDelegate", classDelegate);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(classDelegate.Parent()->Id()) + ".html#" + util::ToUtf8(classDelegate.Id()));
    classDelegateElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(classDelegate.Parent()->Id()) + ".html#" + util::ToUtf8(classDelegate.Id()));
    classDelegateElement->SetAttribute("extPath", extPath);
    File& file = fileMap[classDelegate.GetFullSpan().fileIndex];
    classDelegateElement->SetAttribute("fileName", file.name);
    classDelegateElement->SetAttribute("filePath", file.htmlFilePath);
    classDelegateElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(classDelegate.GetFullSpan())));
    cmajor::symbols::TypeSymbol* returnType = classDelegate.ReturnType();
    Assert(returnType, "nonnull return type expected");
    std::string returnTypeId = GetOrInsertType(returnType);
    classDelegateElement->SetAttribute("returnType", returnTypeId);
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&classDelegate);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::classDelegateNode)
    {
        cmajor::ast::ClassDelegateNode* classDelegateNode = static_cast<cmajor::ast::ClassDelegateNode*>(node);
        classDelegateElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(classDelegateNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("class delegate node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = classDelegateElement;
    AddChildren(classDelegate);
    prevElement->AppendChild(classDelegateElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddTypedef(cmajor::symbols::AliasTypeSymbol& typedef_)
{
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&typedef_);
    if (!node) return;
    soul::xml::Element* typedefElement = CreateSymbolElement("typedef", typedef_);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(typedef_.Parent()->Id()) + ".html#" + util::ToUtf8(typedef_.Id()));
    typedefElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(typedef_.Parent()->Id()) + ".html#" + util::ToUtf8(typedef_.Id()));
    typedefElement->SetAttribute("extPath", extPath);
    cmajor::symbols::TypeSymbol* type = typedef_.GetType();
    Assert(type, "nonnull type expected");
    std::string typeId = GetOrInsertType(type);
    typedefElement->SetAttribute("type", typeId);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::typedefNode)
    {
        cmajor::ast::TypedefNode* typedefNode = static_cast<cmajor::ast::TypedefNode*>(node);
        typedefElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(typedefNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("typedef node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = typedefElement;
    prevElement->AppendChild(typedefElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddEnumType(cmajor::symbols::EnumTypeSymbol& enumType)
{
    soul::xml::Element* enumTypeElement = CreateSymbolElement("enumType", enumType);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(enumType.Id()) + ".html");
    enumTypeElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(enumType.Id()) + ".html");
    enumTypeElement->SetAttribute("extPath", extPath);
    File& file = fileMap[enumType.GetFullSpan().fileIndex];
    enumTypeElement->SetAttribute("fileName", file.name);
    enumTypeElement->SetAttribute("filePath", file.htmlFilePath);
    enumTypeElement->SetAttribute("line", std::to_string(cmajor::symbols::GetLineNumber(enumType.GetFullSpan())));
    cmajor::symbols::TypeSymbol* underlyingType = enumType.UnderlyingType();
    Assert(underlyingType, "nonnull type expected");
    std::string underlyingTypeId = GetOrInsertType(underlyingType);
    enumTypeElement->SetAttribute("underlyingType", underlyingTypeId);
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&enumType);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::enumTypeNode)
    {
        cmajor::ast::EnumTypeNode* enumTypeNode = static_cast<cmajor::ast::EnumTypeNode*>(node);
        enumTypeElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(enumTypeNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("enum type node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = enumTypeElement;
    AddChildren(enumType);
    prevElement->AppendChild(enumTypeElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddEnumConstant(cmajor::symbols::EnumConstantSymbol& enumConstant)
{
    soul::xml::Element* enumConstantElement = CreateSymbolElement("enumConstant", enumConstant);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(enumConstant.Parent()->Id()) + ".html#" + util::ToUtf8(enumConstant.Id()));
    enumConstantElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(enumConstant.Parent()->Id()) + ".html#" + util::ToUtf8(enumConstant.Id()));
    enumConstantElement->SetAttribute("extPath", extPath);
    if (!enumConstant.StrValue().empty())
    {
        enumConstantElement->SetAttribute("value", util::ToUtf8(enumConstant.StrValue()));
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = enumConstantElement;
    prevElement->AppendChild(enumConstantElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddConstant(cmajor::symbols::ConstantSymbol& constant)
{
    soul::xml::Element* constantElement = CreateSymbolElement("constant", constant);
    std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(constant.Parent()->Id()) + ".html#" + util::ToUtf8(constant.Id()));
    constantElement->SetAttribute("docPath", docPath);
    std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(constant.Parent()->Id()) + ".html#" + util::ToUtf8(constant.Id()));
    constantElement->SetAttribute("extPath", extPath);
    if (!constant.StrValue().empty())
    {
        constantElement->SetAttribute("value", util::ToUtf8(constant.StrValue()));
    }
    cmajor::symbols::TypeSymbol* type = constant.GetType();
    Assert(type, "nonnull type expected");
    std::string typeId = GetOrInsertType(type);
    constantElement->SetAttribute("type", typeId);
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&constant);
    if (node && node->GetNodeType() == cmajor::ast::NodeType::constantNode)
    {
        cmajor::ast::ConstantNode* constantNode = static_cast<cmajor::ast::ConstantNode*>(node);
        constantElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(constantNode->GetSpecifiers()));
    }
    else
    {
        throw std::runtime_error("constant node expected");
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = constantElement;
    prevElement->AppendChild(constantElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::AddVariable(cmajor::symbols::VariableSymbol& variable)
{
    if (variable.GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol) return;
    std::string elementName = "variable";
    if (variable.GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol)
    {
        elementName = "parameter";
    }
    soul::xml::Element* variableElement = CreateSymbolElement(elementName, variable);
    if (variable.GetSymbolType() == cmajor::symbols::SymbolType::memberVariableSymbol)
    {
        std::string docPath = util::Path::Combine(modulePrefix, util::ToUtf8(variable.Parent()->Id()) + ".html#" + util::ToUtf8(variable.Id()));
        variableElement->SetAttribute("docPath", docPath);
        std::string extPath = util::Path::Combine(extModulePrefix, util::ToUtf8(variable.Parent()->Id()) + ".html#" + util::ToUtf8(variable.Id()));
        variableElement->SetAttribute("extPath", extPath);
    }
    cmajor::symbols::TypeSymbol* type = variable.GetType();
    Assert(type, "nonnull type expected");
    std::string typeId = GetOrInsertType(type);
    variableElement->SetAttribute("type", typeId);
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(&variable);
    if (node)
    {
        if (node->GetNodeType() == cmajor::ast::NodeType::memberVariableNode)
        {
            cmajor::ast::MemberVariableNode* memberVariableNode = static_cast<cmajor::ast::MemberVariableNode*>(node);
            variableElement->SetAttribute("specifiers", cmajor::ast::SpecifierStr(memberVariableNode->GetSpecifiers()));
        }
    }
    else
    {
        //throw std::runtime_error("variable node expected"); TODO
        return;
    }
    soul::xml::Element* prevElement = currentElement;
    currentElement = variableElement;
    prevElement->AppendChild(variableElement);
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    std::unique_ptr<soul::xml::Element> parensElement(soul::xml::MakeElement("parens"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = parensElement.get();
    parenthesizedConstraintNode.Constraint()->Accept(*this);
    prevElement->AppendChild(parensElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    std::unique_ptr<soul::xml::Element> orElement(soul::xml::MakeElement("or"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = orElement.get();
    disjunctiveConstraintNode.Left()->Accept(*this);
    disjunctiveConstraintNode.Right()->Accept(*this);
    prevElement->AppendChild(orElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    std::unique_ptr<soul::xml::Element> andElement(soul::xml::MakeElement("and"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = andElement.get();
    conjunctiveConstraintNode.Left()->Accept(*this);
    conjunctiveConstraintNode.Right()->Accept(*this);
    prevElement->AppendChild(andElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    std::unique_ptr<soul::xml::Element> whereElement(soul::xml::MakeElement("where"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = whereElement.get();
    whereConstraintNode.Constraint()->Accept(*this);
    prevElement->AppendChild(whereElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode)
{
    std::unique_ptr<soul::xml::Element> predicateElement(soul::xml::MakeElement("predicate"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = predicateElement.get();
    currentElement->SetAttribute("value", predicateConstraintNode.InvokeExpr()->ToString());
    prevElement->AppendChild(predicateElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    std::unique_ptr<soul::xml::Element> isElement(soul::xml::MakeElement("is"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = isElement.get();
    isConstraintNode.TypeExpr()->Accept(*this);
    isConstraintNode.ConceptOrTypeName()->Accept(*this);
    prevElement->AppendChild(isElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    std::unique_ptr<soul::xml::Element> multiElement(soul::xml::MakeElement("multi"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = multiElement.get();
    multiParamConstraintNode.ConceptId()->Accept(*this);
    std::unique_ptr<soul::xml::Element> typesElement(soul::xml::MakeElement("types"));
    soul::xml::Element* prevTypesElement = currentElement;
    currentElement = typesElement.get();
    int n = multiParamConstraintNode.TypeExprs().Count();
    for (int i = 0; i < n; ++i)
    {
        multiParamConstraintNode.TypeExprs()[i]->Accept(*this);
    }
    prevTypesElement->AppendChild(typesElement.release());
    currentElement = prevTypesElement;
    prevElement->AppendChild(multiElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode)
{
    std::unique_ptr<soul::xml::Element> typenameElement(soul::xml::MakeElement("typename"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = typenameElement.get();
    typeNameConstraintNode.TypeId()->Accept(*this);
    prevElement->AppendChild(typenameElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode)
{
    std::unique_ptr<soul::xml::Element> signatureConstraintElement(soul::xml::MakeElement("signature"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = signatureConstraintElement.get();
    signatureConstraintElement->SetAttribute("value", constructorConstraintNode.ToString());
    prevElement->AppendChild(signatureConstraintElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode)
{
    std::unique_ptr<soul::xml::Element> signatureConstraintElement(soul::xml::MakeElement("signature"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = signatureConstraintElement.get();
    signatureConstraintElement->SetAttribute("value", destructorConstraintNode.ToString());
    prevElement->AppendChild(signatureConstraintElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode)
{
    std::unique_ptr<soul::xml::Element> signatureConstraintElement(soul::xml::MakeElement("signature"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = signatureConstraintElement.get();
    signatureConstraintElement->SetAttribute("value", memberFunctionConstraintNode.ToString());
    prevElement->AppendChild(signatureConstraintElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode)
{
    std::unique_ptr<soul::xml::Element> signatureConstraintElement(soul::xml::MakeElement("signature"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = signatureConstraintElement.get();
    signatureConstraintElement->SetAttribute("value", functionConstraintNode.ToString());
    prevElement->AppendChild(signatureConstraintElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode)
{
    std::unique_ptr<soul::xml::Element> axiomStatementElement(soul::xml::MakeElement("axiomStatement"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = axiomStatementElement.get();
    axiomStatementElement->SetAttribute("value", axiomStatementNode.ToString());
    prevElement->AppendChild(axiomStatementElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::AxiomNode& axiomNode)
{
    std::unique_ptr<soul::xml::Element> axiomElement(soul::xml::MakeElement("axiom"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = axiomElement.get();
    if (axiomNode.Id())
    {
        axiomNode.Id()->Accept(*this);
    }
    int np = axiomNode.Parameters().Count();
    if (np > 0)
    {
        std::unique_ptr<soul::xml::Element> parametersElement(soul::xml::MakeElement("parameters"));
        soul::xml::Element* prevElement = currentElement;
        currentElement = parametersElement.get();
        for (int i = 0; i < np; ++i)
        {
            axiomNode.Parameters()[i]->Accept(*this);
        }
        prevElement->AppendChild(parametersElement.release());
        currentElement = prevElement;
    }
    int n = axiomNode.Statements().Count();
    if (n > 0)
    {
        std::unique_ptr<soul::xml::Element> statementsElement(soul::xml::MakeElement("axiomStatements"));
        soul::xml::Element* prevElement = currentElement;
        currentElement = statementsElement.get();
        for (int i = 0; i < n; ++i)
        {
            axiomNode.Statements()[i]->Accept(*this);
        }
        prevElement->AppendChild(statementsElement.release());
        currentElement = prevElement;
    }
    prevElement->AppendChild(axiomElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    conceptIdNode.Id()->Accept(*this);
    std::unique_ptr<soul::xml::Element> typeParamsElement(soul::xml::MakeElement("typeParams"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = typeParamsElement.get();
    int n = conceptIdNode.TypeParameters().Count();
    for (int i = 0; i < n; ++i)
    {
        conceptIdNode.TypeParameters()[i]->Accept(*this);
    }
    prevElement->AppendChild(typeParamsElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    std::unique_ptr<soul::xml::Element> symbolElement(soul::xml::MakeElement("symbol"));
    cmajor::symbols::Symbol* symbol = symbolTable.GetMappedSymbol(&identifierNode);
    if (symbol)
    {
        symbolElement->SetAttribute("ref", util::ToUtf8(symbol->Id()));
    }
    symbolElement->SetAttribute("name", util::ToUtf8(identifierNode.Str()));
    currentElement->AppendChild(symbolElement.release());
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::DotNode& dotNode)
{
    std::unique_ptr<soul::xml::Element> dotElement(soul::xml::MakeElement("dot"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = dotElement.get();
    dotNode.Subject()->Accept(*this);
    dotNode.MemberId()->Accept(*this);
    prevElement->AppendChild(dotElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ParameterNode& parameterNode)
{
    std::unique_ptr<soul::xml::Element> parameterElement(soul::xml::MakeElement("parameter"));
    soul::xml::Element* prevElement = currentElement;
    currentElement = parameterElement.get();
    parameterNode.TypeExpr()->Accept(*this);
    if (parameterNode.Id() && !parameterNode.ArtificialId())
    {
        parameterNode.Id()->Accept(*this);
    }
    prevElement->AppendChild(parameterElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::BoolNode& boolNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "bool");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "sbyte");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ByteNode& byteNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "byte");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ShortNode& shortNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "short");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::UShortNode& ushortNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "ushort");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::IntNode& intNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "int");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::UIntNode& uintNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "uint");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::LongNode& longNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "long");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::ULongNode& ulongNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "ulong");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::FloatNode& floatNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "float");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "double");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::CharNode& charNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "char");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::WCharNode& wcharNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "wchar");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::UCharNode& ucharNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "uchar");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::Visit(cmajor::ast::VoidNode& voidNode)
{
    std::unique_ptr<soul::xml::Element> keywordElement(soul::xml::MakeElement("keyword"));
    keywordElement->SetAttribute("value", "void");
    soul::xml::Element* prevElement = currentElement;
    currentElement = keywordElement.get();
    prevElement->AppendChild(keywordElement.release());
    currentElement = prevElement;
}

void SymbolTableXmlBuilder::GenerateTypes()
{
    soul::xml::Element* typesElement(soul::xml::MakeElement("types"));
    for (cmajor::symbols::TypeSymbol* type : types)
    {
        if (type->IsBasicTypeSymbol())
        {
            soul::xml::Element* typeElement = CreateSymbolElement("basicType", *type);
            typesElement->AppendChild(typeElement);
        }
        else if (type->IsNullPtrTypeSymbol())
        {
            soul::xml::Element* typeElement = CreateSymbolElement("basicType", *type);
            typesElement->AppendChild(typeElement);
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::derivedTypeSymbol)
        {
            cmajor::symbols::DerivedTypeSymbol* derivedTypeSymbol = static_cast<cmajor::symbols::DerivedTypeSymbol*>(type);
            soul::xml::Element* typeElement = CreateSymbolElement("derivedType", *derivedTypeSymbol);
            if (HasFrontConstDerivation(derivedTypeSymbol->DerivationRec().derivations))
            {
                soul::xml::Element* constDerivationElement(soul::xml::MakeElement("derivation"));
                constDerivationElement->SetAttribute("name", "const");
                typeElement->AppendChild(constDerivationElement);
            }
            soul::xml::Element* baseDerivationElement(soul::xml::MakeElement("derivation"));
            baseDerivationElement->SetAttribute("baseType", util::ToUtf8(type->BaseType()->Id()));
            typeElement->AppendChild(baseDerivationElement);
            for (cmajor::symbols::Derivation derivation : derivedTypeSymbol->DerivationRec().derivations)
            {
                switch (derivation)
                {
                case cmajor::symbols::Derivation::lvalueRefDerivation:
                {
                    soul::xml::Element* lvalueRefDerivationElement(soul::xml::MakeElement("derivation"));
                    lvalueRefDerivationElement->SetAttribute("name", "&");
                    typeElement->AppendChild(lvalueRefDerivationElement);
                    break;
                }
                case cmajor::symbols::Derivation::rvalueRefDerivation:
                {
                    soul::xml::Element* rvalueRefDerivationElement(soul::xml::MakeElement("derivation"));
                    rvalueRefDerivationElement->SetAttribute("name", "&&");
                    typeElement->AppendChild(rvalueRefDerivationElement);
                    break;
                }
                case cmajor::symbols::Derivation::pointerDerivation:
                {
                    soul::xml::Element* pointerDerivationElement(soul::xml::MakeElement("derivation"));
                    pointerDerivationElement->SetAttribute("name", "*");
                    typeElement->AppendChild(pointerDerivationElement);
                    break;
                }
                }
            }
            typesElement->AppendChild(typeElement);
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::arrayTypeSymbol)
        {
            cmajor::symbols::ArrayTypeSymbol* arrayTypeSymbol = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type);
            soul::xml::Element* typeElement = CreateSymbolElement("arrayType", *arrayTypeSymbol);
            typeElement->SetAttribute("elementType", util::ToUtf8(arrayTypeSymbol->ElementType()->Id()));
            if (arrayTypeSymbol->Size() != -1)
            {
                typeElement->SetAttribute("size", std::to_string(arrayTypeSymbol->Size()));
            }
            typesElement->AppendChild(typeElement);
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsPrototype())
            {
                soul::xml::Element* typeElement = CreateSymbolElement("specialization", *specialization);
                cmajor::symbols::ClassTypeSymbol* classTemplate = specialization->GetClassTemplate();
                typeElement->SetAttribute("primaryType", util::ToUtf8(classTemplate->Id()));
                soul::xml::Element* argumentTypesElement(soul::xml::MakeElement("templateArgumentTypes"));
                for (cmajor::symbols::TypeSymbol* templateArgumentType : specialization->TemplateArgumentTypes())
                {
                    soul::xml::Element* argumentTypeElement(soul::xml::MakeElement("templateArgumentType"));
                    argumentTypeElement->SetAttribute("ref", util::ToUtf8(templateArgumentType->Id()));
                    argumentTypesElement->AppendChild(argumentTypeElement);
                }
                typeElement->AppendChild(argumentTypesElement);
                typesElement->AppendChild(typeElement);
            }
        }
        else
        {
            throw std::runtime_error("unexpected type symbol");
        }
    }
    symbolTableElement->AppendChild(typesElement);
}

std::string SymbolTableXmlBuilder::GetOrInsertType(cmajor::symbols::TypeSymbol* type)
{
    std::string typeId = util::ToUtf8(type->Id());
    int n = static_cast<int>(moduleXmlDocuments.size());
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Document* moduleDoc = moduleXmlDocuments[i];
        soul::xml::Element* element = moduleDoc->GetElementById(typeId);
        if (element)
        {
            return typeId;
        }
    }
    auto it = typeIdSet.find(typeId);
    if (it != typeIdSet.cend())
    {
        return typeId;
    }
    else
    {
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol ||
            type->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol ||
            type->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol ||
            type->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol ||
            type->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol ||
            type->GetSymbolType() == cmajor::symbols::SymbolType::templateParameterSymbol)
        {
            return typeId;
        }
        else
        {
            if (type->IsBasicTypeSymbol())
            {
                typeIdSet.insert(typeId);
                types.push_back(type);
                return typeId;
            }
            else if (type->IsNullPtrTypeSymbol())
            {
                typeIdSet.insert(typeId);
                types.push_back(type);
                return typeId;
            }
            else if (type->GetSymbolType() == cmajor::symbols::SymbolType::derivedTypeSymbol)
            {
                cmajor::symbols::DerivedTypeSymbol* derivedTypeSymbol = static_cast<cmajor::symbols::DerivedTypeSymbol*>(type);
                GetOrInsertType(derivedTypeSymbol->BaseType());
                typeIdSet.insert(typeId);
                types.push_back(type);
                return typeId;
            }
            else if (type->GetSymbolType() == cmajor::symbols::SymbolType::arrayTypeSymbol)
            {
                cmajor::symbols::ArrayTypeSymbol* arrayTypeSymbol = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type);
                GetOrInsertType(arrayTypeSymbol->ElementType());
                typeIdSet.insert(typeId);
                types.push_back(type);
                return typeId;
            }
            else if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
                if (!specialization->IsPrototype())
                {
                    cmajor::symbols::ClassTypeSymbol* classTemplate = specialization->GetClassTemplate();
                    GetOrInsertType(classTemplate->Prototype());
                    for (cmajor::symbols::TypeSymbol* templateArgumentType : specialization->TemplateArgumentTypes())
                    {
                        GetOrInsertType(templateArgumentType);
                    }
                    typeIdSet.insert(typeId);
                    types.push_back(type);
                    return typeId;
                }
                else
                {
                    return typeId;
                }
            }
            else
            {
                throw std::runtime_error("unexpected type symbol");
            }
        }
    }
}

void SymbolTableXmlBuilder::AddChildren(cmajor::symbols::ContainerSymbol& container)
{
    if (container.GetSymbolType() != cmajor::symbols::SymbolType::classDelegateTypeSymbol && container.GetSymbolType() != cmajor::symbols::SymbolType::delegateTypeSymbol)
    {
        bool added = false;
        std::vector<cmajor::symbols::NamespaceSymbol*> namespaces = GetNamespaces(container);
        if (!namespaces.empty())
        {
            soul::xml::Element* namespacesElement(soul::xml::MakeElement("namespaces"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = namespacesElement;
            for (cmajor::symbols::NamespaceSymbol* namespaceSymbol : namespaces)
            {
                if (AddNamespace(*namespaceSymbol))
                {
                    added = true;
                }
            }
            if (added)
            {
                prevElement->AppendChild(namespacesElement);
            }
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::ConceptSymbol*> concepts = GetConcepts(container);
        if (!concepts.empty())
        {
            soul::xml::Element* conceptsElement(soul::xml::MakeElement("concepts"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = conceptsElement;
            for (cmajor::symbols::ConceptSymbol* conceptSymbol : concepts)
            {
                AddConcept(*conceptSymbol);
            }
            prevElement->AppendChild(conceptsElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::ClassTypeSymbol*> classes = GetClasses(container);
        if (!classes.empty())
        {
            soul::xml::Element* classesElement(soul::xml::MakeElement("classes"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = classesElement;
            for (cmajor::symbols::ClassTypeSymbol* cls : classes)
            {
                AddClass(*cls);
            }
            prevElement->AppendChild(classesElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::InterfaceTypeSymbol*> interfaces = GetInterfaces(container);
        if (!interfaces.empty())
        {
            soul::xml::Element* interfacesElement(soul::xml::MakeElement("interfaces"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = interfacesElement;
            for (cmajor::symbols::InterfaceTypeSymbol* intf : interfaces)
            {
                AddInterface(*intf);
            }
            prevElement->AppendChild(interfacesElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::FunctionSymbol*> ctorsDtors = GetConstructorsAndDestructors(container);
        if (!ctorsDtors.empty())
        {
            soul::xml::Element* ctorsDtorsElement(soul::xml::MakeElement("ctorsDtors"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = ctorsDtorsElement;
            for (cmajor::symbols::FunctionSymbol* function : ctorsDtors)
            {
                AddCtorOrDtor(*function);
            }
            prevElement->AppendChild(ctorsDtorsElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::FunctionSymbol*> functions = GetFunctions(container, moduleName);
        if (!functions.empty())
        {
            soul::xml::Element* functionsElement(soul::xml::MakeElement("functions"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = functionsElement;
            for (cmajor::symbols::FunctionSymbol* function : functions)
            {
                AddFunction(*function);
            }
            prevElement->AppendChild(functionsElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::DelegateTypeSymbol*> delegates = GetDelegates(container);
        if (!delegates.empty())
        {
            soul::xml::Element* delegatesElement(soul::xml::MakeElement("delegates"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = delegatesElement;
            for (cmajor::symbols::DelegateTypeSymbol* delegate : delegates)
            {
                AddDelegate(*delegate);
            }
            prevElement->AppendChild(delegatesElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::ClassDelegateTypeSymbol*> classDelegates = GetClassDelegates(container);
        if (!classDelegates.empty())
        {
            soul::xml::Element* classDelegatesElement(soul::xml::MakeElement("classDelegates"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = classDelegatesElement;
            for (cmajor::symbols::ClassDelegateTypeSymbol* classDelegate : classDelegates)
            {
                AddClassDelegate(*classDelegate);
            }
            prevElement->AppendChild(classDelegatesElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::AliasTypeSymbol*> typedefs = GetTypedefs(container);
        if (!typedefs.empty())
        {
            soul::xml::Element* typedefsElement(soul::xml::MakeElement("typedefs"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = typedefsElement;
            for (cmajor::symbols::AliasTypeSymbol* typedef_ : typedefs)
            {
                AddTypedef(*typedef_);
            }
            prevElement->AppendChild(typedefsElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::EnumTypeSymbol*> enumTypes = GetEnumTypes(container);
        if (!enumTypes.empty())
        {
            soul::xml::Element* enumTypesElement(soul::xml::MakeElement("enumTypes"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = enumTypesElement;
            for (cmajor::symbols::EnumTypeSymbol* enumType : enumTypes)
            {
                AddEnumType(*enumType);
            }
            prevElement->AppendChild(enumTypesElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::EnumConstantSymbol*> enumConstants = GetEnumConstants(container);
        if (!enumConstants.empty())
        {
            soul::xml::Element* enumConstantsElement(soul::xml::MakeElement("enumConstants"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = enumConstantsElement;
            for (cmajor::symbols::EnumConstantSymbol* enumConstant : enumConstants)
            {
                AddEnumConstant(*enumConstant);
            }
            prevElement->AppendChild(enumConstantsElement);
            currentElement = prevElement;
        }
        std::vector<cmajor::symbols::ConstantSymbol*> constants = GetConstants(container);
        if (!constants.empty())
        {
            soul::xml::Element* constantsElement(soul::xml::MakeElement("constants"));
            soul::xml::Element* prevElement = currentElement;
            currentElement = constantsElement;
            for (cmajor::symbols::ConstantSymbol* constant : constants)
            {
                AddConstant(*constant);
            }
            prevElement->AppendChild(constantsElement);
            currentElement = prevElement;
        }
    }
    bool functionOrDelegateContainer = container.IsFunctionSymbol() ||
        container.GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol ||
        container.GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
    bool skipFirst = false;
    if (container.IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(&container);
        if (functionSymbol->IsConstructorDestructorOrNonstaticMemberFunction())
        {
            skipFirst = true;
        }
    }
    std::vector<cmajor::symbols::VariableSymbol*> variables = GetVariables(container, !functionOrDelegateContainer, skipFirst);
    if (!variables.empty())
    {
        std::string varContainerName = "variables";
        if (functionOrDelegateContainer)
        {
            varContainerName = "parameters";
        }
        soul::xml::Element* variablesElement(soul::xml::MakeElement(varContainerName));
        soul::xml::Element* prevElement = currentElement;
        currentElement = variablesElement;
        for (cmajor::symbols::VariableSymbol* variable : variables)
        {
            AddVariable(*variable);
        }
        prevElement->AppendChild(variablesElement);
        currentElement = prevElement;
    }
}

void GenerateSymbolTableXml(cmajor::symbols::Module* rootModule, std::map<int, File>& fileMap)
{
    cmajor::symbols::Context context;
    context.SetRootModule(rootModule);
    cmajor::symbols::SymbolTable& symbolTable = rootModule->GetSymbolTable();
    std::string modulePrefix = util::Path::Combine(util::Path::Combine("../..", util::ToUtf8(rootModule->Name())), "doc");
    std::string extModulePrefix = util::Path::Combine(util::ToUtf8(rootModule->Name()), "doc");
    SymbolTableXmlBuilder symbolTableXmlBuilder(&context, util::ToUtf8(rootModule->Name()), symbolTable, fileMap, modulePrefix, extModulePrefix);
    Input* input = GetInputPtr();
    std::string targetDir = input->targetDirPath;
    std::filesystem::create_directories(targetDir);
    std::string contentDir = util::GetFullPath(util::Path::Combine(targetDir, "content"));
    std::filesystem::create_directories(contentDir);
    std::string moduleDir = util::GetFullPath(util::Path::Combine(contentDir, util::ToUtf8(rootModule->Name())));
    std::filesystem::create_directories(moduleDir);
    symbolTableXmlBuilder.AddNamespace(symbolTable.GlobalNs());
    std::string symbolTableXmlFilePath = util::Path::Combine(moduleDir, util::ToUtf8(rootModule->Name()) + ".xml");
    symbolTableXmlBuilder.WriteDocument(symbolTableXmlFilePath);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(rootModule->LogStreamId(), "==> " + symbolTableXmlFilePath);
    }
}

soul::xml::Document* GetModuleDocument(Input* input, const std::string& moduleName)
{
    for (const auto& p : input->externalModuleDocs)
    {
        soul::xml::Document* doc = p.get();
        if (doc->DocumentElement()->GetAttribute("module") == moduleName)
        {
            return doc;
        }
    }
    return nullptr;
}

std::pair<std::string, bool> ResolveModuleXmlDocFilePath(Input* input, const std::string& moduleName)
{
    bool isExternalModuleDocFilePath = false;
    auto it = input->libraryPrefixMap.find(moduleName);
    if (it != input->libraryPrefixMap.cend())
    {
        std::string libraryPrefix = it->second;
        if (!libraryPrefix.empty())
        {
            isExternalModuleDocFilePath = true;
        }
        else
        {
            libraryPrefix = "content";
        }
        std::string moduleDir = util::Path::Combine(util::Path::Combine(input->baseDir, libraryPrefix), moduleName);
        if (std::filesystem::exists(moduleDir))
        {
            std::string moduleXmlDocFilePath = util::Path::Combine(moduleDir, moduleName + ".xml");
            if (std::filesystem::exists(moduleXmlDocFilePath))
            {
                return std::make_pair(moduleXmlDocFilePath, isExternalModuleDocFilePath);
            }
            else
            {
                throw std::runtime_error("cmdoc module xml document '" + moduleXmlDocFilePath + "' not found");
            }
        }
    }
    return std::pair<std::string, bool>();
}

void ReadExternalModuleDocuments(Input* input)
{
    for (const auto& p : input->libraryPrefixMap)
    {
        if (!p.second.empty())
        {
            const std::string& moduleName = p.first;
            soul::xml::Document* moduleDoc = GetModuleDocument(input, moduleName);
            if (!moduleDoc)
            {
                std::pair<std::string, bool> refModuleXmlDocFilePathExternal = ResolveModuleXmlDocFilePath(input, moduleName);
                std::unique_ptr<soul::xml::Document> refModuleXmlDoc = soul::xml::ParseXmlFile(refModuleXmlDocFilePathExternal.first);
                input->externalModuleDocs.push_back(std::move(refModuleXmlDoc));
            }
        }
    }
}

} // namespace cmdoclib
