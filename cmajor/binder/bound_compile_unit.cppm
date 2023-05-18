// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.binder.bound.compile.unit;

import std.core;
import cmajor.binder.bound.node;
import cmajor.binder.class_template_repository;
import cmajor.binder.function_template_repository;
import cmajor.binder.operation.repository;
import cmajor.binder.inline_function_repository;
import cmajor.binder.const_expr_function_repository;
import cmajor.binder.string.repository;
import cmajor.binder.constant.array.repository;
import cmajor.binder.constant.structure.repository;
import cmajor.binder.uuid.repository;
import cmajor.binder.concept_repository;
import cmajor.symbols;
import cmajor.ast;

export namespace cmajor::binder {

class BoundExpression;
struct ArgumentMatch;
class AttributeBinder;
class BoundNamespace;

class BoundCompileUnit : public BoundNode
{
public:
    BoundCompileUnit(cmajor::symbols::Module& module_, cmajor::ast::CompileUnitNode* compileUnitNode_, AttributeBinder* attributeBinder_);
    BoundCompileUnit(const BoundCompileUnit&) = delete;
    BoundCompileUnit& operator=(const BoundCompileUnit&) = delete;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    cmajor::symbols::Module& GetModule() { return module; }
    cmajor::symbols::SymbolTable& GetSymbolTable() { return symbolTable; }
    cmajor::ast::CompileUnitNode* GetCompileUnitNode() const { return compileUnitNode; }
    void AddFileScope(cmajor::symbols::FileScope* fileScope);
    void RemoveLastFileScope();
    cmajor::symbols::FileScope* ReleaseLastFileScope();
    cmajor::symbols::FileScope* FirstFileScope() const 
    { 
        Assert(!fileScopes.empty(), "file scopes empty");  
        return fileScopes.front().get(); 
    }
    const std::vector<std::unique_ptr<cmajor::symbols::FileScope>>& FileScopes() const { return fileScopes; }
    void AddBoundNode(std::unique_ptr<BoundNode>&& boundNode);
    const std::vector<std::unique_ptr<BoundNode>>& BoundNodes() const { return boundNodes; }
    cmajor::symbols::FunctionSymbol* GetConversion(cmajor::symbols::TypeSymbol* sourceType, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, ArgumentMatch& argumentMatch);
    void CollectViableFunctions(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, std::vector<std::unique_ptr<BoundExpression>>& arguments, 
        BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, CollectFlags flags);
    cmajor::symbols::FunctionSymbol* InstantiateFunctionTemplate(cmajor::symbols::FunctionSymbol* functionTemplate, 
        const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMapping, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    bool InstantiateClassTemplateMemberFunction(cmajor::symbols::FunctionSymbol* memberFunction, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::symbols::FunctionSymbol* InstantiateInlineFunction(cmajor::symbols::FunctionSymbol* inlineFunction, cmajor::symbols::ContainerScope* containerScope, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    cmajor::ast::FunctionNode* GetFunctionNodeFor(cmajor::symbols::FunctionSymbol* constExprFunctionSymbol);
    void GenerateCopyConstructorFor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    void GenerateCopyConstructorFor(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    int Install(const std::string& str);
    int Install(const std::u16string& str);
    int Install(const std::u32string& str);
    int Install(const util::uuid& uuid);
    const std::string& GetUtf8String(int stringId) const;
    const std::u16string& GetUtf16String(int stringId) const;
    const std::u32string& GetUtf32String(int stringId) const;
    const unsigned char* GetUtf8CharPtr(int stringId) const;
    const char16_t* GetUtf16CharPtr(int stringId) const;
    const char32_t* GetUtf32CharPtr(int stringId) const;
    const util::uuid& GetUuid(int uuidId) const;
    void AddConstantArray(cmajor::symbols::ConstantSymbol* constantArraySymbol);
    ConstantArrayRepository& GetConstantArrayRepository() { return constantArrayRepository; }
    void AddConstantStructure(cmajor::symbols::ConstantSymbol* constantStructureSymbol);
    ConstantStructureRepository& GetConstantStructureRepository() { return constantStructureRepository; }
    const std::string& SourceFilePath() const { return compileUnitNode->FilePath(); }
    const std::string& LLFilePath() const { return llFilePath; }
    const std::string& CppFilePath() const { return cppFilePath; }
    const std::string& BCFilePath() const { return bcFilePath; }
    const std::string& OptLLFilePath() const { return optLLFilePath; }
    const std::string& OptBCFilePath() const { return optBCFilePath; }
    const std::string& ObjectFilePath() const { return objectFilePath; }
    const std::string& Id() const { return compileUnitNode->Id(); }
    void SetHasGotos() { hasGotos = true; }
    bool HasGotos() const { return hasGotos; }
    ClassTemplateRepository& GetClassTemplateRepository() { return classTemplateRepository; }
    ConceptRepository& GetConceptRepository() { return conceptRepository; }
    void PushBindingTypes();
    void PopBindingTypes();
    bool BindingTypes() const { return bindingTypes; }
    void FinalizeBinding(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization);
    AttributeBinder* GetAttributeBinder() const { return attributeBinder; }
    void PushNamespace(BoundNamespace* ns);
    void PopNamespace();
    bool HasCopyConstructorFor(const util::uuid& typeId) const;
    cmajor::symbols::FunctionSymbol* GetCopyConstructorFor(const util::uuid& typeId) const;
    void AddCopyConstructorFor(const util::uuid& typeId, std::unique_ptr<cmajor::symbols::FunctionSymbol>&& copyConstructor);
    void AddCopyConstructorToMap(const util::uuid& typeId, cmajor::symbols::FunctionSymbol* copyConstructor);
    bool Immutable() const { return immutable; }
    void SetImmutable() { immutable = true; }
    void AddGlobalNs(std::unique_ptr<cmajor::ast::NamespaceNode>&& globalNs);
    int GetNextExitEntryIndex() { return nextExitEntryIndex++; }
    bool IsGeneratedDestructorInstantiated(cmajor::symbols::DestructorSymbol* generatedDestructorSymbol) const;
    void SetGeneratedDestructorInstantiated(cmajor::symbols::DestructorSymbol* generatedDestructorSymbol);
    void SetSystemRuntimeUnwindInfoSymbol(cmajor::symbols::TypeSymbol* systemRuntimeUnwindInfoSymbol_);
    cmajor::symbols::TypeSymbol* GetSystemRuntimeUnwindInfoSymbol() const { return systemRuntimeUnwindInfoSymbol; }
    void GenerateInitUnwindInfoFunctionSymbol();
    cmajor::symbols::FunctionSymbol* GetInitUnwindInfoFunctionSymbol() const { return initUnwindInfoFunctionSymbol.get(); }
    void GenerateCompileUnitInitialization();
    cmajor::symbols::FunctionSymbol* GetInitCompileUnitFunctionSymbol() const { return initCompileUnitFunctionSymbol.get(); }
    cmajor::symbols::FunctionSymbol* GetPushCompileUnitUnwindInfoInitFunctionSymbol() const { return pushCompileUnitUnwindInfoInitFunctionSymbol; }
    cmajor::symbols::GlobalVariableSymbol* GetCompileUnitUnwindInfoVarSymbol() const { return compileUnitUnwindInfoVarSymbol.get(); }
    cmajor::symbols::TypeSymbol* GetInitUnwindInfoDelegateType() const { return initUnwindInfoDelegateType; }
    cmajor::symbols::FunctionSymbol* GetSystemRuntimeAddCompileUnitFunctionSymbol() const { return systemRuntimeAddCompileUnitFunctionSymbol; }
    void SetSystemRuntimeAddCompileUnitFunctionSymbol(cmajor::symbols::FunctionSymbol* systemRuntimeAddCompileUnitFunctionSymbol_) 
    { 
        systemRuntimeAddCompileUnitFunctionSymbol = systemRuntimeAddCompileUnitFunctionSymbol_; 
    }
    void GenerateGlobalInitializationFunction();
    cmajor::symbols::FunctionSymbol* GetGlobalInitializationFunctionSymbol() const { return globalInitFunctionSymbol; }
    const std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>>& AllCompileUnitInitFunctionSymbols() const { return allCompileUnitInitFunctionSymbols; }
    bool CodeGenerated(cmajor::symbols::FunctionSymbol* functionSymbol) const;
    void SetCodeGenerated(cmajor::symbols::FunctionSymbol* functionSymbol);
    bool CanReuse(cmajor::symbols::FunctionSymbol* functionSymbol) const;
    void SetCanReuse(cmajor::symbols::FunctionSymbol* functionSymbol);
    void SetLatestIdentifier(cmajor::ast::IdentifierNode* latestIdentifierNode_) { latestIdentifierNode = latestIdentifierNode_; }
    cmajor::ast::IdentifierNode* GetLatestIdentifier() { return latestIdentifierNode; }
private:
    cmajor::symbols::Module& module;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::ast::IdentifierNode* latestIdentifierNode;
    cmajor::ast::CompileUnitNode* compileUnitNode;
    AttributeBinder* attributeBinder;
    std::stack<BoundNamespace*> namespaceStack;
    BoundNamespace* currentNamespace;
    std::string llFilePath;
    std::string bcFilePath;
    std::string optBCFilePath;
    std::string optLLFilePath;
    std::string objectFilePath;
    std::string cppFilePath;
    std::vector<std::unique_ptr<cmajor::symbols::FileScope>> fileScopes;
    std::vector<std::unique_ptr<BoundNode>> boundNodes;
    std::vector<std::unique_ptr<cmajor::ast::NamespaceNode>> globalNamespaceNodes;
    bool hasGotos;
    OperationRepository operationRepository;
    FunctionTemplateRepository functionTemplateRepository;
    ClassTemplateRepository classTemplateRepository;
    InlineFunctionRepository inlineFunctionRepository;
    ConstExprFunctionRepository constExprFunctionRepository;
    StringRepository<std::string, const unsigned char*> utf8StringRepository;
    StringRepository<std::u16string, const char16_t*> utf16StringRepository;
    StringRepository<std::u32string, const char32_t*> utf32StringRepository;
    UuidRepository uuidRepository;
    ConstantArrayRepository constantArrayRepository;
    ConstantStructureRepository constantStructureRepository;
    ConceptRepository conceptRepository;
    cmajor::symbols::ConversionTable conversionTable;
    bool bindingTypes;
    std::stack<bool> bindingTypesStack;
    int32_t compileUnitIndex;
    bool immutable;
    std::unordered_map<util::uuid, cmajor::symbols::FunctionSymbol*, util::UuidHash> copyConstructorMap;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> copyConstructors;
    int nextExitEntryIndex;
    std::set<cmajor::symbols::DestructorSymbol*> instantiatedGeneratedDestructors;
    std::set<cmajor::symbols::FunctionSymbol*> codeGenerated;
    std::set<cmajor::symbols::FunctionSymbol*> canReuse;
    cmajor::symbols::TypeSymbol* systemRuntimeUnwindInfoSymbol;
    std::unique_ptr<cmajor::symbols::FunctionSymbol> initUnwindInfoFunctionSymbol;
    std::unique_ptr<cmajor::symbols::FunctionSymbol> initCompileUnitFunctionSymbol;
    cmajor::symbols::FunctionSymbol* systemRuntimeAddCompileUnitFunctionSymbol;
    cmajor::symbols::FunctionSymbol* pushCompileUnitUnwindInfoInitFunctionSymbol;
    cmajor::symbols::TypeSymbol* initUnwindInfoDelegateType;
    std::unique_ptr<cmajor::symbols::GlobalVariableSymbol> compileUnitUnwindInfoVarSymbol;
    cmajor::symbols::FunctionSymbol* globalInitFunctionSymbol;
    std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>> allCompileUnitInitFunctionSymbols;
};

} // namespace cmajor::binder
