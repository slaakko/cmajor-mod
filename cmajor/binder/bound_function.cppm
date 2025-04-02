// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.function;

import std.core;
import cmajor.binder.bound.node;
import cmajor.binder.bound.statement;
import cmajor.ast;
import cmajor.symbols;
import cmajor.ir;

export namespace cmajor::binder {

class BoundCompileUnit;

class BoundFunction : public BoundNode
{
public:
    BoundFunction(BoundCompileUnit* boundCompileUnit_, cmajor::symbols::FunctionSymbol* functionSymbol_);
    BoundFunction(const BoundFunction&) = delete;
    BoundFunction& operator=(const BoundFunction&) = delete;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const { return functionSymbol; }
    cmajor::symbols::FunctionSymbol* GetFunctionSymbol() { return functionSymbol; }
    void SetBody(std::unique_ptr<BoundCompoundStatement>&& body_);
    BoundCompoundStatement* Body() const { return body.get(); }
    void SetHasGotos() { hasGotos = true; }
    bool HasGotos() const { return hasGotos; }
    void AddTemporaryDestructorCall(std::unique_ptr<BoundFunctionCall>&& destructorCall, BoundFunction* currentFunction, cmajor::symbols::ContainerScope* currentContainerScope,
        cmajor::ast::Node* node);
    void MoveTemporaryDestructorCallsTo(BoundExpression& expression);
    void AddLabeledStatement(BoundStatement* labeledStatement);
    const std::vector<BoundStatement*>& LabeledStatements() const { return labeledStatements; }
    void SetEnterCode(std::vector<std::unique_ptr<BoundStatement>>&& enterCode_);
    const std::vector<std::unique_ptr<BoundStatement>>& EnterCode() const { return enterCode; }
    void SetExitCode(std::vector<std::unique_ptr<BoundStatement>>&& exitCode_);
    const std::vector<std::unique_ptr<BoundStatement>>& ExitCode() const { return exitCode; }
    void SetCheckerCode(std::vector<std::unique_ptr<BoundStatement>>&& checkerCode_);
    const std::vector<std::unique_ptr<BoundStatement>>& CheckerCode() const { return checkerCode; }
    void SetLineCode(std::unique_ptr<BoundStatement>&& lineCode_);
    BoundStatement* GetLineCode() const { return lineCode.get(); }
    BoundCompileUnit* GetBoundCompileUnit() const { return boundCompileUnit; }
    void AddTemporary(cmajor::symbols::LocalVariableSymbol* temporary);
private:
    BoundCompileUnit* boundCompileUnit;
    cmajor::symbols::FunctionSymbol* functionSymbol;
    std::unique_ptr<BoundCompoundStatement> body;
    bool hasGotos;
    std::vector<std::unique_ptr<BoundFunctionCall>> temporaryDestructorCalls;
    std::vector<BoundStatement*> labeledStatements;
    std::vector<std::unique_ptr<BoundStatement>> enterCode;
    std::vector<std::unique_ptr<BoundStatement>> exitCode;
    std::vector<std::unique_ptr<BoundStatement>> checkerCode;
    std::unique_ptr<BoundStatement> lineCode;
    std::vector<std::unique_ptr<cmajor::symbols::LocalVariableSymbol>> temporaries;
};

} // namespace cmajor::binder
