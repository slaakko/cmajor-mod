// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.expression.binder;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundCompileUnit;
class BoundFunction;
class BoundExpression;
class StatementBinder;

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder);
std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue);
std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression);
std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression, bool acceptIncomplete);
std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression, bool acceptIncomplete, 
    bool moveTemporaryDestructorCalls);

} // namespace cmajor::binder
