// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.symbol.table;

import cmajor.symbols.derived.type.symbol;
import cmajor.symbols.namespaces;
import cmajor.symbols.conversion.table;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.type.symbol;
import cmajor.ast;
import soul.ast.span;
import soul.xml.document;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class Module;
class FunctionSymbol;
class ClassTypeSymbol;
class ConceptSymbol;
class AliasTypeSymbol;
class ConstantSymbol;
class ArrayTypeSymbol;
class ClassTemplateSpecializationSymbol;
class GlobalVariableSymbol;

struct TypeOrConceptRequest
{
    TypeOrConceptRequest(Symbol* symbol_, const util::uuid& typeId_, int index_) : symbol(symbol_), typeId(typeId_), index(index_) {}
    Symbol* symbol;
    util::uuid typeId;
    int index;
};

struct FunctionRequest
{
    FunctionRequest(Symbol* symbol_, const util::uuid& functionId_, int index_) : symbol(symbol_), functionId(functionId_), index(index_) {}
    Symbol* symbol;
    util::uuid functionId;
    int index;
};

struct ClassTemplateSpecializationKey
{
    ClassTemplateSpecializationKey(ClassTypeSymbol* classTemplate_, const std::vector<TypeSymbol*>& templateArgumentTypes_) :
        classTemplate(classTemplate_), templateArgumentTypes(templateArgumentTypes_)
    {
    }
    ClassTypeSymbol* classTemplate;
    std::vector<TypeSymbol*> templateArgumentTypes;
};

struct ClassTemplateSpecializationKeyHash
{
    size_t operator()(const ClassTemplateSpecializationKey& key) const;
};

bool operator==(const ClassTemplateSpecializationKey& left, const ClassTemplateSpecializationKey& right);
bool operator!=(const ClassTemplateSpecializationKey& left, const ClassTemplateSpecializationKey& right);

struct ArrayKey
{
    ArrayKey(TypeSymbol* elementType_, int64_t size_) : elementType(elementType_), size(size_) {}
    TypeSymbol* elementType;
    int64_t size;
};

bool operator==(const ArrayKey& left, const ArrayKey& right);
bool operator!=(const ArrayKey& left, const ArrayKey& right);

struct ArrayKeyHash
{
    size_t operator()(const ArrayKey& key) const;
};

void MapIdentifierToSymbolDefinition(cmajor::ast::IdentifierNode* identifierNode, Symbol* symbol);

