// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.io;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

enum class IOControlItem : int32_t
{
    isConsole = 0,
    isHostTextFile = 1,
    hasColors = 2,
    columns = 3,
    rows = 4,
    cursorX = 5,
    cursorY = 6,
    set_cursor_pos = 7,
    set_cooked = 8,
    set_raw = 9,
    set_echo = 10,
    push_lines = 11,
    pop_lines = 12,
    tab = 13,
    untab = 14,
    push_pid = 15,
    pop_pid = 16
};

std::vector<std::uint8_t> MakeStringBuffer(const std::string& str, int64_t maxLength);
std::vector<uint8_t> ReadProcessMemory(Process* process, int64_t addr, int64_t count);
void WriteProcessMemory(Process* process, int64_t addr, const std::vector<uint8_t>& buffer);
std::string ReadString(Process* process, int64_t address, cmajor::systemx::machine::Memory& mem);
int64_t WriteString(Process* process, const std::string& s, int64_t address, cmajor::systemx::machine::Memory& mem);
std::vector<std::string> ReadStringPointerArray(Process* process, int64_t arrayAddress, cmajor::systemx::machine::Memory& mem);
int32_t Create(Process* process, int64_t pathAddr, int32_t mode);
int32_t Open(Process* process, int64_t pathAddr, int32_t flags, int32_t mode);
int32_t Open(Process* process, const std::string& path, int32_t flags, int32_t mode);
void Close(Process* process, int32_t fd);
int64_t Write(Process* process, int32_t fd, int64_t bufferAddr, int64_t count);
int64_t Read(Process* process, int32_t fd, int64_t bufferAddr, int64_t count);
int32_t IOCtl(Process* process, int32_t fd, int32_t item, int64_t argAddr, int64_t argSize);
void Unlink(Process* process, int64_t pathAddr);
int64_t Seek(Process* process, int32_t fd, int64_t offset, int32_t whence);
int64_t Tell(Process* process, int32_t fd);
void Stat(Process* process, int64_t pathAddr, int64_t statBufAddr, int32_t statBufSize);
void GetCWD(Process* process, int64_t bufAddr, int64_t bufSize);
void ChDir(Process* process, int64_t pathAddr);
void MkDir(Process* process, int64_t pathAddr, int32_t mode);
int32_t OpenDir(Process* process, int64_t pathAddr);
void CloseDir(Process* process, int32_t dfd);
int32_t ReadDir(Process* process, int32_t dfd, int64_t dirEntryBufAddr, int64_t dirEntryBufSize);
void UTime(Process* process, int64_t pathAddr, int64_t timeBufAddr, int64_t timeBufSize);
std::vector<uint8_t> ReadFile(Process* process, int64_t pathAddr);
int32_t Dup(Process* process, int32_t fd);
void ChMod(Process* process, int64_t pathAddr, int32_t mode);
void ChOwn(Process* process, int64_t pathAddr, int32_t uid, int32_t gid);
void Rename(Process* process, int64_t sourcePathAddr, int64_t targetPathAddr);
void AddDirChangeNotification(Process* process, int64_t directoryPathsAddr);
void BindTerminal(Process* process, int32_t md);
void UnbindTerminal(Process* process);
void LockIO(cmajor::systemx::machine::UserProcess* process);
void UnlockIO(cmajor::systemx::machine::UserProcess* process);

} // namespace cmajor::systemx::kernel
