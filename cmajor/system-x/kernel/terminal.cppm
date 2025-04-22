// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.terminal;

import cmajor.systemx.kernel.file;
import std.core;

export namespace cmajor::systemx::kernel {

class TerminalFile : public File
{
public:
    TerminalFile();
    bool IsShareable() const override { return true; }
    File* Share() override { return this; }
    void Release(cmajor::systemx::kernel::Process* process) override { }
    void Close(cmajor::systemx::kernel::Process* process) override { }
    bool IsReadable() const override { return true; }
    bool IsWritable() const override { return true; }
    bool IsConsole() const override { return true; }
    bool IsHostTextFile() const override { return false; }
    bool IsResource() const override { return false; }
    bool IsDecompressionFile() const override { return false; }
    bool HasColors() const override { return true; }
    int Columns() const override;
    int Rows() const override;
    int CursorX() const override;
    int CursorY() const override;
    void SetCursorPos(int32_t cursorX, int32_t cursorY) override;
    void SetCooked() override;
    void SetRaw() override;
    void SetEcho(bool echo) override;
    void PushLines() override;
    void PopLines() override;
    bool IsDirFile() const override { return false; }
    std::vector<uint8_t> Read(int64_t count, cmajor::systemx::machine::Process* process) override;
    int64_t Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process) override;
    void Bind(int32_t md) override;
    void Unbind() override;
    void SetTabMsgQueues(int32_t putTabMd, int32_t getTabMd) override;
    void ResetTabMsgQueues() override;
    void PushPid(int32_t pid) override;
    void PopPid() override;
};

void SetTerminalMachine(cmajor::systemx::machine::Machine* machine);
bool TerminalStarted();
void StartTerminal();
void StopTerminal();
void InitTerminal();
void DoneTerminal();
std::string ReadLineFromTerminal();

} // namespace cmajor::systemx::kernel

