// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.host.file.manager;

import cmajor.systemx.kernel.file;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class HostFile
{
public:
    HostFile(int32_t id_, const std::string& filePath_, OpenFlags flags_);
    int32_t Id() const { return id; }
    const std::string& FilePath() const { return filePath; }
    bool IsOpened() const { return (openFlags & OpenFlags::opened) != OpenFlags::none; }
    void SetOpened() { openFlags = openFlags | OpenFlags::opened; }
    bool IsRandomAccess() const { return (openFlags & OpenFlags::random_access) != OpenFlags::none; }
    bool Truncate() const { return (openFlags & OpenFlags::truncate) != OpenFlags::none; }
    bool Create() const { return (openFlags & OpenFlags::create) != OpenFlags::none; }
    void* FileHandle() const { return fileHandle; }
    void SetFileHandle(void* fileHandle_) { fileHandle = fileHandle_; }
    OpenFlags GetOpenFlags() const { return openFlags; }
    void SetOpenFlags(OpenFlags flags) { openFlags = flags; }
    int32_t ReferenceCount() const { return referenceCount; }
    void IncrementReferenceCount() { ++referenceCount; }
    void DecrementReferenceCount() { --referenceCount; }
    int32_t OutstandingRequests() const { return outstandingRequests; }
    void IncrementOutstandingRequests() { ++outstandingRequests; }
    void DecrementOutstandingRequests() { --outstandingRequests; }
private:
    int32_t id;
    std::string filePath;
    OpenFlags openFlags;
    void* fileHandle;
    int32_t referenceCount;
    int32_t outstandingRequests;
};

int32_t OpenHostFile(const std::string& filePath, OpenFlags flags);
void CloseHostFile(int32_t hostFileId);
HostFile* GetHostFile(int32_t hostFileId);
void InitHostFileManager();
void DoneHostFileManager();
void StartHostFileManager();
void StopHostFileManager();
void SetHostFileManagerMachine(cmajor::systemx::machine::Machine* machine);

} // namespace cmajor::systemx::kernel
