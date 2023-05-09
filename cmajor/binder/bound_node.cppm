// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.node;

import std.core;
import cmajor.ir;
import soul.ast.source.pos;
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
    BoundNode(const soul::ast::SourcePos& span_, const util::uuid& moduleId, BoundNodeType boundNodeType_);
    virtual void Accept(BoundNodeVisitor& visitor) = 0;
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    const util::uuid& ModuleId() const { return moduleId; }
    BoundNodeType GetBoundNodeType() const { return boundNodeType; }
private:
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    BoundNodeType boundNodeType;
};

} // namespace cmajor::binder
