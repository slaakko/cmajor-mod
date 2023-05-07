// =================================
// Copyright (c) 2023 Seppo Laakko
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
import soul.ast.source.pos;
import soul.xml.document;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class Module;
class FunctionSymbol;
class ClassTypeSymbol;
class ConceptSymbol;
class TypedefSymbol;
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

/*
struct ConversionTableEntryLess
{
    bool operator()(const ConversionTableEntry& left, const ConversionTableEntry& right) const
    {
        if (left.sourceType < right.sourceType)
        {
            return true;
        }
        else if (left.sourceType > right.sourceType)
        {
            return false;
        }
        else
        {
            return left.targetType < right.targetType;
        }
    }
};
*/
/*
struct ClassTemplateSpecializationKeyHash
{
    size_t operator()(const ClassTemplateSpecializationKey& key) const
    {
        size_t x = boost::hash<util::uuid>()(key.classTemplate->TypeId());
        int n = key.templateArgumentTypes.size();
        for (int i = 0; i < n; ++i)
        {
            x = x ^ boost::hash<util::uuid>()(key.templateArgumentTypes[i]->TypeId());
        }
        return x;
    }
};
*/
struct ClassTemplateSpecializationKeyLess
{
    bool operator()(const ClassTemplateSpecializationKey& left, const ClassTemplateSpecializationKey& right) const
    { // TODO
        return true;
    }
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

/*
struct ArrayKeyHash
{
    size_t operator()(const ArrayKey& key) const
    {
        size_t x = boost::hash<util::uuid>()(key.elementType->TypeId());
        x = x ^ std::hash<int64_t>()(key.size);
        return x;
    }
};
*/
struct ArrayKeyLess
{
    bool operator()(const ArrayKey& left, const ArrayKey& right) const
    {
        if (left.elementType < right.elementType)
        {
            return true;
        }
        else if (left.elementType > right.elementType)
        {
            return false;
        }
        else
        { 
            return false;
        }
    }
};

void MapIdentifierToSymbolDefinition(cmajor::ast::IdentifierNode* identifierNode, Symbol* symbol);

class SymbolTable
{
public:
    SymbolTable(Module* module_);
    ~SymbolTable();
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    void Import(const SymbolTable& symbolTable);
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
    NamespaceSymbol* BeginNamespace(cmajor::ast::NamespaceNode& namespaceNode);
    NamespaceSymbol* BeginNamespace(const std::u32string& namespaceName, const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId);
    void EndNamespace();
    void BeginFunction(cmajor::ast::FunctionNode& functionNode, int32_t functionIndex);
    void EndFunction(bool addMember);
    void AddParameter(cmajor::ast::ParameterNode& parameterNode);
    void BeginClass(cmajor::ast::ClassNode& classNode);
    void EndClass();
    void BeginClassTemplateSpecialization(cmajor::ast::ClassNode& classInstanceNode, ClassTemplateSpecializationSymbol* classTemplateSpecialization);
    void EndClassTemplateSpecialization();
    void AddTemplateParameter(cmajor::ast::TemplateParameterNode& templateParameterNode);
    void AddTemplateParameter(cmajor::ast::IdentifierNode& identifierNode);
    void BeginInterface(cmajor::ast::InterfaceNode& interfaceNode);
    void EndInterface();
    void BeginStaticConstructor(cmajor::ast::StaticConstructorNode& staticConstructorNode, int32_t functionIndex);
    void EndStaticConstructor(bool addMember);
    void BeginConstructor(cmajor::ast::ConstructorNode& constructorNode, int32_t functionIndex);
    void EndConstructor(bool addMember);
    void BeginDestructor(cmajor::ast::DestructorNode& destructorNode, int32_t functionIndex);
    void EndDestructor(bool addMember);
    void BeginMemberFunction(cmajor::ast::MemberFunctionNode& memberFunctionNode, int32_t functionIndex);
    void EndMemberFunction(bool addMember);
    void BeginConversionFunction(cmajor::ast::ConversionFunctionNode& conversionFunctionNode, int32_t functionIndex);
    void EndConversionFunction(bool addMember);
    void AddMemberVariable(cmajor::ast::MemberVariableNode& memberVariableNode);
    void BeginDelegate(cmajor::ast::DelegateNode& delegateNode);
    void EndDelegate();
    void BeginClassDelegate(cmajor::ast::ClassDelegateNode& classDelegateNode);
    void EndClassDelegate();
    void BeginConcept(cmajor::ast::ConceptNode& conceptNode, bool hasSource);
    void EndConcept();
    void BeginAxiom(cmajor::ast::AxiomNode& axiomNode);
    void EndAxiom();
    void BeginDeclarationBlock(cmajor::ast::Node& node);
    void EndDeclarationBlock();
    void AddLocalVariable(cmajor::ast::ConstructionStatementNode& constructionStatementNode);
    void AddLocalVariable(cmajor::ast::IdentifierNode& identifierNode);
    TypedefSymbol* AddTypedef(cmajor::ast::TypedefNode& typedefNode);
    ConstantSymbol* AddConstant(cmajor::ast::ConstantNode& constantNode);
    GlobalVariableSymbol* AddGlobalVariable(cmajor::ast::GlobalVariableNode& globalVariableNode);
    void BeginEnumType(cmajor::ast::EnumTypeNode& enumTypeNode);
    void EndEnumType();
    void AddEnumConstant(cmajor::ast::EnumConstantNode& enumConstantNode);
    void AddTypeSymbolToGlobalScope(TypeSymbol* typeSymbol);
    void AddFunctionSymbolToGlobalScope(FunctionSymbol* functionSymbol);
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
    TypeSymbol* MakeDerivedType(TypeSymbol* baseType, const TypeDerivationRec& derivationRec, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    ClassTemplateSpecializationSymbol* MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<TypeSymbol*>& templateArgumentTypes,
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    ClassTemplateSpecializationSymbol* CopyClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source);
    ClassTemplateSpecializationSymbol* GetCurrentClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source);
    void AddClassTemplateSpecializationsToClassTemplateSpecializationMap(const std::vector<ClassTemplateSpecializationSymbol*>& classTemplateSpecializations);
    ArrayTypeSymbol* MakeArrayType(TypeSymbol* elementType, int64_t size, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    const FunctionSymbol* MainFunctionSymbol() const { return mainFunctionSymbol; }
    FunctionSymbol* MainFunctionSymbol() { return mainFunctionSymbol; }
    void AddConversion(FunctionSymbol* conversion);
    void AddConversion(FunctionSymbol* conversion, Module* module);
    FunctionSymbol* GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) const;
    ConversionTable& GetConversionTable() { return conversionTable; }
    const ConversionTable& GetConversionTable() const { return conversionTable; }
    void AddPolymorphicClass(ClassTypeSymbol* polymorphicClass);
    const std::unordered_set<ClassTypeSymbol*>& PolymorphicClasses() const { return polymorphicClasses; }
    void AddClassHavingStaticConstructor(ClassTypeSymbol* classHavingStaticConstructor);
    const std::unordered_set<ClassTypeSymbol*>& ClassesHavingStaticConstructor() const { return classesHavingStaticConstructor; }
    void AddJsonClass(const std::u32string& jsonClass);
    const std::unordered_set<std::u32string>& JsonClasses() const { return jsonClasses; }
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
    SymbolLocation* GetDefinitionLocation(const SymbolLocation& identifierLocation);
    void InitUuids();
    const util::uuid& GetDerivationId(Derivation derivation) const;
    const util::uuid& GetPositionId(int index) const;
    int NumSpecializations() const { return classTemplateSpecializationMap.size(); }
    int NumSpecializationsNew() const { return numSpecializationsNew; }
    int NumSpecializationsCopied() const { return numSpecializationsCopied; }
    void Check();
    FunctionSymbol* GetCreatedFunctionSymbol() { return createdFunctionSymbol; }
    void AddFunctionSymbol(std::unique_ptr<FunctionSymbol>&& functionSymbol);
    void ResetCursorContainer() { cursorContainer = nullptr; }
    ContainerSymbol* CursorContainer() const { return cursorContainer; }
    void SetCursorContainer(const cmajor::ast::Node& node);
    int NextAxiomNumber() { return axiomNumber++; }
    void ResetAxiomNumber() { axiomNumber = 0; }
    void ResetAliasNodesAndNamespaceImports();
    void ResetMainFunctionSymbol() { mainFunctionSymbol = nullptr; }
    const std::vector<cmajor::ast::AliasNode*>& AliasNodes() const { return aliasNodes; }
    void AddAliasNode(cmajor::ast::AliasNode* aliasNode) { aliasNodes.push_back(aliasNode); }
    const std::vector<cmajor::ast::NamespaceImportNode*>& NamespaceImports() const { return namespaceImports; }
    void AddNamespaceImport(cmajor::ast::NamespaceImportNode* namespaceImport) { namespaceImports.push_back(namespaceImport); }
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

// TOOD
    //std::unordered_map<util::uuid, FunctionSymbol*, boost::hash<util::uuid>> functionMap;
    //std::map<util::uuid, FunctionSymbol*> functionMap;

