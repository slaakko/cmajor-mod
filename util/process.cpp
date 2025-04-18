// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util_inc/native_process.hpp>

module util.process;

import util.execute_win;

#ifndef NDEBUG
#pragma comment(lib, "util_incd")
#else
#pragma comment(lib, "util_inc")
#endif

namespace util {

util_inc::NativeProcess::Redirections ConvertRedirections(Process::Redirections redirections)
{
    util_inc::NativeProcess::Redirections result = util_inc::NativeProcess::Redirections::none;
    if ((redirections & Process::Redirections::processStdIn) != Process::Redirections::none)
    {
        result = result | util_inc::NativeProcess::Redirections::processStdIn;
    }
    if ((redirections & Process::Redirections::processStdOut) != Process::Redirections::none)
    {
        result = result | util_inc::NativeProcess::Redirections::processStdOut;
    }
    if ((redirections & Process::Redirections::processStdErr) != Process::Redirections::none)
    {
        result = result | util_inc::NativeProcess::Redirections::processStdErr;
    }
    return result;
}

util_inc::NativeProcess::StdHandle ConvertHandle(Process::StdHandle stdHandle)
{
    switch (stdHandle)
    {
        case Process::StdHandle::stdOut:
        {
            return util_inc::NativeProcess::StdHandle::stdOut;
        }
        case Process::StdHandle::stdErr:
        {
            return util_inc::NativeProcess::StdHandle::stdErr;
        }
    }
    return util_inc::NativeProcess::StdHandle::stdOut;
}

Process::Process(const std::string& command, Redirections redirections) : nativeProcess(new util_inc::NativeProcess(command, ConvertRedirections(redirections)))
{
}

Process::~Process()
{
    delete nativeProcess;
}

bool Process::Running()
{
    return nativeProcess->Running();
}

void Process::WaitForExit()
{
    nativeProcess->WaitForExit();
}

int Process::ExitCode() const
{
    return nativeProcess->ExitCode();
}

void Process::Terminate()
{
    nativeProcess->Terminate();
}

bool Process::Eof(StdHandle handle)
{
    return nativeProcess->Eof(ConvertHandle(handle));
}

std::string Process::ReadLine(StdHandle handle)
{
    if (nativeProcess)
    {
        return nativeProcess->ReadLine(ConvertHandle(handle));
    }
    else
    {
        return std::string();
    }
}

std::string Process::ReadToEnd(StdHandle handle)
{
    return nativeProcess->ReadToEnd(ConvertHandle(handle));
}

void Process::WriteLine(const std::string& line)
{
    nativeProcess->WriteLine(line);
}

int GetPid()
{
    return util_inc::GetNativePid();
}

#ifdef _WIN32

ExecuteResult Execute(const std::string& commandLine)
{
    std::pair<int, std::string> executeResult = ExecuteWin(commandLine);
    ExecuteResult result(executeResult.first, std::move(executeResult.second));
    return result;
}

#else

ExecuteResult Execute(const std::string& commandLine)
{
    Process process(commandLine, Process::Redirections::processStdOut | Process::Redirections::processStdErr);
    process.WaitForExit();
    std::string stdoutText = process.ReadToEnd(Process::StdHandle::stdOut);
    std::string stderrText = process.ReadToEnd(Process::StdHandle::stdErr);
    int exitCode = process.ExitCode();
    ExecuteResult result(exitCode, stdoutText + stderrText);
    return result;
}

#endif

} // namespace util
