// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.compression;

import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

DecompressionFile::DecompressionFile(Process* process, uint64_t sourceBufferAddr, uint64_t count) : File("DECOMPRESSION")
{
    compressedData = ReadProcessMemory(process, sourceBufferAddr, count);
    util::MemoryReader reader(compressedData.data(), 8);
    util::MemoryStream compressedMemoryStream(compressedData.data() + 8, std::max(static_cast<int64_t>(0), static_cast<int64_t>(compressedData.size()) - 8));
    util::DeflateStream extractStream(util::CompressionMode::decompress, compressedMemoryStream);
    int64_t size = reader.ReadLong();
    for (int64_t i = 0; i < size; ++i)
    {
        int x = extractStream.ReadByte();
        if (x != -1)
        {
            decompressedMemoryStream.Write(static_cast<uint8_t>(x));
        }
    }
}

void DecompressionFile::Close(cmajor::systemx::kernel::Process* process)
{
    compressedData.clear();
    delete this;
}

void DecompressionFile::GetData(Process* process, uint64_t targetBufferAddr, uint64_t count)
{
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    mem.NCopy(decompressedMemoryStream.Content().data(), process->RV(), targetBufferAddr, count);
}

int32_t Decompress(Process* process, int64_t bufferAddr, int64_t count)
{
    DecompressionFile* file = new DecompressionFile(process, bufferAddr, count);
    return process->GetFileTable().AddFile(file);
}

int64_t GetDecompressedDataSize(Process* process, int32_t dd)
{
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(dd);
    if (file->IsDecompressionFile())
    {
        DecompressionFile* decompressionFile = static_cast<DecompressionFile*>(file);
        return decompressionFile->Size();
    }
    else
    {
        throw SystemError(EBADF, std::to_string(dd) + " is not a decompression file descriptor", __FUNCTION__);
    }
}

void GetDecompressedData(Process* process, int32_t dd, int64_t bufferAddr, int64_t count)
{
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(dd);
    if (file->IsDecompressionFile())
    {
        DecompressionFile* decompressionFile = static_cast<DecompressionFile*>(file);
        decompressionFile->GetData(process, bufferAddr, count);
    }
    else
    {
        throw SystemError(EBADF, std::to_string(dd) + " is not a decompression file descriptor", __FUNCTION__);
    }
}

void CloseDecompression(Process* process, int32_t dd)
{
    ProcessFileTable& fileTable = process->GetFileTable();
    File* file = fileTable.GetFile(dd);
    if (file->IsDecompressionFile())
    {
        fileTable.CloseFile(dd, process);
    }
    else
    {
        throw SystemError(EBADF, std::to_string(dd) + " is not a decompression file descriptor", __FUNCTION__);
    }
}

} // namespace cmajor::systemx::kernel
