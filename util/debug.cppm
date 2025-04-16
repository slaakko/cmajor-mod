// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.debug;

import std.core;
import util.synchronized.queue;

export namespace util {

void DebugBreak();

class UnexpectedExecutorThread : public std::runtime_error
{
public:
    UnexpectedExecutorThread();
};

class DebugLogMessage
{
public:
    DebugLogMessage(const std::string& msg_);
    const std::string& Msg() const { return msg; }
private:
    std::string msg;
};

class DebugLog
{
public:
    static DebugLog& Instance();
    void Open();
    bool IsOpen() const { return open; }
    void Run();
    void Close();
    void Write(const std::string& msg);
private:
    DebugLog();
    void WriteMessages();
    bool open;
    std::string filePath;
    std::ofstream stream;
    std::thread thread;
    SynchronizedQueue<DebugLogMessage*> queue;
    std::chrono::steady_clock::time_point startTime;
    bool exiting;
};

} // namespace util