class SymbolTable
{
public:
    SymbolTable(Module* module_, Context* context);
    ~SymbolTable();
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    void Import(const SymbolTable& symbolTable, Context* context);
    void FinishRead(const std::vector<ArrayTypeSymbol*>& arrayTypes, const std::vector<DerivedTypeSymbol*>& derivedTypes,
        const std::vector<ClassTemplateSpecializationSymbol*>& classTemplateSpecializations,
        const std::vector<TypeOrConceptRequest>& typeAndConceptRequests, const std::vector<FunctionRequest>& functionRequests, std::vector<FunctionSymbol*>& conversions);
    void Clear();
    const NamespaceSymbol& GlobalNs() const { return globalNs; }
    NamespaceSymbol& GlobalNs() { return globalNs; }
    const ContainerSymbol* Container() const { return container; }
    ContainerSymbol* Container() { return container; }
    void SetCurrentCompileUnit(cmajor::ast::CompileUnitNode* currentCompileUnit_) { currentCompileUnit = currentCompileUnit_; }
    void BeginContainer(ContainerSymbol* container_);
    void EndContainer();
    void MapNs(NamespaceSymbol* fromNs, NamespaceSymbol* toNs);
    NamespaceSymbol* GetMappedNs(NamespaceSymbol* fromNs) const;
    NamespaceSymbol* BeginNamespace(cmajor::ast::NamespaceNode& namespaceNode, Context* context);
    NamespaceSymbol* BeginNamespace(const std::u32string& namespaceName, const soul::ast::Span& span, const util::uuid& moduleId, int32_t fileIndex, Context* context);
    void EndNamespace();
    void BeginFunction(cmajor::ast::FunctionNode& functionNode, int32_t functionIndex);
    void EndFunction(bool addMember, Context* context);
    void AddParameter(cmajor::ast::ParameterNode& parameterNode, Context* context);
    void BeginClass(cmajor::ast::ClassNode& classNode);
    void EndClass(Context* context);
    void BeginClassTemplateSpecialization(cmajor::ast::ClassNode& classInstanceNode, ClassTemplateSpecializationSymbol* classTemplateSpecialization);
    void EndClassTemplateSpecialization();
    void AddTemplateParameter(cmajor::ast::TemplateParameterNode& templateParameterNode, Context* context);
    void AddTemplateParameter(cmajor::ast::IdentifierNode& identifierNode, Context* context);
    void BeginInterface(cmajor::ast::InterfaceNode& interfaceNode, Context* context);
    void EndInterface();
    void BeginStaticConstructor(cmajor::ast::StaticConstructorNode& staticConstructorNode, int32_t functionIndex);
    void EndStaticConstructor(bool addMember, Context* context);
    void BeginConstructor(cmajor::ast::ConstructorNode& constructorNode, int32_t functionIndex, Context* context);
    void EndConstructor(bool addMember, Context* context);
    void BeginDestructor(cmajor::ast::DestructorNode& destructorNode, int32_t functionIndex, Context* context);
    void EndDestructor(bool addMember, Context* context);
    void BeginMemberFunction(cmajor::ast::MemberFunctionNode& memberFunctionNode, int32_t functionIndex, Context* context);
    void EndMemberFunction(bool addMember, Context* context);
    void BeginConversionFunction(cmajor::ast::ConversionFunctionNode& conversionFunctionNode, int32_t functionIndex, Context* context);
    void EndConversionFunction(bool addMember, Context* context);
    void AddMemberVariable(cmajor::ast::MemberVariableNode& memberVariableNode, Context* context);
    void BeginDelegate(cmajor::ast::DelegateNode& delegateNode, Context* context);
    void EndDelegate();
    void BeginClassDelegate(cmajor::ast::ClassDelegateNode& classDelegateNode, Context* context);
    void EndClassDelegate();
    void BeginConcept(cmajor::ast::ConceptNode& conceptNode, bool hasSource);
    void EndConcept(Context* context);
    void BeginAxiom(cmajor::ast::AxiomNode& axiomNode);
    void EndAxiom(Context* context);
    void BeginDeclarationBlock(cmajor::ast::Node& node, Context* context);
    void EndDeclarationBlock();
    void AddLocalVariable(cmajor::ast::ConstructionStatementNode& constructionStatementNode, Context* context);
    void AddLocalVariable(cmajor::ast::IdentifierNode& identifierNode, Context* context);
    AliasTypeSymbol* AddAliasType(cmajor::ast::AliasNode& aliasNode, Context* context);
    AliasTypeSymbol* AddAliasType(cmajor::ast::TypedefNode& typedefNode, Context* context);
    ConstantSymbol* AddConstant(cmajor::ast::ConstantNode& constantNode, Context* context);
    GlobalVariableSymbol* AddGlobalVariable(cmajor::ast::GlobalVariableNode& globalVariableNode, Context* context);
    void BeginEnumType(cmajor::ast::EnumTypeNode& enumTypeNode, Context* context);
    void EndEnumType();
    void AddEnumConstant(cmajor::ast::EnumConstantNode& enumConstantNode, Context* context);
    void AddTypeSymbolToGlobalScope(TypeSymbol* typeSymbol, Context* context);
    void AddTypeSymbol(TypeSymbol* typeSymbol, Context* context);
    void AddFunctionSymbolToGlobalScope(FunctionSymbol* functionSymbol, Context* context);
    void MapNode(cmajor::ast::Node* node, Symbol* symbol);
    Symbol* GetSymbolNoThrow(cmajor::ast::Node* node) const;
    Symbol* GetSymbol(cmajor::ast::Node* node) const;
    cmajor::ast::Node* GetNodeNoThrow(Symbol* symbol) const;
    cmajor::ast::Node* GetNode(Symbol* symbol) const;
    void SetTypeIdFor(TypeSymbol* typeSymbol);
    void SetTypeIdFor(ConceptSymbol* conceptSymbol);
    void SetFunctionIdFor(FunctionSymbol* functionSymbol);
    FunctionSymbol* GetFunctionById(const util::uuid& functionId) const;
    void AddTypeOrConceptSymbolToTypeIdMap(Symbol* typeOrConceptSymbol);
    void AddFunctionSymbolToFunctionIdMap(FunctionSymbol* functionSymbol);
    void EmplaceTypeRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId, int index);
    void EmplaceConceptRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId);
    void EmplaceFunctionRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& functionId, int index);
    void ProcessTypeConceptAndFunctionRequests(const std::vector<TypeOrConceptRequest>& typeAndConceptRequests, const std::vector<FunctionRequest>& functionRequests);
    TypeSymbol* GetTypeByNameNoThrow(const std::u32string& typeName) const;
    TypeSymbol* GetTypeByName(const std::u32string& typeName) const;
    TypeSymbol* MakeDerivedType(TypeSymbol* baseType, const TypeDerivationRec& derivationRec, Context* context);
    ClassTemplateSpecializationSymbol* MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<TypeSymbol*>& templateArgumentTypes);
    ClassTemplateSpecializationSymbol* CopyClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source);
    ClassTemplateSpecializationSymbol* GetCurrentClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source);
    void AddClassTemplateSpecializationsToClassTemplateSpecializationMap(const std::vector<ClassTemplateSpecializationSymbol*>& classTemplateSpecializations);
    ArrayTypeSymbol* MakeArrayType(TypeSymbol* elementType, int64_t size, Context* context);
    const FunctionSymbol* MainFunctionSymbol() const { return mainFunctionSymbol; }
    FunctionSymbol* MainFunctionSymbol() { return mainFunctionSymbol; }
    void AddConversion(FunctionSymbol* conversion);
    void AddConversion(FunctionSymbol* conversion, Module* module);
    FunctionSymbol* GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType, Context* context) const;
    void AddPolymorphicClass(ClassTypeSymbol* polymorphicClass);
    const std::set<ClassTypeSymbol*>& PolymorphicClasses() const { return polymorphicClasses; }
    void AddClassHavingStaticConstructor(ClassTypeSymbol* classHavingStaticConstructor);
    const std::set<ClassTypeSymbol*>& ClassesHavingStaticConstructor() const { return classesHavingStaticConstructor; }
    void AddJsonClass(const std::u32string& jsonClass);
    const std::set<std::u32string>& JsonClasses() const { return jsonClasses; }
    std::vector<TypeSymbol*> Types() const;
    void Copy(const SymbolTable& that);
    ClassTypeSymbol* CurrentClass() { return currentClass; }
    InterfaceTypeSymbol* CurrentInterface() { return currentInterface; }
    void SetCurrentClass(ClassTypeSymbol* currentClass_) { currentClass = currentClass_; }
    void SetCurrentFunctionSymbol(FunctionSymbol* currentFunctionSymbol_) { currentFunctionSymbol = currentFunctionSymbol_; }
    void MapProfiledFunction(const util::uuid& functionId, const std::u32string& profiledFunctionName);
    std::u32string GetProfiledFunctionName(const util::uuid& functionId) const;
    Module* GetModule() { return module; }
    std::unique_ptr<soul::xml::Document> ToDomDocument();
    void MapInvoke(cmajor::ast::IdentifierNode* invokeId, FunctionSymbol* functionSymbol);
    FunctionSymbol* GetInvoke(cmajor::ast::IdentifierNode* invokeId) const;
    void MapSymbol(cmajor::ast::Node* node, Symbol* symbol);
    Symbol* GetMappedSymbol(cmajor::ast::Node* node) const;
    void MapIdentifierToSymbolDefinition(cmajor::ast::IdentifierNode* identifierNode, Module* module, Symbol* symbol);
    SymbolLocation GetDefinitionLocation(const SymbolLocation& identifierLocation) const;
    void InitUuids();
    const util::uuid& GetDerivationId(Derivation derivation) const;
    const util::uuid& GetPositionId(int index) const;
    int NumSpecializations() const { return classTemplateSpecializationMap.size(); }
    int NumSpecializationsNew() const { return numSpecializationsNew; }
    int NumSpecializationsCopied() const { return numSpecializationsCopied; }
    void Check();
    FunctionSymbol* GetCreatedFunctionSymbol() { return createdFunctionSymbol; }
    void AddFunctionSymbol(std::unique_ptr<FunctionSymbol>&& functionSymbol);
    int NextAxiomNumber() { return axiomNumber++; }
    void ResetAxiomNumber() { axiomNumber = 0; }
    void ResetAliasNodesAndNamespaceImports();
    void ResetMainFunctionSymbol() { mainFunctionSymbol = nullptr; }
    const std::vector<cmajor::ast::AliasNode*>& AliasNodes() const { return aliasNodes; }
    void AddAliasNode(cmajor::ast::AliasNode* aliasNode) { aliasNodes.push_back(aliasNode); }
    const std::vector<cmajor::ast::NamespaceImportNode*>& NamespaceImports() const { return namespaceImports; }
    void AddNamespaceImport(cmajor::ast::NamespaceImportNode* namespaceImport) { namespaceImports.push_back(namespaceImport); }
    void PushInstantiatingTemplate();
    void PushCurrentFileIndex(int32_t currentFileIndex_);
    void PushCurrentModuleId(const util::uuid& currentModuleId_);
    void PopInstantiatingTemplate();
    void PopCurrentFileIndex();
    void PopCurrentModuleId();
