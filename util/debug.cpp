// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.debug;

import std.filesystem;
import util.unicode;
import util.path;
import util.time;

namespace util {

UnexpectedExecutorThread::UnexpectedExecutorThread() : std::runtime_error("unexpected executor thread")
{
}

void DebugBreak()
{
    ::DebugBreak();
}

DebugLogMessage::DebugLogMessage(const std::string& msg_) : msg(msg_)
{
}

DebugLog::DebugLog() : open(false), filePath(Path::Combine(Path::Combine(CmajorRoot(), "log"), "debug.txt")), exiting(false)
{
}

DebugLog& DebugLog::Instance()
{
    static DebugLog instance;
    return instance;
}

void RunDebugLog()
{
    DebugLog::Instance().Run();
}

void DebugLog::Open()
{
    thread = std::thread(RunDebugLog);
    startTime = std::chrono::steady_clock::now();
    std::filesystem::remove(filePath);
    queue.Put(new DebugLogMessage(GetCurrentDateTime().ToString()));
    open = true;
}

void DebugLog::Run()
{
    while (!exiting)
    {
        WriteMessages();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void DebugLog::WriteMessages()
{
    int n = queue.Count();
    if (n > 0)
    {
        std::ofstream stream(filePath, std::ios_base::app);
        for (int i = 0; i < n; ++i)
        {
            std::unique_ptr<DebugLogMessage> msg(queue.Get());
            if (msg)
            {
                stream << msg->Msg() << "\n";
            }
            else
            {
                return;
            }
        }
    }
}

void DebugLog::Close()
{
    if (!open) return;
    exiting = true;
    queue.Exit();
    thread.join();
    WriteMessages();
}

void DebugLog::Write(const std::string& msg)
{
    if (!open) return;
    std::chrono::steady_clock::duration timestamp = std::chrono::steady_clock::now() - startTime;
    std::string str = DurationStr(timestamp);
    str.append(1, ' ').append(msg);
    queue.Put(new DebugLogMessage(str));
}

} // namespace util
