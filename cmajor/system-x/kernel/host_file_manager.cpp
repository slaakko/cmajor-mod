// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.host.file.manager;

import cmajor.systemx.kernel.io.manager;
import cmajor.systemx.kernel.os.api;
import cmajor.systemx.kernel.debug.help;

namespace cmajor::systemx::kernel {

class HostFileManager
{
public:
    static void Init();
    static void Done();
    static HostFileManager& Instance() { return *instance; }
    void Start();
    void Stop();
    void SetMachine(cmajor::systemx::machine::Machine* machine_) { machine = machine_; }
    int32_t OpenHostFile(const std::string& filePath, OpenFlags flags);
    void CloseHostFile(int32_t hostFileId);
    HostFile* GetHostFile(int32_t hostFileId) const;
private:
    HostFileManager();
    static std::unique_ptr<HostFileManager> instance;
    cmajor::systemx::machine::Machine* machine;
    std::map<int32_t, HostFile*> hostFileMap;
    std::map<std::string, std::set<int32_t>> hostFilePathMap;
    int32_t nextHostFileId;
};

std::unique_ptr<HostFileManager> HostFileManager::instance;

void HostFileManager::Init()
{
    instance.reset(new HostFileManager());
}

void HostFileManager::Done()
{
    instance.reset();
}

void HostFileManager::Start()
{
    hostFileMap.clear();
    hostFilePathMap.clear();
}

void HostFileManager::Stop()
{
    hostFileMap.clear();
    hostFilePathMap.clear();
}

HostFileManager::HostFileManager() : nextHostFileId(0), machine(nullptr)
{
}

int32_t HostFileManager::OpenHostFile(const std::string& filePath, OpenFlags flags)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, NO_LOCK | OPEN_FILE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, HAS_LOCK | OPEN_FILE);
#endif 
    auto it = hostFilePathMap.find(filePath);
    if (it != hostFilePathMap.cend())
    {
        const std::set<int32_t>& hostFileIdSet = it->second;
        for (int32_t hostFileId : hostFileIdSet)
        {
            HostFile* prevHostFile = GetHostFile(hostFileId);
            if (prevHostFile)
            {
                if (prevHostFile->GetOpenFlags() == flags)
                {
                    prevHostFile->IncrementReferenceCount();
                    return hostFileId;
                }
            }
        }
    }
    HostFile* hostFile = new HostFile(nextHostFileId++, filePath, flags);
    hostFileMap[hostFile->Id()] = hostFile;
    hostFilePathMap[hostFile->FilePath()].insert(hostFile->Id());
    return hostFile->Id();
}

void HostFileManager::CloseHostFile(int32_t hostFileId)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, NO_LOCK | CLOSE_FILE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, HAS_LOCK | CLOSE_FILE);
#endif 
    HostFile* hostFile = GetHostFile(hostFileId);
    if (hostFile)
    {
        auto it = hostFilePathMap.find(hostFile->FilePath());
        if (it != hostFilePathMap.end())
        {
            std::set<int32_t>& hostFileIdSet = it->second;
            hostFileIdSet.erase(hostFileId);
            if (hostFileIdSet.empty())
            {
                hostFilePathMap.erase(hostFile->FilePath());
            }
        }
        hostFile->DecrementReferenceCount();
        if (hostFile->ReferenceCount() == 0)
        {
            if (hostFile->FileHandle())
            {
                OsCloseHostFile(hostFile->FileHandle());
            }
            hostFileMap.erase(hostFile->Id());
            delete hostFile;
        }
    }
}

HostFile* HostFileManager::GetHostFile(int32_t hostFileId) const
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, NO_LOCK | GET_HOST_FILE);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), HOST_FILE_MANAGER, 0, HAS_LOCK | GET_HOST_FILE);
#endif 
    auto it = hostFileMap.find(hostFileId);
    if (it != hostFileMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

HostFile::HostFile(int32_t id_, const std::string& filePath_, OpenFlags flags_) :
    id(id_), filePath(filePath_), openFlags(flags_), fileHandle(nullptr), referenceCount(1), outstandingRequests(0)
{
}

int32_t OpenHostFile(const std::string& filePath, OpenFlags flags)
{
    return HostFileManager::Instance().OpenHostFile(filePath, flags);
}

void CloseHostFile(int32_t hostFileId)
{
    HostFileManager::Instance().CloseHostFile(hostFileId);
}

HostFile* GetHostFile(int32_t hostFileId)
{
    return HostFileManager::Instance().GetHostFile(hostFileId);
}

void InitHostFileManager()
{
    HostFileManager::Init();
}

void DoneHostFileManager()
{
    HostFileManager::Done();
}

void StartHostFileManager()
{
    HostFileManager::Instance().Start();
}

void StopHostFileManager()
{
    HostFileManager::Instance().Stop();
}

void SetHostFileManagerMachine(cmajor::systemx::machine::Machine* machine)
{
    HostFileManager::Instance().SetMachine(machine);
}

} // namespace cmajor::systemx::kernel
