// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.exception.handling.traps;

import std.core;

export namespace cmajor::systemx::kernel {

void InitExceptionHandlingTraps();
void DoneExceptionHandlingTraps();

} // namespace cmajor::systemx::kernel
