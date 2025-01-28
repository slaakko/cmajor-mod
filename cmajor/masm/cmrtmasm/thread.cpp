// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.thread;

namespace cmajor::masm::rt {

using ThreadFunction = void(*)();
using ThreadFunctionWithParam = void(*)(void*);
using ThreadMethod = void(*)(void*);
using ThreadMethodWithParam = void(*)(void*, void*);

void ExecuteThreadFunction(ThreadFunction threadFunction, int32_t threadId)
{
    threadFunction();
}

void ExecuteThreadFunctionWithParam(ThreadFunctionWithParam threadFunction, void* param, int32_t threadId)
{
    threadFunction(param);
}

void ExecuteThreadMethod(ThreadMethod threadMethod, void* object, int32_t threadId)
{
    threadMethod(object);
}

void ExecuteThreadMethodWithParam(ThreadMethodWithParam threadMethod, void* object, void* param, int32_t threadId)
{
    threadMethod(object, param);
}

class ThreadPool
{
public:
    static ThreadPool& Instance();
    void Exit();
    int32_t StartThreadFunction(ThreadFunction fun);
    int32_t StartThreadFunction(ThreadFunctionWithParam fun, void* param);
    int32_t StartThreadMethod(ThreadMethod method, void* object);
    int32_t StartThreadMethod(ThreadMethodWithParam method, void* object, void* param);
    bool JoinThread(int32_t threadId);
private:
    static std::unique_ptr<ThreadPool> instance;
    const int32_t numNoLockThreads = 256;
    std::atomic<int32_t> nextThreadId;
    std::vector<std::unique_ptr<std::thread>> noLockThreads;
    std::unordered_map<int32_t, std::unique_ptr<std::thread>> threadMap;
    std::mutex mtx;
    ThreadPool();
};

ThreadPool& ThreadPool::Instance()
{
    static ThreadPool instance;
    return instance;
}

void ThreadPool::Exit()
{
    try
    {
        for (std::unique_ptr<std::thread>& t : noLockThreads)
        {
            if (t.get())
            {
                if (t->joinable())
                {
                    t->join();
                }
            }
        }
        for (auto& p : threadMap)
        {
            JoinThread(p.first);
        }
    }
    catch (...)
    {
    }
}

std::unique_ptr<ThreadPool> ThreadPool::instance;

ThreadPool::ThreadPool() : nextThreadId(1), noLockThreads()
{
    noLockThreads.resize(numNoLockThreads);
}

int32_t ThreadPool::StartThreadFunction(ThreadFunction fun)
{
    int32_t threadId = nextThreadId++;
    if (threadId < numNoLockThreads)
    {
        noLockThreads[threadId].reset(new std::thread(ExecuteThreadFunction, fun, threadId));
        return threadId;
    }
    else
    {
        std::lock_guard<std::mutex> lock(mtx);
        threadMap[threadId].reset(new std::thread(ExecuteThreadFunction, fun, threadId));
        return threadId;
    }
}

int32_t ThreadPool::StartThreadFunction(ThreadFunctionWithParam fun, void* param)
{
    int32_t threadId = nextThreadId++;
    if (threadId < numNoLockThreads)
    {
        noLockThreads[threadId].reset(new std::thread(ExecuteThreadFunctionWithParam, fun, param, threadId));
        return threadId;
    }
    else
    {
        std::lock_guard<std::mutex> lock(mtx);
        threadMap[threadId].reset(new std::thread(ExecuteThreadFunctionWithParam, fun, param, threadId));
        return threadId;
    }
}

int32_t ThreadPool::StartThreadMethod(ThreadMethod method, void* object)
{
    int32_t threadId = nextThreadId++;
    if (threadId < numNoLockThreads)
    {
        noLockThreads[threadId].reset(new std::thread(ExecuteThreadMethod, method, object, threadId));
        return threadId;
    }
    else
    {
        std::lock_guard<std::mutex> lock(mtx);
        threadMap[threadId].reset(new std::thread(ExecuteThreadMethod, method, object, threadId));
        return threadId;
    }
}

int32_t ThreadPool::StartThreadMethod(ThreadMethodWithParam method, void* object, void* param)
{
    int32_t threadId = nextThreadId++;
    if (threadId < numNoLockThreads)
    {
        noLockThreads[threadId].reset(new std::thread(ExecuteThreadMethodWithParam, method, object, param, threadId));
        return threadId;
    }
    else
    {
        std::lock_guard<std::mutex> lock(mtx);
        threadMap[threadId].reset(new std::thread(ExecuteThreadMethodWithParam, method, object, param, threadId));
        return threadId;
    }
}

bool ThreadPool::JoinThread(int32_t threadId)
{
    if (threadId < numNoLockThreads)
    {
        if (noLockThreads[threadId])
        {
            if (noLockThreads[threadId]->joinable())
            {
                noLockThreads[threadId]->join();
            }
            noLockThreads[threadId].reset();
            return true;
        }
    }
    else
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = threadMap.find(threadId);
        if (it != threadMap.cend())
        {
            std::thread* thread = it->second.get();
            if (thread)
            {
                if (thread->joinable())
                {
                    thread->join();
                }
                threadMap.erase(threadId);
                return true;
            }
        }
    }
    return false;
}

