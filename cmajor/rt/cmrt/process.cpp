// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/process.hpp>
#include <cmrt/error.hpp>
#include <cmrt/strings.hpp>
#include <util/process.hpp>

void* RtmCreateProcess(const char* command, int32_t redirections, int32_t& errorId)
{
    errorId = 0;
    std::string cmd(command);
    try
    {
        util::Process* process = new util::Process(cmd, static_cast<util::Process::Redirections>(redirections));
        return process;
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError("error creating process '" + cmd + "': " + ex.what());
    }
    return nullptr;
}

void RtmDeleteProcess(void* process)
{
    delete static_cast<util::Process*>(process);
}

bool RtmProcessRunning(void* process, int32_t& errorId)
{
    errorId = 0;
    try
    {
        return static_cast<util::Process*>(process)->Running();
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
}

bool RtmProcessWaitForExit(void* process, int32_t& errorId)
{
    errorId = 0;
    try
    {
        static_cast<util::Process*>(process)->WaitForExit();
        return true;
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
}

int RtmProcessExitCode(void* process, int32_t& errorId)
{
    errorId = 0;
    try
    {
        return static_cast<util::Process*>(process)->ExitCode();
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return 1;
    }
}

bool RtmProcessTerminate(void* process, int32_t& errorId)
{
    errorId = 0;
    try
    {
        static_cast<util::Process*>(process)->Terminate();
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
    return true;
}

bool RtmProcessEof(void* process, int handle, int32_t& errorId)
{
    errorId = 0;
    try
    {
        return static_cast<util::Process*>(process)->Eof(static_cast<util::Process::StdHandle>(handle));
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return true;
    }
}

int32_t RtmProcessReadLine(void* process, int handle, int32_t& errorId)
{
    errorId = 0;
    try
    {
        std::string line = static_cast<util::Process*>(process)->ReadLine(static_cast<util::Process::StdHandle>(handle));
        return RtmAllocateString(line.c_str());
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return -1;
    }
}

int32_t RtmProcessReadToEnd(void* process, int handle, int32_t& errorId)
{
    errorId = 0;
    try
    {
        std::string str = static_cast<util::Process*>(process)->ReadToEnd(static_cast<util::Process::StdHandle>(handle));
        return RtmAllocateString(str.c_str());
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return -1;
    }
}

bool RtmProcessWriteLine(void* process, const char* line, int32_t& errorId)
{
    errorId = 0;
    try
    {
        std::string lineStr(line);
        static_cast<util::Process*>(process)->WriteLine(lineStr);
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
    return true;
}
