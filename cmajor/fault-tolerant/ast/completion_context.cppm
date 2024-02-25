// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.completion.context;

import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class CompletionContext : int
{
    none = 0,
    global,
    nsName,
    dot,
    arrow,
    typeOrNamespaceName,
    statement,
    booleanExpr,
    container,
    expression
};

} // namespace cmajor::fault::tolerant::ast
