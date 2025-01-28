// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.error;

import soul.ast.source.pos;
import std.core;

export namespace cmajor::systemx::intermediate {

export namespace error { 
}

class Context;

void Error(const std::string& message, const soul::ast::SourcePos& sourcePos, Context* context);
void Error(const std::string& message, const soul::ast::SourcePos& sourcePos, Context* context, const soul::ast::SourcePos& refSourcePos);

} // cmajor::systemx::intermediate
