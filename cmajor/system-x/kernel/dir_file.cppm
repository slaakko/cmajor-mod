// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.dir.file;

import cmajor.systemx.kernel.file;
import cmajor.systemx.kernel.fs;

export namespace cmajor::systemx::kernel {

class DirFile : public File
{
public:
    DirFile(const std::string& name_);
    bool IsShareable() const override { return false; }
    bool IsDirFile() const override { return true; }
    bool IsReadable() const override { return false; }
    bool IsWritable() const override { return false; }
    bool IsConsole() const override { return false; }
    bool IsHostTextFile() const override { return false; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return false; }
    bool HasColors() const override { return false; }
    int Columns() const override { return 0; }
    int Rows() const override { return 0; }
    int CursorX() const override { return 0; }
    int CursorY() const override { return 0; }
    virtual int32_t Read(DirectoryEntry& dirEntry, cmajor::systemx::machine::Process* process) = 0;
};

} // namespace cmajor::systemx::kernel
