// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_PROCESS_HPP
#define UTIL_PROCESS_HPP
#include <string>
#include <memory>

namespace util {

class ProcessImpl;

class Process
{
public:
    enum class Redirections : int
    {
        none = 0,
        processStdIn = 1 << 0,
        processStdOut = 1 << 1,
        processStdErr = 1 << 2
    };
    enum class StdHandle : int
    {
        stdOut = 1, stdErr = 2
    };
    Process(const std::string& command, Redirections redirections);
    ~Process();
    bool Running();
    void WaitForExit();
    int ExitCode() const;
    void Terminate();
    bool Eof(StdHandle handle);
    std::string ReadLine(StdHandle handle);
    std::string ReadToEnd(StdHandle handle);
    void WriteLine(const std::string& line);
private:
    ProcessImpl* impl;
};

constexpr Process::Redirections operator|(Process::Redirections left, Process::Redirections right)
{
    return Process::Redirections(int(left) | int(right));
}

constexpr Process::Redirections operator&(Process::Redirections left, Process::Redirections right)
{
    return Process::Redirections(int(left) & int(right));
}

int GetPid();

} // namespace util

#endif // UTIL_PROCESS_HPP
