// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.debug.expr.evaluator;

import soul.xml.dom;
import cmajor.debug;
import std.core;

export namespace cmajor::debugger {

class Debugger;

class DebugExprEvaluator : public cmajor::debug::BoundDebugNodeVisitor
{
public:
    DebugExprEvaluator(Debugger* debugger_);
    void Visit(cmajor::debug::BoundDebugExpression& expr) override;
    void Visit(cmajor::debug::BoundIntegerLiteralNode& node) override;
    void Visit(cmajor::debug::BoundSubscriptNode& node) override;
    void Visit(cmajor::debug::BoundRangeNode& node) override;
    soul::xml::Element* ReleaseResult() { return result.release(); }
    void Evaluate(cmajor::debug::BoundDebugNode* node);
    void EvaluatePointerRange(cmajor::debug::BoundDebugNode* node, cmajor::debug::BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd);
    void EvaluateArrayRange(cmajor::debug::BoundDebugNode* node, cmajor::debug::BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd);
    void AddTypes(cmajor::debug::BoundDebugNode* node);
    cmajor::debug::ContainerClassTemplateKind GetContainerKind(cmajor::debug::DIType* type);
private:
    Debugger* debugger;
    std::unique_ptr<soul::xml::Element> result;
    int64_t integer;
    int64_t rangeStart;
    int64_t rangeEnd;
    cmajor::debug::InitializationStatus status;
};

} // namespace cmajor::debugger
