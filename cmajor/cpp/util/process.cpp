// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util/process.hpp>
#include <util/text_util.hpp>
#include <boost/process.hpp>
#ifdef _WIN32
#include <boost/process/windows.hpp>
#include <Windows.h>
#undef min
#undef max
#else
#include <sys/types.h>
#include <unistd.h>
#endif

namespace util {

class ProcessImpl
{
public:
    ProcessImpl(const std::string& command, Process::Redirections redirections_);
    ProcessImpl(const ProcessImpl&) = delete;
    ProcessImpl& operator=(const ProcessImpl&) = delete;
    ProcessImpl(ProcessImpl&&) = delete;
    ProcessImpl& operator=(ProcessImpl&&) = delete;
    bool Running();
    void WaitForExit();
    int ExitCode() const;
    void Terminate();
    bool Eof(Process::StdHandle handle);
    std::string ReadLine(Process::StdHandle handle);
    std::string ReadToEnd(Process::StdHandle handle);
    void WriteLine(const std::string& line);
private:
    Process::Redirections redirections;
    boost::process::ipstream processStdOut;
    boost::process::ipstream processStdErr;
    boost::process::opstream processStdIn;
    boost::process::child process;
    bool stdOutEof;
    bool stdErrEof;
};

ProcessImpl::ProcessImpl(const std::string& command, Process::Redirections redirections_) : redirections(redirections_), stdOutEof(false), stdErrEof(false)
{
    try
    {
        switch (redirections)
        {
        case Process::Redirections::none:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::windows::hide);
#else
            process = boost::process::child(command);
#endif
            break;
        }
        case Process::Redirections::processStdIn:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn);
#endif
            break;
        }
        case Process::Redirections::processStdOut:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_out > processStdOut);
#endif
            break;
        }
        case Process::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case Process::Redirections::processStdIn | Process::Redirections::processStdOut:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut);
#endif
            break;
        }
        case Process::Redirections::processStdIn | Process::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case Process::Redirections::processStdOut | Process::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr, boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr);
#endif
            break;
        }
        case Process::Redirections::processStdIn | Process::Redirections::processStdOut | Process::Redirections::processStdErr:
        {
#ifdef _WIN32
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr,
                boost::process::windows::hide);
#else
            process = boost::process::child(command, boost::process::std_in < processStdIn, boost::process::std_out > processStdOut, boost::process::std_err > processStdErr);
#endif
            break;
        }
        }
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("error running command '" + command + "': " + PlatformStringToUtf8(ex.what()));
    }
}

bool ProcessImpl::Running()
{
    return process.running();
}

void ProcessImpl::WaitForExit()
{
    process.wait();
}

int ProcessImpl::ExitCode() const
{
    return process.exit_code();
}

void ProcessImpl::Terminate()
{
    process.terminate();
}

bool ProcessImpl::Eof(Process::StdHandle handle)
{
    switch (handle)
    {
    case Process::StdHandle::stdOut:
    {
        if ((redirections & Process::Redirections::processStdOut) != Process::Redirections::none)
        {
            if (!stdOutEof && processStdOut) return false;
        }
        break;
    }
    case Process::StdHandle::stdErr:
    {
        if ((redirections & Process::Redirections::processStdErr) != Process::Redirections::none)
        {
            if (!stdErrEof && processStdErr) return false;
        }
        break;
    }
    }
    return true;
}

std::string ProcessImpl::ReadLine(Process::StdHandle handle)
{
    std::string line;
    switch (handle)
    {
    case Process::StdHandle::stdOut:
    {
        if ((redirections & Process::Redirections::processStdOut) != Process::Redirections::none)
        {
            if (processStdOut)
            {
                if (!std::getline(processStdOut, line))
                {
                    stdOutEof = true;
                }
            }
        }
        break;
    }
    case Process::StdHandle::stdErr:
    {
        if ((redirections & Process::Redirections::processStdErr) != Process::Redirections::none)
        {
            if (processStdErr)
            {
                if (!std::getline(processStdErr, line))
                {
                    stdErrEof = true;
                }
            }
        }
        break;
    }
    }
    return line;
}

std::string ProcessImpl::ReadToEnd(Process::StdHandle handle)
{
    std::string lines;
    bool first = true;
    while (!Eof(handle))
    {
        if (first)
        {
            first = false;
        }
        else
        {
            lines.append(1, '\n');
        }
        std::string line = ReadLine(handle);
        if (!line.empty())
        {
            lines.append(line);
        }
    }
    return lines;
}

void ProcessImpl::WriteLine(const std::string& line)
{
    if ((redirections & Process::Redirections::processStdIn) != Process::Redirections::none)
    {
        if (processStdIn)
        {
            processStdIn << line << std::endl;
        }
    }
    else
    {
        throw std::runtime_error("process stdin not redirected");
    }
}

Process::Process(const std::string& command, Redirections redirections) : impl(new ProcessImpl(command, redirections))
{
}

Process::~Process()
{
    delete impl;
}

bool Process::Running()
{
    return impl->Running();
}

void Process::WaitForExit()
{
    impl->WaitForExit();
}

int Process::ExitCode() const
{
    return impl->ExitCode();
}

void Process::Terminate()
{
    impl->Terminate();
}

bool Process::Eof(StdHandle handle)
{
    return impl->Eof(handle);
}

std::string Process::ReadLine(StdHandle handle)
{
    return impl->ReadLine(handle);
}

std::string Process::ReadToEnd(StdHandle handle)
{
    return impl->ReadToEnd(handle);
}

void Process::WriteLine(const std::string& line)
{
    impl->WriteLine(line);
}

int GetPid()
{
#ifdef _WIN32
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}

} // namespace util
