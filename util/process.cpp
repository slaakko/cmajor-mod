// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/native_process.hpp>

module util.process;

namespace util {

NativeProcess::Redirections ConvertRedirections(Process::Redirections redirections)
{
    NativeProcess::Redirections result = NativeProcess::Redirections::none;
    if ((redirections & Process::Redirections::processStdIn) != Process::Redirections::none)
    {
        result = result | NativeProcess::Redirections::processStdIn;
    }
    if ((redirections & Process::Redirections::processStdOut) != Process::Redirections::none)
    {
        result = result | NativeProcess::Redirections::processStdOut;
    }
    if ((redirections & Process::Redirections::processStdErr) != Process::Redirections::none)
    {
        result = result | NativeProcess::Redirections::processStdErr;
    }
    return result;
}

NativeProcess::StdHandle ConvertHandle(Process::StdHandle stdHandle)
{
    switch (stdHandle)
    {
        case Process::StdHandle::stdOut:
        {
            return NativeProcess::StdHandle::stdOut;
        }
        case Process::StdHandle::stdErr:
        {
            return NativeProcess::StdHandle::stdErr;
        }
    }
    return NativeProcess::StdHandle::stdOut;
}

Process::Process(const std::string& command, Redirections redirections) : nativeProcess(new NativeProcess(command, ConvertRedirections(redirections)))
{
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
    return nativeProcess->ReadLine(ConvertHandle(handle));
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
    return GetNativePid();
}

} // namespace util
