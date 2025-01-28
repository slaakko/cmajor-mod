// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.file;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

enum class OpenFlags : int32_t
{
    none = 0, read = 1 << 0, write = 1 << 1, create = 1 << 2, append = 1 << 3, truncate = 1 << 4, text = 1 << 5, random_access = 1 << 6, opened = 1 << 7
};

constexpr OpenFlags operator|(OpenFlags left, OpenFlags right)
{
    return OpenFlags(int32_t(left) | int32_t(right));
}

constexpr OpenFlags operator&(OpenFlags left, OpenFlags right)
{
    return OpenFlags(int32_t(left) & int32_t(right));
}

constexpr OpenFlags operator~(OpenFlags flags)
{
    return OpenFlags(~int32_t(flags));
}

enum class Origin : int32_t
{
    seekSet = 0, seekCur = 1, seekEnd = 2
};

class File
{
public:
    File(const std::string& name_);
    const std::string& Name() const { return name; }
    virtual ~File();
    virtual bool IsShareable() const { return true; }
    virtual File* Share() { return this; }
    virtual void Release(cmajor::systemx::kernel::Process* process) { }
    virtual void Close(cmajor::systemx::kernel::Process* process) = 0;
    virtual bool IsReadable() const = 0;
    virtual bool IsWritable() const = 0;
    virtual bool IsConsole() const = 0;
    virtual bool IsHostTextFile() const = 0;
    virtual bool IsResource() const = 0;
    virtual bool IsDecompressionFile() const = 0;
    virtual bool HasColors() const = 0;
    virtual int Columns() const = 0;
    virtual int Rows() const = 0;
    virtual int CursorX() const = 0;
    virtual int CursorY() const = 0;
    virtual void SetCursorPos(int32_t cursorX, int32_t cursorY);
    virtual void SetCooked();
    virtual void SetRaw();
    virtual void SetEcho(bool echo);
    virtual void PushLines();
    virtual void PopLines();
    virtual bool IsDirFile() const { return false; }
    virtual std::vector<uint8_t> Read(int64_t count, cmajor::systemx::machine::Process* process);
    virtual int64_t Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process);
    virtual int64_t Seek(int64_t offset, Origin whence, cmajor::systemx::machine::Process* process);
    virtual int64_t Tell(cmajor::systemx::machine::Process* process);
    virtual void Bind(int32_t md);
    virtual void Unbind();
    virtual void SetTabMsgQueues(int32_t putMd, int32_t getMd);
    virtual void ResetTabMsgQueues();
    virtual void PushPid(int32_t pid);
    virtual void PopPid();
private:
    std::string name;
};

class ProcessFileTable
{
public:
    ProcessFileTable();
    int32_t AddFile(File* file);
    void CloseFile(int32_t fd, cmajor::systemx::kernel::Process* process);
    File* GetFile(int32_t fd) const;
    void CopyFrom(const ProcessFileTable& that);
    void CloseFiles(cmajor::systemx::kernel::Process* process);
    int32_t Dup(int32_t fd);
private:
    std::vector<File*> files;
};

void SetTerminalFile(File* terminalFile);
File* GetTerminalFile();
void WriteToTerminal(const std::string& text, cmajor::systemx::machine::Process* process);

} // namespace cmajor::systemx::kernel