void DoneThread()
{
    ThreadPool::Instance().Exit();
}

} // cmajor::masm::rt

int32_t RtmGetHardwareConcurrency()
{
    return std::thread::hardware_concurrency();
}

int32_t RtmStartThreadFunction(void* function)
{
    cmajor::masm::rt::ThreadFunction threadFun = reinterpret_cast<cmajor::masm::rt::ThreadFunction>(function);
    return cmajor::masm::rt::ThreadPool::Instance().StartThreadFunction(threadFun);
}

int32_t RtmStartThreadFunctionWithParam(void* function, void* param)
{
    cmajor::masm::rt::ThreadFunctionWithParam threadFunWithParam = reinterpret_cast<cmajor::masm::rt::ThreadFunctionWithParam>(function);
    return cmajor::masm::rt::ThreadPool::Instance().StartThreadFunction(threadFunWithParam, param);
}

struct ClassDelegate
{
    void* object;
    void* method;
};

int32_t RtmStartThreadMethod(void* classDelegate)
{
    ClassDelegate* clsDlg = reinterpret_cast<ClassDelegate*>(classDelegate);
    cmajor::masm::rt::ThreadMethod threadMethod = reinterpret_cast<cmajor::masm::rt::ThreadMethod>(clsDlg->method);
    return cmajor::masm::rt::ThreadPool::Instance().StartThreadMethod(threadMethod, clsDlg->object);
}

int32_t RtmStartThreadMethodWithParam(void* classDelegate, void* param)
{
    ClassDelegate* clsDlg = reinterpret_cast<ClassDelegate*>(classDelegate);
    cmajor::masm::rt::ThreadMethodWithParam threadMethodWithParam = reinterpret_cast<cmajor::masm::rt::ThreadMethodWithParam>(clsDlg->method);
    return cmajor::masm::rt::ThreadPool::Instance().StartThreadMethod(threadMethodWithParam, clsDlg->object, param);
}

bool RtmJoinThread(int32_t threadId)
{
    return cmajor::masm::rt::ThreadPool::Instance().JoinThread(threadId);
}

std::map<std::thread::id, int> threadIdMap;

int nextThreadId = 0;
std::mutex threadIdMapMutex;

int32_t RtmThisThreadId()
{
    std::lock_guard<std::mutex> lock(threadIdMapMutex);
    std::thread::id id = std::this_thread::get_id();
    auto it = threadIdMap.find(id);
    if (it != threadIdMap.cend())
    {
        return it->second;
    }
    int threadId = nextThreadId++;
    threadIdMap[id] = threadId;
    return threadId;
}
