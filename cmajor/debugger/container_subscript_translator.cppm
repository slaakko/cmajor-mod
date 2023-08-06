// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.container.subscript.translator;

import cmajor.debugger.container;
import cmajor.debug;
import std.core;

export namespace cmajor::debugger {

class Debugger;

class ContainerSubscriptTranslator : public cmajor::debug::BoundDebugNodeVisitor
{
public:
    ContainerSubscriptTranslator(Debugger* debugger_);
    void Visit(cmajor::debug::BoundDebugExpression& expr) override;
    void Visit(cmajor::debug::BoundVariableReferenceNode& node) override;
    void Visit(cmajor::debug::BoundIntegerLiteralNode& node) override;
    void Visit(cmajor::debug::BoundAddNode& node) override;
    void Visit(cmajor::debug::BoundSubNode& node) override;
    void Visit(cmajor::debug::BoundDerefNode& node) override;
    void Visit(cmajor::debug::BoundAddrOfNode& node) override;
    void Visit(cmajor::debug::BoundDotNode& node) override;
    void Visit(cmajor::debug::BoundSubscriptNode& node) override;
    void Visit(cmajor::debug::BoundRangeNode& node) override;
    void Visit(cmajor::debug::BoundParenExprNode& node) override;
    void Visit(cmajor::debug::BoundTypeNode& node) override;
    void Visit(cmajor::debug::BoundCastNode& node) override;
    cmajor::debug::ContainerClassTemplateKind GetContainerKind(cmajor::debug::DIType* type);
    const std::string& Expression() const { return expression; }
private:
    Debugger* debugger;
    std::string expression;
    bool evaluateIndex;
    int64_t index;
};

} // namespace cmajor::debug
