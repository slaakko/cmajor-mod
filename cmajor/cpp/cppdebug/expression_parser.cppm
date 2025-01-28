// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.expression.parser;

import cmajor.debug.expr;
import cmajor.debug.di.type;
import cmajor.debug.debug.info;
import std.core;

export namespace cmajor::debug {

std::unique_ptr<DebugExprNode> Parse(const std::string& expression);

DIType* GetType(const std::string& expression, Instruction* stoppedInstruction, DebugInfo* debugInfo);

} // namespace cmajor::debug