    //std::unordered_map<NamespaceSymbol*, NamespaceSymbol*> nsMap;
    std::map<NamespaceSymbol*, NamespaceSymbol*> nsMap;
    std::unordered_map<cmajor::ast::Node*, Symbol*> nodeSymbolMap;
    std::unordered_map<Symbol*, cmajor::ast::Node*> symbolNodeMap;
    //std::unordered_map<util::uuid, Symbol*, boost::hash<util::uuid>> typeIdMap;
    std::map<util::uuid, Symbol*> typeIdMap;
    //std::unordered_map<util::uuid, FunctionSymbol*, boost::hash<util::uuid>> functionIdMap;
    std::map<util::uuid, FunctionSymbol*> functionIdMap;
    std::unordered_map<std::u32string, TypeSymbol*> typeNameMap;
    //std::unordered_map<util::uuid, std::u32string, boost::hash<util::uuid>> profiledFunctionNameMap;
    std::map<util::uuid, std::u32string> profiledFunctionNameMap;
    //std::unordered_map<util::uuid, std::vector<DerivedTypeSymbol*>, boost::hash<util::uuid>> derivedTypeMap;
    std::map<util::uuid, std::vector<DerivedTypeSymbol*>> derivedTypeMap;
    std::vector<std::unique_ptr<DerivedTypeSymbol>> derivedTypes;
    //std::unordered_map<ClassTemplateSpecializationKey, ClassTemplateSpecializationSymbol*, ClassTemplateSpecializationKeyHash> classTemplateSpecializationMap;
    std::map<ClassTemplateSpecializationKey, ClassTemplateSpecializationSymbol*, ClassTemplateSpecializationKeyLess> classTemplateSpecializationMap;
    std::vector<std::unique_ptr<ClassTemplateSpecializationSymbol>> classTemplateSpecializations;
    std::unordered_map<ClassTemplateSpecializationSymbol*, ClassTemplateSpecializationSymbol*> specializationCopyMap;
    //std::unordered_map<ArrayKey, ArrayTypeSymbol*, ArrayKeyHash> arrayTypeMap;
    std::map<ArrayKey, ArrayTypeSymbol*, ArrayKeyLess> arrayTypeMap;
    std::vector<std::unique_ptr<ArrayTypeSymbol>> arrayTypes;
    std::unordered_map<cmajor::ast::IdentifierNode*, FunctionSymbol*> invokeMap;
    std::unordered_map<cmajor::ast::Node*, Symbol*> mappedNodeSymbolMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functionSymbols;
    ConversionTable conversionTable;
    std::unordered_set<ClassTypeSymbol*> polymorphicClasses;
    std::unordered_set<ClassTypeSymbol*> classesHavingStaticConstructor;
    std::unordered_set<std::u32string> jsonClasses;
    int numSpecializationsCopied;
    int numSpecializationsNew;
    int GetNextDeclarationBlockIndex() { return declarationBlockIndex++; }
    void ResetDeclarationBlockIndex() { declarationBlockIndex = 0; }
    void EmplaceTypeOrConceptRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId, int index);
    void WriteSymbolDefinitionMap(SymbolWriter& writer);
    void ReadSymbolDefinitionMap(SymbolReader& reader);
};

void InitCoreSymbolTable(SymbolTable& symbolTable);

void InitSymbolTable();
void DoneSymbolTable();

} // namespace cmajor::symbols

