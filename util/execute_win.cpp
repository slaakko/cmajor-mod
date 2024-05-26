// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#define BUFSIZE 4096 
#endif

module util.execute_win;

import util.unicode;

namespace util {

#ifdef _WIN32

std::pair<int, std::string> Error(const std::string& command, const std::string& function)
{
    std::u16string fn = ToUtf16(function);
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    PCSTR lpszFunction = (PCSTR)fn.c_str();

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    std::string errorMessage = "error executing command '" + command + "':\n" + ToUtf8((char16_t*)(LPTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    std::pair<int, std::string> error(dw, std::move(errorMessage));
    return error;
}

std::pair<int, std::string> ExecuteWin(const std::string& command)
{
    HANDLE g_hChildStd_OUT_Rd = nullptr;
    HANDLE g_hChildStd_OUT_Wr = nullptr;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = nullptr;

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
    {
        return Error(command, "CreatePipe");
    }
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
    {
        return Error(command, "SetHandleInformation");
    }

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    BOOL bSuccess = FALSE;
    DWORD exitCode = 0;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    LPSTR cmd = (char*)command.c_str();

    bSuccess = CreateProcessA(nullptr,
        cmd,                // command line 
        nullptr,       // process security attributes 
        nullptr,        // primary thread security attributes 
        TRUE,             // handles are inherited 
        CREATE_NO_WINDOW, // creation flags 
        nullptr,            // use parent's environment 
        nullptr,         // use parent's current directory 
        &siStartInfo,        // STARTUPINFO pointer 
        &piProcInfo);   // receives PROCESS_INFORMATION 

    // If an error occurs return error. 
    if (!bSuccess)
    {
        return Error(command, "CreateProcess");
    }
    else
    {
        // Close handles to stdout pipe no longer needed by the child process.
        // If not explicitly closed, there is no way to recognize that the child process has ended.

        CloseHandle(g_hChildStd_OUT_Wr);
    }

    std::string output;
    DWORD dwRead;
    CHAR chBuf[BUFSIZE];

    for (;;)
    {
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;
        output.append(chBuf, dwRead);
    }

    WaitForSingleObject(piProcInfo.hProcess, INFINITE);
    GetExitCodeProcess(piProcInfo.hProcess, &exitCode);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    if (exitCode == 0)
    {
        return std::make_pair(exitCode, "executing command '" + command + "' succeeded.\n" + std::move(output));
    }
    else
    {
        return std::make_pair(exitCode, "executing command '" + command + "' failed with exit code: " + std::to_string(exitCode) + ":\n" + std::move(output));
    }
}

#else

std::pair<int, std::string> ExecuteWin(const std::string& command)
{
   throw std::runtime_error("ExecuteWin supports only Windows")
}

#endif

} // namespace util
