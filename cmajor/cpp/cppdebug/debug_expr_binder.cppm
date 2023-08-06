// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debug.expr.binder;

import cmajor.debug.debug.expr.visitor;
import cmajor.debug.bound.debug.expr;
import std.core;

export namespace cmajor::debug {

class Debugger;
class DebugInfo;
class Scope;
class BoundDebugNode;
class BoundDebugExpression;
class DebugExprNode;
class DIType;
class DIClassTemplateSpecializationType;
class Instruction;

class DebugExprBinder : public DebugExprVisitor
{
public:
    DebugExprBinder(Instruction* stoppedInstruction_, DebugInfo* debugInfo_, Scope* scope_, bool isBreakConditionExpr_);
    void Visit(IdentifierDebugExprNode& node) override;
    void Visit(IntegerDebugExprNode& node) override;
    void Visit(DisjunctionDebugExprNode& node) override;
    void Visit(ConjunctionDebugExprNode& node) override;
    void Visit(BitOrDebugExprNode& node) override;
    void Visit(BitXorDebugExprNode& node) override;
    void Visit(BitAndDebugExprNode& node) override;
    void Visit(EqualDebugExprNode& node) override;
    void Visit(NotEqualDebugExprNode& node) override;
    void Visit(LessEqualDebugExprNode& node) override;
    void Visit(GreaterEqualDebugExprNode& node) override;
    void Visit(LessDebugExprNode& node) override;
    void Visit(GreaterDebugExprNode& node) override;
    void Visit(ShiftLeftDebugExprNode& node) override;
    void Visit(ShiftRightDebugExprNode& node) override;
    void Visit(AddDebugExprNode& node) override;
    void Visit(SubDebugExprNode& node) override;
    void Visit(MulDebugExprNode& node) override;
    void Visit(DivDebugExprNode& node) override;
    void Visit(ModDebugExprNode& node) override;
    void Visit(ParenthesizedDebugExprNode& node) override;
    void Visit(DerefDebugExprNode& node) override;
    void Visit(AddrOfDebugExprNode& node) override;
    void Visit(DotDebugExprNode& node) override;
    void Visit(SubscriptDebugExprNode& node) override;
    void Visit(RangeDebugExprNode& node) override;
    void Visit(TypeIdDebugExprNode& node) override;
    void Visit(CastDebugExprNode& node) override;
    BoundDebugExpression* BoundExpression(DebugExprNode* sourceNode);
    DIType* GetValueType(DIClassTemplateSpecializationType* specializationType) const;
private:
    Instruction* stoppedInstruction;
    DebugInfo* debugInfo;
    Scope* scope;
    std::unique_ptr<BoundDebugNode> currentNode;
    std::unique_ptr<BoundDebugExpression> expression;
    bool hasContainerSubscript;
    InitializationStatus status;
    bool isBreakConditionExpr;
};

} // namespace cmajor::debug
