// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.executor.console;

import cmajor.systemx.kernel;
import util;
import std.core;

export namespace cmajor::systemx::executor {

class Console : public cmajor::systemx::kernel::File
{
public:
    Console();
    bool IsReadable() const override { return true; }
    bool IsWritable() const override { return true; }
    bool IsConsole() const override { return true; }
    bool IsHostTextFile() const override { return false; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return false; }
    bool HasColors() const override { return true; }
    int Columns() const override;
    int Rows() const override;
    int CursorX() const override { return 0; }
    int CursorY() const override { return 0; }
    void Close(cmajor::systemx::kernel::Process* process) override;
    std::vector<uint8_t> Read(int64_t count, cmajor::systemx::machine::Process* process) override;
    int64_t Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process) override;
    void SetToUtf16Mode();
    void SetToTextMode();
private:
    void AllocateBuffer();
    int bufferSize;
    util::Utf8ToUtf32Engine utf8Engine;
    bool stdInInUtf16Mode;
    bool stdOutInUtf16Mode;
    bool stdErrInUtf16Mode;
    std::string stdInBuf;
    std::unique_ptr<char16_t[]> utf16buffer;
};

} // namespace cmajor::systemx::executor
