// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/environment.hpp>
#include <rt/string.hpp>
#include <rt/error.hpp>
#include <util/text_util.hpp>
#include <util/system.hpp>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <process.h>
#include <sys/stat.h>
#include <string.h>
#include <cstring>
#include <errno.h>
#else
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <errno.h>
#endif
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace cmajor::rt {

struct Exec
{
    std::string command;
    std::vector<std::pair<int, std::string>> redirections;
};

struct Handle
{
    Handle() : handle(-1) { }
    Handle(int handle_) : handle(handle_) { }
    Handle(Handle&& that) : handle(that.handle)
    {
        that.handle = -1;
    }
    operator int() const
    {
        return handle;
    }
    ~Handle()
    {
        if (handle != -1)
        {
            close(handle);
        }
    }
    int handle;
};

class Executor
{
public:
    static void Init();
    static void Done();
    static Executor& Instance() { return *instance; }
    int32_t BeginExecute(const char* command);
    void AddRedirection(int32_t execHandle, int handle, const char* file);
    int32_t EndExecute(int32_t execHandle);
private:
    static std::unique_ptr<Executor> instance;
    Executor();
    std::mutex mtx;
    int32_t nextExecHandle;
    std::unordered_map<int32_t, std::unique_ptr<Exec>> execMap;
    int32_t Execute(Exec* exec);
};

std::unique_ptr<Executor> Executor::instance;

void Executor::Init()
{
    instance.reset(new Executor());
}

void Executor::Done()
{
    instance.reset();
}

Executor::Executor() : nextExecHandle(1)
{
}

int32_t Executor::BeginExecute(const char* command)
{
    std::lock_guard<std::mutex> lock(mtx);
    int32_t execHandle = nextExecHandle++;
    std::unique_ptr<Exec> exec(new Exec());
    exec->command = command;
    execMap[execHandle] = std::move(exec);
    return execHandle;
}

void Executor::AddRedirection(int32_t execHandle, int handle, const char* file)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = execMap.find(execHandle);
    if (it != execMap.cend())
    {
        Exec* exec = it->second.get();
        exec->redirections.push_back(std::make_pair(handle, std::string(file)));
    }
}

int32_t Executor::EndExecute(int32_t execHandle)
{
    Exec* exec = nullptr;
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = execMap.find(execHandle);
        if (it != execMap.cend())
        {
            exec = it->second.get();
        }
    }
    if (exec)
    {
        int32_t exitCode = Execute(exec);
        std::lock_guard<std::mutex> lock(mtx);
        execMap.erase(execHandle);
        return exitCode;
    }
    else
    {
        return 1;
    }
}

#ifdef _WIN32

int32_t Executor::Execute(Exec* exec)
{
    std::vector<std::pair<int, Handle>> toRestore;
    for (const std::pair<int, std::string>& p : exec->redirections)
    {
        int handle = p.first;
        std::string file = p.second;
        Handle oldHandle = _dup(handle);
        if (oldHandle != -1)
        {
            toRestore.push_back(std::make_pair(handle, std::move(oldHandle)));
            int pmode = _S_IREAD | _S_IWRITE;
            Handle fd = _creat(file.c_str(), pmode);
            if (fd != -1)
            {
                _dup2(fd, handle);
            }
        }
    }
    int32_t exitCode = system(exec->command.c_str());
    for (std::pair<int, Handle>& p : toRestore)
    {
        int handle = p.first;
        Handle old = std::move(p.second);
        _dup2(old, handle);
    }
    return exitCode;
}

#else

int32_t Executor::Execute(Exec* exec)
{
    std::vector<std::pair<int, Handle>> toRestore;
    for (const std::pair<int, std::string>& p : exec->redirections)
    {
        int handle = p.first;
        std::string file = p.second;
        Handle oldHandle = dup(handle);
        if (oldHandle != -1)
        {
            toRestore.push_back(std::make_pair(handle, std::move(oldHandle)));
            int pmode = S_IRUSR | S_IWUSR | S_IRGRP;
            Handle fd = creat(file.c_str(), pmode);
            if (fd != -1)
            {
                dup2(fd, handle);
            }
        }
    }
    int32_t exitCode = system(exec->command.c_str());
    for (std::pair<int, Handle>& p : toRestore)
    {
        int handle = p.first;
        Handle old = std::move(p.second);
        dup2(old, handle);
    }
    return exitCode;
}

#endif

void InitEnvironment()
{
    Executor::Init();
}

void DoneEnvironment()
{
    Executor::Done();
}

} // namespace cmajor::rt

const char* RtGetEnvironmentVariable(const char* environmentVariableName)
{
    const char* envVar = std::getenv(environmentVariableName);
    if (envVar)
    {
        return envVar;
    }
    return "";
}

std::mutex mtx;

int32_t RtGetCurrentWorkingDirectoryHandle()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::unique_ptr<char[]> buffer(new char[8192]);
    if (getcwd(buffer.get(), 8192))
    {
        return cmajor::rt::InstallString(buffer.get());
    }
    else
    {
        return cmajor::rt::InstallError(std::string("could not get current working directory: ") + util::PlatformStringToUtf8(std::strerror(errno)));
    }
}

const char* RtGetCurrentWorkingDirectory(int32_t currentWorkingDirectoryHandle)
{
    return cmajor::rt::GetString(currentWorkingDirectoryHandle);
}

void RtFreeCurrentWorkingDirectoryHandle(int32_t currentWorkingDirectoryHandle)
{
    cmajor::rt::DisposeString(currentWorkingDirectoryHandle);
}

int32_t RtExecute(const char* command)
{
    return system(command);
}

int32_t RtBeginExec(const char* command)
{
    return cmajor::rt::Executor::Instance().BeginExecute(command);
}

void RtAddRedirection(int32_t execHandle, int handle, const char* file)
{
    cmajor::rt::Executor::Instance().AddRedirection(execHandle, handle, file);
}

int32_t RtEndExec(int32_t execHandle)
{
    return cmajor::rt::Executor::Instance().EndExecute(execHandle);
}

int32_t RtBeginGetPathToExecutable()
{
    std::string pathToExecutable = util::GetPathToExecutable();
    return cmajor::rt::InstallString(pathToExecutable);
}

const char* RtGetPathToExecutable(int32_t pathHandle)
{
    return cmajor::rt::GetString(pathHandle);
}

void RtEndGetPathToExecutable(int32_t pathHandle)
{
    cmajor::rt::DisposeString(pathHandle);
}

const char* RtGetOsInfo()
{
#ifdef _WIN32
    return "windows";
#else
    static char buf[1024];
    std::memset(buf, 0, 1024);
    FILE* fp = fopen("/proc/version", "r");
    if (fp != NULL)
    {
        const char* s = fgets(buf, 1023, fp);
        fclose(fp);
        if (s == NULL)
        {
            return "ERROR";
        }
    }
    else
    {
        return "ERROR";
    }
    return buf;
#endif
}
