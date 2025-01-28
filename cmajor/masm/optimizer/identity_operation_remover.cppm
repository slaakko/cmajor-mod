// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.identity_operation_remover;

import std.core;
import cmajor.masm.intermediate;

export namespace cmajor::masm::optimizer {

void RemoveIdentityFunctionCalls(cmajor::masm::intermediate::Function* function);

} // cmajor::masm::optimizer
