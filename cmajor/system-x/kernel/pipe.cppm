// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.pipe;

import std.core;

export namespace cmajor::systemx::kernel {

class Process;

void MakePipe(cmajor::systemx::kernel::Process* process, int64_t readerFdAddr, int64_t writerFdAddr);
void InitPipe();
void DonePipe();

} // namespace cmajor::systemx::kernel
