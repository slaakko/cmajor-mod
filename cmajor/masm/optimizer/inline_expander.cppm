// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.inline_expander;

import std.core;
import cmajor.masm.intermediate;

export namespace cmajor::masm::optimizer {

void InlineExpand(cmajor::masm::intermediate::Function* function);

} // cmajor::masm::optimizer
