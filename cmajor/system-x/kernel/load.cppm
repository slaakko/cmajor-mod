// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.load;

import cmajor.systemx.machine;
import cmajor.systemx.object;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

class TextSegmentWriteProtectionGuard
{
public:
    TextSegmentWriteProtectionGuard(uint64_t rv_, cmajor::systemx::machine::Memory& mem_);
    ~TextSegmentWriteProtectionGuard();
private:
    uint64_t rv;
    cmajor::systemx::machine::Memory& mem;
};


void Load(Process* process, const std::vector<std::string>& args, const std::vector<std::string>& env, cmajor::systemx::machine::Machine& machine);
void Load(Process* process, cmajor::systemx::object::BinaryFile* binaryFile,
    const std::vector<std::string>& args, const std::vector<std::string>& env, cmajor::systemx::machine::Machine& machine, uint64_t rv, bool addRunnable);

} // namespace cmajor::systemx::kernel
