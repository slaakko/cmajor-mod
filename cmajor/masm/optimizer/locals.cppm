// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.locals;

import std.core;
import cmajor.masm.intermediate;

export namespace cmajor::masm::optimizer {

void MoveLocalsToEntryBlock(cmajor::masm::intermediate::Function* fn);

} // cmajor::masm::optimizer
