// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.io.manager;

import cmajor.systemx.kernel.error;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Block;

enum class RequestKind : int32_t
{
    read, write
};

class IORequest
{
public:
    IORequest(RequestKind kind_, int32_t id_, int32_t hostFileId_, Block* block_);
    RequestKind Kind() const { return kind; }
    int32_t Id() const { return id; }
    int32_t HostFileId() const { return hostFileId; }
    Block* GetBlock() const { return block; }
    void SetOverlapped(void* overlapped_) { overlapped = overlapped_; }
    void* Overlapped() const { return overlapped; }
    int64_t BytesTransferred() const { return bytesTransferred; }
    void SetBytesTransferred(int64_t bytesTransferred_) { bytesTransferred = bytesTransferred_; }
    bool Failed() const { return error.ErrorCode() != 0; }
    const SystemError& Error() const { return error; }
    void SetError(const SystemError& error_);
private:
    RequestKind kind;
    int32_t id;
    int32_t hostFileId;
    Block* block;
    SystemError error;
    void* overlapped;
    int64_t bytesTransferred;
};

int32_t Read(int32_t hostFileId, Block* block);
int32_t Write(int32_t hostFileId, Block* block);
IORequest* GetRequest(int32_t requestId);
void DeleteRequest(int32_t requestId);
void StartIOManager();
void StopIOManager();
void SetIOManagerMachine(cmajor::systemx::machine::Machine* machine);

} // namespace cmajor::systemx::kernel
