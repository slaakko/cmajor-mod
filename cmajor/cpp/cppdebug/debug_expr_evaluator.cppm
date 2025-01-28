// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debug.expression.evaluator;

import cmajor.debug.bound.debug.node.visitor;
import cmajor.debug.bound.debug.expr;
import cmajor.debug.container;
import util;
import std.core;

export namespace cmajor::debug {

class Debugger;
class DebugInfo;
class DIType;

class DebugExpressionEvaluator : public BoundDebugNodeVisitor
{
public:
    DebugExpressionEvaluator(Debugger& debugger_, DebugInfo* debugInfo_);
    void Visit(BoundDebugExpression& expr) override;
    void Visit(BoundIntegerLiteralNode& node) override;
    void Visit(BoundSubscriptNode& node) override;
    void Visit(BoundRangeNode& node) override;
    util::JsonValue* ReleaseResult() { return result.release(); }
    ContainerClassTemplateKind GetContainerKind(DIType* type);
    void Evaluate(BoundDebugNode* node);
    void EvaluatePointerRange(BoundDebugNode* node, BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd);
    void EvaluateArrayRange(BoundDebugNode* node, BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd);
    void AddTypes(BoundDebugNode* node);
private:
    Debugger& debugger;
    DebugInfo* debugInfo;
    std::unique_ptr<util::JsonValue> result;
    int64_t integer;
    int64_t rangeStart;
    int64_t rangeEnd;
    InitializationStatus status;
};

} // namespace cmajor::debug
