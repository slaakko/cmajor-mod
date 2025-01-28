// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.compression;

import cmajor.systemx.kernel.file;
import util;

export namespace cmajor::systemx::kernel {

class DecompressionFile : public File
{
public:
    DecompressionFile(Process* process, uint64_t sourceBufferAddr, uint64_t count);
    bool IsShareable() const override { return false; }
    void Close(cmajor::systemx::kernel::Process* process) override;
    bool IsReadable() const override { return false; }
    bool IsWritable() const override { return false; }
    bool IsConsole() const override { return false; }
    bool IsHostTextFile() const override { return false; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return true; }
    bool HasColors() const override { return false; }
    int Columns() const override { return 0; }
    int Rows() const override { return 0; }
    int CursorX() const override { return 0; }
    int CursorY() const override { return 0; }
    int64_t Size() const { return decompressedMemoryStream.Content().size(); }
    void GetData(Process* process, uint64_t targetBufferAddr, uint64_t count);
private:
    std::vector<uint8_t> compressedData;
    util::MemoryStream decompressedMemoryStream;
};

int32_t Decompress(Process* process, int64_t bufferAddr, int64_t count);
int64_t GetDecompressedDataSize(Process* process, int32_t dd);
void GetDecompressedData(Process* process, int32_t dd, int64_t bufferAddr, int64_t count);
void CloseDecompression(Process* process, int32_t dd);

} // namespace cmajor::systemx::kernel
