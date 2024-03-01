// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.node;

import std.core;
import cmajor.ir;
import soul.ast.span;
import util.uuid;

export namespace cmajor::binder {

enum class BoundNodeType : uint8_t
{
    boundCompileUnit, boundClass, boundFunction,
    boundSequenceStatement, boundCompoundStatement, boundReturnStatement, boundIfStatement, boundWhileStatement, boundDoStatement, boundForStatement,
    boundSwitchStatement, boundCaseStatement, boundDefaultStatement, boundGotoCaseStatement, boundGotoDefaultStatement, boundBreakStatement, boundContinueStatement,
    boundGotoStatement, boundConstructionStatement, boundAssignmentStatement, boundExpressionStatement, boundInitializationStatement, boundEmptyStatement,
    boundSetVmtPtrStatement, boundThrowStatement, boundRethrowStatement, boundTryStatement, boundCatchStatement,
    boundParameter, boundLocalVariable, boundMemberVariable, boundConstant, boundEnumConstant, boundLiteral, boundTemporary, boundBitCast, boundFunctionPtr, boundConstructAndReturnTemporary,
    boundClassOrClassDelegateConversionResult,
    boundFunctionCall, boundDelegateCall, boundClassDelegateCall, boundConversion, boundSizeOfExpression, boundAddressOfExpression, boundDereferenceExpression, boundReferenceToPointerExpression,
    boundConstructExpression, boundIsExpression, boundAsExpression, boundTypeNameExpression, boundTypeIdExpression, boundDisjunction, boundConjunction,
    boundAtomicConstraint, boundDisjunctiveConstraint, boundConjunctiveConstraint, boundConcept,
    boundTypeExpression, boundNamespaceExpression, boundFunctionGroupExpression, boundMemberExpression,
    boundNamespace, boundEnumTypeDefinition, boundGlobalVariable
};

class BoundNodeVisitor;

class BoundNode : public cmajor::ir::GenObject
{
public:
    BoundNode(const soul::ast::Span& span_, BoundNodeType boundNodeType_);
    virtual void Accept(BoundNodeVisitor& visitor) = 0;
    const soul::ast::Span& GetSpan() const { return span; }
    void SetSpan(const soul::ast::Span& span_);
    BoundNodeType GetBoundNodeType() const { return boundNodeType; }
    soul::ast::FullSpan GetFullSpan() const;
    virtual const util::uuid& ModuleId() const;
    virtual int32_t FileIndex() const;
    BoundNode* Parent() const { return parent; }
    void SetParent(BoundNode* parent_) { parent = parent_; }
    virtual bool IsBoundStatement() const { return false; }
private:
    soul::ast::Span span;
    BoundNodeType boundNodeType;
    BoundNode* parent;
};

} // namespace cmajor::binder
