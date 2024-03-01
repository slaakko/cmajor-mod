// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.heap;

import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

std::string LogDir()
{
    std::string root = util::CmajorRoot();
    return util::GetFullPath(util::Path::Combine(root, "log"));
}

std::string HeapLogFilePath(uint64_t index)
{
    return util::Path::Combine(LogDir(), "heap" + std::to_string(index) + ".log");
}

void Print(uint64_t a, uint64_t x, const std::string& name, util::CodeFormatter& formatter)
{
    formatter.WriteLine("address: #" + util::ToHexString(a) + ": " + name + ": value: #" + util::ToHexString(x));
}

void DumpHeap(cmajor::systemx::machine::Processor& processor, uint64_t freeAddr, int32_t tag, uint64_t ptr, uint64_t size)
{
    static uint64_t index = 0;
    std::ofstream file(HeapLogFilePath(index++));
    util::CodeFormatter formatter(file);
    if (tag == 0)
    {
        formatter.WriteLine("before alloc size: #" + util::ToHexString(size));
    }
    else if (tag == 1)
    {
        formatter.WriteLine("after alloc ptr: #" + util::ToHexString(ptr));
    }
    else if (tag == 2)
    {
        formatter.WriteLine("before free ptr: #" + util::ToHexString(ptr));
    }
    else if (tag == 3)
    {
        formatter.WriteLine("after free:");
    }
    else if (tag == 4)
    {
        formatter.WriteLine("before resize");
    }
    else if (tag == 5)
    {
        formatter.WriteLine("after resize");
    }
    cmajor::systemx::machine::UserProcess* currentProcess = processor.CurrentProcess();
    if (currentProcess)
    {
        int64_t heapStart = currentProcess->HeapStartAddress();
        formatter.WriteLine("heap start:  #" + util::ToHexString(static_cast<uint64_t>(heapStart)));
        int64_t heapLength = currentProcess->HeapLength();
        formatter.WriteLine("heap length: #" + util::ToHexString(static_cast<uint64_t>(heapLength)));
    }
    uint64_t rv = processor.Regs().GetSpecial(cmajor::systemx::machine::rV);
    if (freeAddr != 0u)
    {
        uint64_t next = processor.GetMachine()->Mem().ReadOcta(rv, freeAddr, cmajor::systemx::machine::Protection::read);
        Print(freeAddr, next, "next", formatter);
        uint64_t blockSize = processor.GetMachine()->Mem().ReadOcta(rv, freeAddr + 8, cmajor::systemx::machine::Protection::read);
        Print(freeAddr + 8, 16 * blockSize, "size", formatter);
        while (next != freeAddr)
        {
            uint64_t n = processor.GetMachine()->Mem().ReadOcta(rv, next, cmajor::systemx::machine::Protection::read);
            uint64_t s = processor.GetMachine()->Mem().ReadOcta(rv, next + 8, cmajor::systemx::machine::Protection::read);
            Print(next, n, "next", formatter);
            Print(next + 8, 16 * s, "size", formatter);
            next = n;
        }
    }
}

void MCpy(cmajor::systemx::kernel::Process* process, uint64_t sourceBufferAddr, uint64_t targetBufferAddr, uint64_t count)
{
    process->GetProcessor()->GetMachine()->Mem().Copy(process->RV(), sourceBufferAddr, targetBufferAddr, count);
}

} // namespace cmajor::systemx::kernel
