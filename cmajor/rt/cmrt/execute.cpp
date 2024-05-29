// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/execute.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <map>
#include <direct.h>
#include <io.h>
#include <process.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

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
    static Executor& Instance();
    int32_t BeginExecute(const char* command);
    void AddRedirection(int32_t execHandle, int handle, const char* file);
    int32_t EndExecute(int32_t execHandle);
private:
    Executor();
    int32_t Execute(Exec* exec);
    std::mutex mtx;
    int32_t nextExecHandle;
    std::map<int32_t, std::unique_ptr<Exec>> execMap;
};

Executor& Executor::Instance()
{
    static Executor instance;
    return instance;
}

Executor::Executor() : nextExecHandle(1)
{
}

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

} // namespace cmajor::rt

int32_t RtmExecute(const char* command)
{
    return system(command);
}

int32_t RtmBeginExec(const char* command)
{
    return cmajor::rt::Executor::Instance().BeginExecute(command);
}

void RtmAddRedirection(int32_t execHandle, int handle, const char* file)
{
    cmajor::rt::Executor::Instance().AddRedirection(execHandle, handle, file);
}

int32_t RtmEndExec(int32_t execHandle)
{
    return cmajor::rt::Executor::Instance().EndExecute(execHandle);
}
