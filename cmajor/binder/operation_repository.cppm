// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.operation.repository;

import std.core;
import cmajor.symbols;
import cmajor.ast;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

const int maxGeneratedInlineFunctionMembers = 4;

class BoundCompileUnit;
class BoundClass;
class BoundExpression;
class BoundFunction;
class BoundCompoundStatement;
class StatementBinder;

enum class CollectFlags : int
{
    none = 0, noRvalueRef = 1 << 0, dontInstantiate = 1 << 1
};

inline CollectFlags operator|(CollectFlags left, CollectFlags right)
{
    return static_cast<CollectFlags>(static_cast<int>(left) | static_cast<int>(right));
}

inline CollectFlags operator&(CollectFlags left, CollectFlags right)
{
    return static_cast<CollectFlags>(static_cast<int>(left) & static_cast<int>(right));
}

class Operation
{
public:
    Operation(const std::u32string& groupName_, int arity_, BoundCompileUnit& boundCompileUnit_);
    virtual ~Operation();
    virtual void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope_, 
        const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, 
        cmajor::ast::Node* node, CollectFlags flags) = 0;
    const std::u32string& GroupName() const { return groupName; }
    int Arity() const { return arity; }
    cmajor::symbols::SymbolTable* GetSymbolTable();
    BoundCompileUnit& GetBoundCompileUnit();
private:
    std::u32string groupName;
    int arity;
    BoundCompileUnit& boundCompileUnit;
};

class ArityOperation
{
public:
    void Add(Operation* operation);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags);
private:
    std::vector<Operation*> operations;
};

class OperationGroup
{
public:
    void Add(Operation* operation);
    void CollectViableFunctions(cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundFunction* currentFunction,
        cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, cmajor::ast::Node* node, CollectFlags flags);
private:
    std::vector<std::unique_ptr<ArityOperation>> arityOperations;
};

class OperationRepository
{
public:
    OperationRepository(BoundCompileUnit& boundCompileUnit_);
    void Add(Operation* operation);
    void CollectViableFunctions(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, const std::vector<std::unique_ptr<BoundExpression>>& arguments,
        BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception,
        cmajor::ast::Node* node, CollectFlags flags);
    void GenerateCopyConstructorFor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        cmajor::ast::Node* node);
    void GenerateCopyConstructorFor(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::symbols::ContainerScope* containerScope, cmajor::ast::Node* node);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_map<std::u32string, OperationGroup*> operationGroupMap;
    std::vector<std::unique_ptr<OperationGroup>> operationGroups;
    std::vector<std::unique_ptr<Operation>> operations;
    Operation* copyConstructorOperation;
};

void GenerateDestructorImplementation(BoundClass* boundClass, cmajor::symbols::DestructorSymbol* destructorSymbol, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, cmajor::ast::Node* node);

void GenerateStaticClassInitialization(cmajor::symbols::StaticConstructorSymbol* staticConstructorSymbol, cmajor::ast::StaticConstructorNode* staticConstructorNode, 
    BoundCompileUnit& boundCompileUnit, BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction, cmajor::symbols::ContainerScope* containerScope, 
    StatementBinder* statementBinder, cmajor::ast::Node* node);

void GenerateClassInitialization(cmajor::symbols::ConstructorSymbol* constructorSymbol, cmajor::ast::ConstructorNode* constructorNode, 
    BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction,
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, 
    bool generateDefault, cmajor::ast::Node* node);

void GenerateClassAssignment(cmajor::symbols::MemberFunctionSymbol* assignmentFunctionSymbol, cmajor::ast::MemberFunctionNode* assignmentNode, 
    BoundCompoundStatement* boundCompoundStatement, BoundFunction* boundFunction,
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool generateDefault, 
    cmajor::ast::Node* node);

void GenerateClassTermination(cmajor::symbols::DestructorSymbol* destructorSymbol, cmajor::ast::DestructorNode* destructorNode, BoundCompoundStatement* boundCompoundStatement, 
    BoundFunction* boundFunction, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, 
    cmajor::ast::Node* node);

} // namespace cmajor::binder
