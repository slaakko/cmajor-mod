// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol.table;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.namespace_symbol;
import cmajor.fault.tolerant.symbols.derivations;
import cmajor.fault.tolerant.symbols.array.type.compare;
import cmajor.fault.tolerant.symbols.array.type.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class ErrorContainer;
class TypeSymbol;
class BasicTypeSymbol;
class CompoundTypeSymbol;
class DeclarationBlockSymbol;

class SymbolTable
{
public:
    SymbolTable();
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_) { name = name_; }
    GlobalNamespaceSymbol* GlobalNs() { return &globalNs; }
    ContainerSymbol* Container() { return container; }
    void Import(const SymbolTable& that);
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    void BeginContainer(ContainerSymbol* container_);
    void EndContainer();
    bool BeginNamespace(cmajor::fault::tolerant::ast::IdentifierNode* namespaceName, Source* source, ErrorContainer* errors, bool map);
    void EndNamespace();
    void AddAliasType(cmajor::fault::tolerant::ast::UsingAliasNode& node, Source* source);
    void AddAliasType(cmajor::fault::tolerant::ast::TypedefAliasNode& node, Source* source);
    void AddConstant(cmajor::fault::tolerant::ast::ConstantNode& node, Source* source);
    void BeginDelegate(cmajor::fault::tolerant::ast::DelegateNode& node, Source* source);
    void EndDelegate();
    void BeginClassDelegate(cmajor::fault::tolerant::ast::ClassDelegateNode& node, Source* source);
    void EndClassDelegate();
    void BeginEnumType(cmajor::fault::tolerant::ast::EnumTypeNode& node, Source* source);
    void EndEnumType();
    void AddEnumConstant(cmajor::fault::tolerant::ast::EnumConstantNode& node, Source* source);
    void BeginDeclarationBlock(cmajor::fault::tolerant::ast::CompoundStatementNode& node, Source* source, int& count);
    void EndDeclarationBlock();
    void AddParameter(cmajor::fault::tolerant::ast::ParameterNode& node, int& paramIndex, Source* source);
    void AddLocalVariable(cmajor::fault::tolerant::ast::ConstructionStatementNode& node, Source* source);
    void AddMemberVariable(cmajor::fault::tolerant::ast::MemberVariableNode& node, Source* source);
    void AddGlobalVariable(cmajor::fault::tolerant::ast::GlobalVariableNode& node, Source* source);
    void AddTemplateParameter(cmajor::fault::tolerant::ast::TemplateParameterNode& node, Source* source);
    void BeginFunction(cmajor::fault::tolerant::ast::FunctionNode& node, Source* source);
    void EndFunction();
    void BeginStaticConstructor(cmajor::fault::tolerant::ast::StaticConstructorNode& node, Source* source);
    void EndStaticConstructor();
    void BeginConstructor(cmajor::fault::tolerant::ast::ConstructorNode& node, Source* source);
    void EndConstructor();
    void BeginDestructor(cmajor::fault::tolerant::ast::DestructorNode& node, Source* source);
    void EndDestructor();
    void BeginMemberFunction(cmajor::fault::tolerant::ast::MemberFunctionNode& node, Source* source);
    void EndMemberFunction();
    void BeginConversionFunction(cmajor::fault::tolerant::ast::ConversionFunctionNode& node, Source* source);
    void EndConversionFunction();
    void BeginClass(cmajor::fault::tolerant::ast::ClassNode& node, Source* source);
    void EndClass();
    void BeginInterface(cmajor::fault::tolerant::ast::InterfaceNode& node, Source* source);
    void EndInterface();
    void BeginConcept(cmajor::fault::tolerant::ast::ConceptNode& node, Source* source);
    void EndConcept();
    void AddTypeParameter(cmajor::fault::tolerant::ast::IdentifierNode& node, Source* source);
    void MapNode(cmajor::fault::tolerant::ast::Node* node, Symbol* symbol);
    Symbol* GetMappedSymbol(cmajor::fault::tolerant::ast::Node* node) const;
    void MapSymbol(Symbol* symbol, cmajor::fault::tolerant::ast::Node* node);
    cmajor::fault::tolerant::ast::Node* GetMappedNode(Symbol* symbol) const;
    void AddSource(Source* source);
    Source* GetSource(int index) const;
    void ResetSource(int index, Source* source);
    Source* GetSource(const std::string& filePath) const;
    void MapType(TypeSymbol* type);
    TypeSymbol* GetType(const util::uuid& id) const;
    void AddBasicTypeToGlobalNamespace(BasicTypeSymbol* basicType);
    void AddBasicType(BasicTypeSymbol* basicType);
    BasicTypeSymbol* GetBasicType(SymbolKind symbolKind) const;
    TypeSymbol* MakeCompoundType(TypeSymbol* baseType, const Derivations& derivations);
    void MakeCompoundTypeMap();
    TypeSymbol* MakeArrayType(TypeSymbol* elementType, int64_t size);
    void MakeArrayTypeSet();
    const std::vector<std::unique_ptr<Source>>& Sources() const { return sources; }
    void Compact();
private:
    std::string name;
    std::vector<std::unique_ptr<Source>> sources;
    std::map<std::string, Source*> sourceMap;
    GlobalNamespaceSymbol globalNs;
    ContainerSymbol* container;
    std::stack<ContainerSymbol*> containerStack;
    std::map<cmajor::fault::tolerant::ast::Node*, Symbol*> nodeSymbolMap;
    std::map<Symbol*, cmajor::fault::tolerant::ast::Node*> symbolNodeMap;
    std::map<SymbolKind, BasicTypeSymbol*> basicTypeMap;
    std::map<TypeSymbol*, std::vector<CompoundTypeSymbol*>> compoundTypeMap;
    std::vector<std::unique_ptr<CompoundTypeSymbol>> compoundTypes;
    std::set<ArrayTypeSymbol*, ArrayTypeLess>  arrayTypeSet;
    std::vector<std::unique_ptr<ArrayTypeSymbol>> arrayTypes;
    std::map<util::uuid, TypeSymbol*> typeMap;
};

void InitCoreSymbolTable(SymbolTable* symbolTable);

} // namespace cmajor::fault::tolerant::symbols
