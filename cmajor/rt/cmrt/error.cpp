// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/error.hpp>
#include <cmrt/io.hpp>
#include <cmrt/trace.hpp>
#include <errno.h>
#include <memory>

void RtmPanic(const char* message)
{
    int errorId = 0;
    void* stderrx = RtmOpenStdFile(2, errorId);
    std::string errorMessage = "error: ";
    errorMessage.append(message).append("\n");
    RtmPrintToFile(stderrx, errorMessage.c_str());
    cmajor::rt::StackTrace stackTrace = cmajor::rt::GetStackTrace();
    if (!stackTrace.IsEmpty())
    {
        std::string trace = "\nSTACK TRACE:\n" + stackTrace.ToString();
        RtmPrintToFile(stderrx, trace.c_str());
    }
    std::exit(1);
}

void RtmFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber)
{
    int errorId = 0;
    void* stderrx = RtmOpenStdFile(2, errorId);
    std::string errorMessage = "assertion '";
    errorMessage.append(assertion).append("' failed: function=").append(function).append(", file=").append(sourceFilePath).append(", line=").append(
        std::to_string(lineNumber).append("\n"));
    RtmPrintToFile(stderrx, errorMessage.c_str());
    cmajor::rt::StackTrace stackTrace = cmajor::rt::GetStackTrace();
    if (!stackTrace.IsEmpty())
    {
        std::string trace = "\nSTACK TRACE:\n" + stackTrace.ToString();
        RtmPrintToFile(stderrx, trace.c_str());
    }
    std::exit(1);
}

namespace cmajor::rt {

    class Error
    {
    public:
        Error();
        Error(const std::string& message_, StackTrace&& stackTrace_);
        const char* Message();
    private:
        std::string message;
        StackTrace stackTrace;
        bool stackTraceGenerated;
    };

    Error::Error() : message(), stackTrace(), stackTraceGenerated(false)
    {
    }

    Error::Error(const std::string& message_, StackTrace&& stackTrace_) : message(message_), stackTrace(std::move(stackTrace_)), stackTraceGenerated(false)
    {
    }

    const char* Error::Message()
    {
        if (!stackTraceGenerated && !stackTrace.IsEmpty())
        {
            stackTraceGenerated = true;
            message.append("\nSTACK TRACE:\n").append(stackTrace.ToString());
        }
        return message.c_str();
    }

    class Errors
    {
    public:
        Errors();
        static Errors& Instance();
        int AllocateError(const std::string& errorMessage, StackTrace&& stackTrace);
        void DisposeError(int errorId);
        const char* GetErrorMessage(int errorId) const;
    private:
        std::vector<std::unique_ptr<Error>> errors;
    };

    Errors::Errors()
    {
        errors.push_back(std::unique_ptr<Error>(new Error()));
    }

    Errors& Errors::Instance()
    {
        static Errors instance;
        return instance;
    }

    int Errors::AllocateError(const std::string& errorMessage, StackTrace&& stackTrace)
    {
        int errorId = errors.size();
        errors.push_back(std::unique_ptr<Error>(new Error(errorMessage, std::move(stackTrace))));
        return errorId;
    }

    void Errors::DisposeError(int errorId)
    {
        if (errorId > 0 && errorId < errors.size())
        {
            errors[errorId].reset();
        }
    }

    const char* Errors::GetErrorMessage(int errorId) const
    {
        if (errorId > 0 && errorId < errors.size())
        {
            Error* error = errors[errorId].get();
            if (error)
            {
                return error->Message();
            }
        }
        return "";
    }

    int AllocateError(const std::string& errorMessage)
    {
        return Errors::Instance().AllocateError(errorMessage, GetStackTrace());
    }

} // cmajor::rt

const char* RtmGetErrorMessage(int errorId)
{
    if (errorId == 0) return "";
    return cmajor::rt::Errors::Instance().GetErrorMessage(errorId);
}

int RtmAllocateError(const char* errorMessage)
{
    return cmajor::rt::AllocateError(errorMessage);
}

void RtmDisposeError(int errorId)
{
    cmajor::rt::Errors::Instance().DisposeError(errorId);
}

int RtmGetErrno()
{
    return errno;
}