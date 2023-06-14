// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.exception.handling;

import std.core;

export namespace cmajor::systemx::kernel {

class Process;

std::string GetStackTrace(Process* process);
void DispatchException(uint64_t exceptionAddress, uint64_t exceptionClassId, Process* process, uint64_t& setFP, uint64_t& setSP, uint64_t& setPC);
void ContinueExceptionDispatch(Process* process, uint64_t& setFP, uint64_t& setSP, uint64_t& setPC);

} // namespace cmajor::systemx::kernel
