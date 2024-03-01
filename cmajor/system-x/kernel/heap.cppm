// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.heap;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

void DumpHeap(cmajor::systemx::machine::Processor& processor, uint64_t freeAddr, int32_t tag, uint64_t ptr, uint64_t size);
void MCpy(cmajor::systemx::kernel::Process* process, uint64_t sourceBufferAddr, uint64_t targetBufferAddr, uint64_t count);

} // namespace cmajor::systemx::kernel
