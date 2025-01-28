// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.arithmetics;

import std.core;
import cmajor.masm.intermediate;

export namespace cmajor::masm::optimizer {

void OptimizeArithmetics(cmajor::masm::intermediate::Function* fn, cmajor::masm::intermediate::Context* context);

} // cmajor::masm::optimizer
