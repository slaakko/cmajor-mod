// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.prog;

import std.core;

export namespace cmajor::systemx::kernel {

class Process;

int32_t Start(Process* process, int64_t progAddr, int port);
void Stop(int32_t prog);
int32_t GetPort(int32_t prog);
void InitProg();
void DoneProg();

} // namespace cmajor::systemx::kernel
