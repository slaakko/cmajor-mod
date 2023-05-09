// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.statement.binder;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundGotoCaseStatement;
class BoundGotoDefaultStatement;
class BoundClass;
class BoundFunction;
class BoundCompileUnit;
class BoundStatement;

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::FunctionNode& functionNode, cmajor::symbols::ContainerScope* containerScope, 
    BoundCompileUnit& boundCompileUnit);

class StatementBinder : public cmajor::ast::Visitor
{
public:
    StatementBinder(BoundCompileUnit& boundCompileUnit_);
    void Visit(cmajor::ast::CompileUnitNode& compileUnitNode) override;
    void Visit(cmajor::ast::NamespaceNode& namespaceNode) override;
    void Visit(cmajor::ast::EnumTypeNode& enumTypeNode) override;
    void Visit(cmajor::ast::ClassNode& classNode) override;
    void Visit(cmajor::ast::MemberVariableNode& memberVariableNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::FullInstantiationRequestNode& fullInstantiationRequestNode) override;
    void Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode) override;
    void Visit(cmajor::ast::ConstructorNode& constructorNode) override;
    void Visit(cmajor::ast::DestructorNode& destructorNode) override;
    void Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode) override;
    void Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode) override;
    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::ReturnStatementNode& returnStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::BreakStatementNode& breakStatementNode) override;
    void Visit(cmajor::ast::ContinueStatementNode& continueStatementNode) override;
    void Visit(cmajor::ast::GotoStatementNode& gotoStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode) override;
    void Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode) override;
    void Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode) override;
    void Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode) override;
    void Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode) override;
    void Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode) override;
    void Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode) override;
    void Visit(cmajor::ast::ThrowStatementNode& throwStatementNode) override;
    void Visit(cmajor::ast::TryStatementNode& tryStatementNode) override;
    void Visit(cmajor::ast::CatchNode& catchNode) override;
    void Visit(cmajor::ast::AssertStatementNode& assertStatementNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode) override;
    void Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode) override;
    void Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode) override;
    void CompileStatement(cmajor::ast::Node* statementNode, bool setPostfix);
    BoundCompileUnit& GetBoundCompileUnit() { return boundCompileUnit; }
    void SetCurrentClass(BoundClass* currentClass_) { currentClass = currentClass_; }
    BoundClass* CurrentClass() const { return currentClass; }
    cmajor::symbols::ContainerScope* GetContainerScope() { return containerScope; }
    void SetContainerScope(cmajor::symbols::ContainerScope* containerScope_) { containerScope = containerScope_; }
    BoundFunction* CurrentFunction() { return currentFunction; }
    void SetCurrentFunction(BoundFunction* currentFunction_) { currentFunction = currentFunction_; }
    void SetCurrentConstructor(cmajor::symbols::ConstructorSymbol* currentConstructorSymbol_, cmajor::ast::ConstructorNode* currentConstructorNode_);
    cmajor::symbols::ConstructorSymbol* CurrentConstructorSymbol() { return currentConstructorSymbol; }
    cmajor::ast::ConstructorNode* CurrentConstructorNode() { return currentConstructorNode; }
    cmajor::symbols::MemberFunctionSymbol* CurrentMemberFunctionSymbol() const { return currentMemberFunctionSymbol; }
    cmajor::ast::MemberFunctionNode* CurrentMemberFunctionNode() const { return currentMemberFunctionNode; }
    void SetCurrentDestructor(cmajor::symbols::DestructorSymbol* currentDestructorSymbol_, cmajor::ast::DestructorNode* currentDestructorNode_);
    void SetCurrentMemberFunction(cmajor::symbols::MemberFunctionSymbol* currentMemberFunctionSymbol_, cmajor::ast::MemberFunctionNode* currentMemberFunctionNode_);
    BoundStatement* ReleaseStatement() { return statement.release(); }
    bool CompilingThrow() const { return compilingThrow; }
    bool InsideCatch() const { return insideCatch; }
    void GenerateEnterAndExitFunctionCode(BoundFunction* boundFunction);
private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::Module* module;
    cmajor::symbols::ContainerScope* containerScope;
    std::unique_ptr<BoundStatement> statement;
    int compoundLevel;
    bool insideCatch;
    BoundClass* currentClass;
    BoundFunction* currentFunction;
    cmajor::symbols::StaticConstructorSymbol* currentStaticConstructorSymbol;
    cmajor::ast::StaticConstructorNode* currentStaticConstructorNode;
    cmajor::symbols::ConstructorSymbol* currentConstructorSymbol;
    cmajor::ast::ConstructorNode* currentConstructorNode;
    cmajor::symbols::DestructorSymbol* currentDestructorSymbol;
    cmajor::ast::DestructorNode* currentDestructorNode;
    cmajor::symbols::MemberFunctionSymbol* currentMemberFunctionSymbol;
    cmajor::ast::MemberFunctionNode* currentMemberFunctionNode;
    cmajor::symbols::TypeSymbol* switchConditionType;
    std::unordered_map<cmajor::symbols::IntegralValue, cmajor::ast::CaseStatementNode*, cmajor::symbols::IntegralValueHash>* currentCaseValueMap;
    std::vector<std::pair<BoundGotoCaseStatement*, cmajor::symbols::IntegralValue>>* currentGotoCaseStatements;
    std::vector<BoundGotoDefaultStatement*>* currentGotoDefaultStatements;
    bool postfix;
    bool compilingThrow;
    bool compilingReleaseExceptionStatement;
    bool dontCheckDuplicateFunctionSymbols;
    std::stack<bool> conditionalCompilationStack;
    void AddStatement(BoundStatement* boundStatement);
    void AddReleaseExceptionStatement(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
};

} // namespace cmajor::binder
