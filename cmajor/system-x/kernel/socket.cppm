// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.socket;

import std.core;

export namespace cmajor::systemx::kernel {

class Process;

int32_t Connect(Process* process, int64_t nodeAddr, int64_t serviceAddr);

} // namespace cmajor::systemx::kernel
