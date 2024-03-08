// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.error;

import soul.ast.span;
import std.core;

export namespace cmajor::masm::intermediate {

namespace error {}

class Context;

void Error(const std::string& message, const soul::ast::Span& span, Context* context);
void Error(const std::string& message, const soul::ast::Span& span, Context* context, const soul::ast::Span& refSpan);

} // cmajor::masm::intermediate
