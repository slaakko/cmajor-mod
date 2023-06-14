// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.prog;

import std.core;

export namespace cmajor::systemx::kernel {

class Process;

const int sxbsPortNumber = 55001;

int32_t Start(Process* process, int64_t progAddr);
void Stop(int32_t prog);
void InitProg();
void DoneProg();

} // namespace cmajor::systemx::kernel
