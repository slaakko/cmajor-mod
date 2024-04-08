// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/pool/pool_alloc.hpp>

module cmajor.systemx.kernel.io.manager;

import cmajor.systemx.kernel.host.file.manager;
import cmajor.systemx.kernel.os.api;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.debug.help;
import util;
import std.filesystem;

namespace cmajor::systemx::kernel {

const uint64_t exitCompletionKey = -1;

class IOManager
{
public:
    static IOManager& Instance();
    void Start();
    void Stop();
    void SetMachine(cmajor::systemx::machine::Machine* machine_) { machine = machine_; }
    void RunRequestHandler();
    void RunCompletionHandler();
    int32_t Read(int32_t hostFileId, Block* block);
    int32_t Write(int32_t hostFileId, Block* block);
    IORequest* GetRequest(int32_t requestId);
    void DeleteRequest(int32_t requestId);
    void HandleRequest(IORequest* request);
    void Exit();
private:
    IOManager();
    bool started;
    void RemoveIfExists(HostFile* hostFile);
    void OpenHostFile(HostFile* hostFile);
    uint64_t GetFileOffset(Block* block);
    cmajor::systemx::machine::Machine* machine;
    static std::unique_ptr<IOManager> instance;
    std::map<int32_t, IORequest*> requestMap;
    int32_t nextRequestId;
    std::list<int32_t, boost::fast_pool_allocator<int32_t>> requestQueue;
    std::map<void*, int32_t> overlappedMap;
    bool exiting;
    std::condition_variable_any requestQueueNotEmptyOrExitingVar;
    std::thread requestHandlerThread;
    std::thread completionHandlerThread;
    void* completionPortHandle;
};

IOManager& IOManager::Instance()
{
    static IOManager instance;
    return instance;
}

void RunRequestHandlerThread()
{
    IOManager::Instance().RunRequestHandler();
}

void RunCompletionHandlerThread()
{
    IOManager::Instance().RunCompletionHandler();
}

std::unique_ptr<IOManager> IOManager::instance;

IOManager::IOManager() : nextRequestId(0), exiting(false), machine(nullptr), completionPortHandle(nullptr), started(false)
{
}

void IOManager::Start()
{
    nextRequestId = 0;
    exiting = false;
    completionHandlerThread = std::thread(RunCompletionHandlerThread);
    requestHandlerThread = std::thread(RunRequestHandlerThread);
    started = true;
}

void IOManager::Stop()
{
    Exit();
    requestQueue.clear();
    requestMap.clear();
    started = false;
}

void IOManager::RunRequestHandler()
{
    try
    {
        while (!exiting)
        {
#if (LOCK_DEBUG)
            DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | HANDLE_REQUEST);
#endif 
            std::unique_lock<std::recursive_mutex> lock(machine->Lock());
            requestQueueNotEmptyOrExitingVar.wait(lock, [this] { return !requestQueue.empty() || exiting; });
            if (exiting)
            {
                return;
            }
#if (LOCK_DEBUG)
            DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | HANDLE_REQUEST);
#endif 
            int32_t requestId = requestQueue.front();
            requestQueue.pop_front();
            IORequest* request = GetRequest(requestId);
            if (request)
            {
                HandleRequest(request);
            }
        }
    }
    catch (...)
    {
        machine->SetException(std::current_exception());
    }
}

void IOManager::HandleRequest(IORequest* request)
{
    try
    {
        HostFile* hostFile = GetHostFile(request->HostFileId());
        if (!hostFile)
        {
            throw SystemError(EFAIL, "host file id " + std::to_string(request->HostFileId()) + " not found", __FUNCTION__);
        }
        if (!hostFile->IsOpened())
        {
            hostFile->SetOpened();
            OpenHostFile(hostFile);
        }
        uint64_t offset = GetFileOffset(request->GetBlock());
        void* overlapped = OsCreateOverlapped(offset);
        overlappedMap[overlapped] = request->Id();
        hostFile->IncrementOutstandingRequests();
        switch (request->Kind())
        {
        case RequestKind::read:
        {
            OsReadFile(hostFile->FileHandle(), request->GetBlock()->Data(), Block::Size(), overlapped);
            break;
        }
        case RequestKind::write:
        {
            OsWriteFile(hostFile->FileHandle(), request->GetBlock()->Data(), Block::Size(), overlapped);
            break;
        }
        }
    }
    catch (const SystemError& error)
    {
        request->SetError(error);
        cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::ioEvent, request->Id());
        Wakeup(evnt);
    }
}

