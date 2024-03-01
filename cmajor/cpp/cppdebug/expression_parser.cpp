// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.expression.parser;

import cmajor.debug.expr.lexer;
import cmajor.debug.expr.parser;
import cmajor.debug.debug.expr.binder;
import util;

namespace cmajor::debug {

std::unique_ptr<DebugExprNode> Parse(const std::string& expression)
{
    std::u32string expr = util::ToUtf32(expression);
    auto lexer = cmajor::debug::expr::lexer::MakeLexer(expr.c_str(), expr.c_str() + expr.length(), "");
    using LexerType = decltype(lexer);
    std::unique_ptr<DebugExprNode> node = cmajor::debug::expr::parser::DebugExprParser<LexerType>::Parse(lexer);
    return node;
}

DIType* GetType(const std::string& expression, Instruction* stoppedInstruction, DebugInfo* debugInfo)
{
    std::unique_ptr<DebugExprNode> node = Parse(expression);
    DebugExprBinder binder(stoppedInstruction, debugInfo, stoppedInstruction->GetScope(), false);
    node->Accept(binder);
    BoundDebugExpression* boundExpression = binder.BoundExpression(node.get());
    return boundExpression->Type();
}

} // namespace cmajor::debug
