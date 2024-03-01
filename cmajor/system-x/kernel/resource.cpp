// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.resource;

import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.io;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

Resource::Resource(int32_t fd_, int64_t start_, int64_t length_) : File("RESOURCE"), fd(fd_), start(start_), length(length_)
{
}

void Resource::Close(cmajor::systemx::kernel::Process* process)
{
    cmajor::systemx::kernel::Close(process, fd);
}

int32_t OpenResource(Process* process, uint64_t resourceNameAddr)
{
    if (resourceNameAddr == 0)
    {
        throw SystemError(EPARAM, "resource name is null", __FUNCTION__);
    }
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    std::string resourceName = ReadString(process, resourceNameAddr, mem);
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    cmajor::systemx::object::Symbol* symbol = symbolTable->GetSymbol("@resource." + resourceName);
    if (symbol)
    {
        int64_t start = symbol->Start();
        int64_t length = symbol->Length();
        std::unique_ptr<Resource> resource(new Resource(cmajor::systemx::kernel::Open(process, process->FilePath(), static_cast<int32_t>(OpenFlags::read), 0), start, length));
        ProcessFileTable& fileTable = process->GetFileTable();
        cmajor::systemx::kernel::Seek(process, resource->Fd(), resource->Start(), static_cast<int32_t>(Origin::seekSet));
        return fileTable.AddFile(resource.release());
    }
    else
    {
        throw SystemError(ENOTFOUND, "resource '" + resourceName + "' not found", __FUNCTION__);
    }
}

void CloseResource(Process* process, int32_t rd)
{
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(rd);
    fileTable.CloseFile(rd, process);
    delete file;
}

int64_t GetResourceSize(Process* process, int32_t rd)
{
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(rd);
    if (file->IsResource())
    {
        Resource* resource = static_cast<Resource*>(file);
        return resource->Length();
    }
    else
    {
        throw SystemError(EBADF, std::to_string(rd) + " is not a resource descriptor", __FUNCTION__);
    }
}

void ReadResource(Process* process, int32_t rd, int64_t offset, int64_t length, int64_t bufferAddr)
{
    Resource* resource = nullptr;
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(rd);
    if (file->IsResource())
    {
        resource = static_cast<Resource*>(file);
    }
    else
    {
        throw SystemError(EBADF, std::to_string(rd) + " is not a resource descriptor", __FUNCTION__);
    }
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    if (offset != 0)
    {
        cmajor::systemx::kernel::Seek(process, resource->Fd(), resource->Start() + offset, static_cast<int32_t>(Origin::seekSet));
    }
    int64_t bytesRead = cmajor::systemx::kernel::Read(process, resource->Fd(), bufferAddr, length);
    length -= bytesRead;
    bufferAddr += bytesRead;
    while (length > 0)
    {
        bytesRead = cmajor::systemx::kernel::Read(process, resource->Fd(), bufferAddr, length);
        length -= bytesRead;
        bufferAddr += bytesRead;
    }
}

} // namespace cmajor::systemx::kernel