private:
    Module* module;
    std::vector<util::uuid> derivationIds;
    std::vector<util::uuid> positionIds;
    NamespaceSymbol globalNs;
    std::vector<cmajor::ast::AliasNode*> aliasNodes;
    std::vector<cmajor::ast::NamespaceImportNode*> namespaceImports;
    std::unordered_map<cmajor::ast::IdentifierNode*, Symbol*> identifierSymbolDefinitionMap;
    std::map<SymbolLocation, SymbolLocation> symbolDefinitionMap;
    cmajor::ast::CompileUnitNode* currentCompileUnit;
    ContainerSymbol* container;
    ContainerSymbol* cursorContainer;
    ClassTypeSymbol* currentClass;
    std::stack<ClassTypeSymbol*> currentClassStack;
    InterfaceTypeSymbol* currentInterface;
    std::stack<InterfaceTypeSymbol*> currentInterfaceStack;
    std::stack<ContainerSymbol*> containerStack;
    FunctionSymbol* mainFunctionSymbol;
    FunctionSymbol* currentFunctionSymbol;
    FunctionSymbol* createdFunctionSymbol;
    int parameterIndex;
    int declarationBlockIndex;
    int axiomNumber;
    std::unordered_map<util::uuid, FunctionSymbol*, util::UuidHash> functionMap;
    std::map<NamespaceSymbol*, NamespaceSymbol*> nsMap;
    std::unordered_map<cmajor::ast::Node*, Symbol*> nodeSymbolMap;
    std::unordered_map<Symbol*, cmajor::ast::Node*> symbolNodeMap;
    std::unordered_map<util::uuid, Symbol*, util::UuidHash> typeIdMap;
    std::unordered_map<util::uuid, FunctionSymbol*, util::UuidHash> functionIdMap;
    std::unordered_map<std::u32string, TypeSymbol*> typeNameMap;
    std::unordered_map<util::uuid, std::u32string, util::UuidHash> profiledFunctionNameMap;
    std::unordered_map<util::uuid, std::vector<DerivedTypeSymbol*>, util::UuidHash> derivedTypeMap;
    std::vector<std::unique_ptr<DerivedTypeSymbol>> derivedTypes;
    std::unordered_map<ClassTemplateSpecializationKey, ClassTemplateSpecializationSymbol*, ClassTemplateSpecializationKeyHash> classTemplateSpecializationMap;
    std::vector<std::unique_ptr<ClassTemplateSpecializationSymbol>> classTemplateSpecializations;
    std::unordered_map<ClassTemplateSpecializationSymbol*, ClassTemplateSpecializationSymbol*> specializationCopyMap;
    std::unordered_map<ArrayKey, ArrayTypeSymbol*, ArrayKeyHash> arrayTypeMap;
    std::vector<std::unique_ptr<ArrayTypeSymbol>> arrayTypes;
    std::unordered_map<cmajor::ast::IdentifierNode*, FunctionSymbol*> invokeMap;
    std::unordered_map<cmajor::ast::Node*, Symbol*> mappedNodeSymbolMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functionSymbols;
    ConversionTable conversionTable;
    std::set<ClassTypeSymbol*> polymorphicClasses;
    std::set<ClassTypeSymbol*> classesHavingStaticConstructor;
    std::set<std::u32string> jsonClasses;
    int numSpecializationsCopied;
    int numSpecializationsNew;
    bool instantiatingTemplate;
    std::stack<bool> instantiatingTemplateStack;
    int32_t currentFileIndex;
    std::stack<int32_t> fileIndexStack;
    util::uuid currentModuleId;
    std::stack<util::uuid> moduleIdStack;
    int GetNextDeclarationBlockIndex() { return declarationBlockIndex++; }
    void ResetDeclarationBlockIndex() { declarationBlockIndex = 0; }
    void EmplaceTypeOrConceptRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId, int index);
    void WriteSymbolDefinitionMap(SymbolWriter& writer);
    void ReadSymbolDefinitionMap(SymbolReader& reader);
};

void InitCoreSymbolTable(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, cmajor::ast::CompileUnitNode* rootCompileUnit, Context* context);

} // namespace cmajor::symbols