void IOManager::RunCompletionHandler()
{
    try
    {
        completionPortHandle = OsCreateIoCompletionPort();
        while (!exiting)
        {
            uint64_t numberOfBytes = 0;
            uint64_t completionKey = 0;
            void* overlapped = nullptr;
            bool result = OsGetQueuedCompletionStatus(completionPortHandle, &numberOfBytes, &completionKey, &overlapped);
            if (exiting || completionKey == exitCompletionKey)
            {
                break;
            }
#if (LOCK_DEBUG)
            DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | HANDLE_COMPLETION);
#endif 
            std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
            DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | HANDLE_COMPLETION);
#endif 
            int32_t hostFileId = static_cast<int32_t>(completionKey);
            HostFile* hostFile = GetHostFile(hostFileId);
            if (hostFile)
            {
                hostFile->DecrementOutstandingRequests();
            }
            if (result)
            {
                auto it = overlappedMap.find(overlapped);
                if (it != overlappedMap.cend())
                {
                    int32_t requestId = it->second;
                    IORequest* request = GetRequest(requestId);
                    request->SetBytesTransferred(numberOfBytes);
                    overlappedMap.erase(overlapped);
                    OsDestroyOverlapped(overlapped);
                    cmajor::systemx::machine::Event evnt(cmajor::systemx::machine::EventKind::ioEvent, request->Id());
                    Wakeup(evnt);
                }
            }
            else
            {
                ThrowLastHostError("");
            }
        }
        OsCloseIoCompletionPort(completionPortHandle);
    }
    catch (...)
    {
        machine->SetException(std::current_exception());
    }
}

void IOManager::RemoveIfExists(HostFile* hostFile)
{
    if (std::filesystem::exists(hostFile->FilePath()))
    {
        std::error_code ec;
        std::filesystem::remove(hostFile->FilePath(), ec);
        if (ec)
        {
            throw SystemError(EFAIL, util::PlatformStringToUtf8(ec.message()), __FUNCTION__);
        }
    }
}

void IOManager::OpenHostFile(HostFile* hostFile)
{
    if (hostFile->Truncate())
    {
        RemoveIfExists(hostFile);
    }
    bool randomAccess = hostFile->IsRandomAccess();
    if (hostFile->Create() || hostFile->Truncate())
    {
        void* hostFileHandle = OsCreateHostFile(hostFile->FilePath().c_str(), randomAccess);
        hostFile->SetFileHandle(hostFileHandle);
    }
    else
    {
        void* hostFileHandle = OsOpenHostFile(hostFile->FilePath().c_str(), randomAccess);
        hostFile->SetFileHandle(hostFileHandle);
    }
    uint64_t completionKey = hostFile->Id();
    OsAssociateFileWithCompletionPort(hostFile->FileHandle(), completionPortHandle, completionKey);
}

uint64_t IOManager::GetFileOffset(Block* block)
{
    uint64_t fileOffset = static_cast<uint64_t>(block->Key().blockNumber) * Block::Size();
    return fileOffset;
}

void IOManager::Exit()
{
    if (!started) return;
    exiting = true;
    requestQueueNotEmptyOrExitingVar.notify_one();
    if (requestHandlerThread.joinable())
    {
        requestHandlerThread.join();
    }
    if (completionPortHandle)
    {
        OsPostQueuedCompletionStatus(completionPortHandle, 0, exitCompletionKey);
        completionHandlerThread.join();
    }
}

int32_t IOManager::Read(int32_t hostFileId, Block* block)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | READ);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | READ);
#endif 
    IORequest* request = new IORequest(RequestKind::read, nextRequestId++, hostFileId, block);
    requestMap[request->Id()] = request;
    requestQueue.push_back(request->Id());
    requestQueueNotEmptyOrExitingVar.notify_one();
    return request->Id();
}

int32_t IOManager::Write(int32_t hostFileId, Block* block)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | WRITE);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | WRITE);
#endif 
    IORequest* request = new IORequest(RequestKind::write, nextRequestId++, hostFileId, block);
    requestMap[request->Id()] = request;
    requestQueue.push_back(request->Id());
    requestQueueNotEmptyOrExitingVar.notify_one();
    return request->Id();
}

IORequest* IOManager::GetRequest(int32_t requestId)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | GET_REQUEST);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | GET_REQUEST);
#endif 
    auto it = requestMap.find(requestId);
    if (it != requestMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void IOManager::DeleteRequest(int32_t requestId)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), IO_MANAGER, 0, NO_LOCK | DELETE_REQUEST);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), IO_MANAGER, 0, HAS_LOCK | DELETE_REQUEST);
#endif 
    IORequest* request = GetRequest(requestId);
    if (request)
    {
        requestMap.erase(request->Id());
        delete request;
    }
}

IORequest::IORequest(RequestKind kind_, int32_t id_, int32_t hostFileId_, Block* block_) :
    kind(kind_), id(id_), hostFileId(hostFileId_), block(block_), overlapped(nullptr), bytesTransferred(0)
{
}

void IORequest::SetError(const SystemError& error_)
{
    error = error_;
}

int32_t Read(int32_t hostFileId, Block* block)
{
    return IOManager::Instance().Read(hostFileId, block);
}

int32_t Write(int32_t hostFileId, Block* block)
{
    return IOManager::Instance().Write(hostFileId, block);
}

IORequest* GetRequest(int32_t requestId)
{
    return IOManager::Instance().GetRequest(requestId);
}

void DeleteRequest(int32_t requestId)
{
    IOManager::Instance().DeleteRequest(requestId);
}

void StartIOManager()
{
    IOManager::Instance().Start();
}

void StopIOManager()
{
    IOManager::Instance().Stop();
}

void SetIOManagerMachine(cmajor::systemx::machine::Machine* machine)
{
    IOManager::Instance().SetMachine(machine);
}

} // namespace cmajor::systemx::kernel
