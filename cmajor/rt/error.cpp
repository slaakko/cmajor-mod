// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/multiprecision/cpp_int.hpp>

module cmajor.rt.error;

import cmajor.rt.call.stack;
import cmajor.rt.io;
import cmajor.rt.classes;
import cmajor.eh;
import util;

namespace cmajor::rt {

class ErrorTable
{
public:
    static ErrorTable& Instance();
    int32_t InstallError(const std::string& errorMessage);
    const char* GetError(int32_t errorId);
    void DisposeError(int32_t errorId);
private:
    static std::unique_ptr<ErrorTable> instance;
    std::mutex mtx;
    std::unordered_map<int32_t, std::string> errorMap;
    int32_t nextErrorId;
    ErrorTable();
};

ErrorTable& ErrorTable::Instance()
{
    static ErrorTable instance;
    return instance;
}

ErrorTable::ErrorTable() : nextErrorId(1)
{
}

int32_t ErrorTable::InstallError(const std::string& errorMessage)
{
    std::lock_guard<std::mutex> lock(mtx);
    int32_t errorId = nextErrorId++;
    errorMap[errorId] = errorMessage;
    return errorId;
}

const char* ErrorTable::GetError(int32_t errorId)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = errorMap.find(errorId);
    if (it != errorMap.cend())
    {
        return it->second.c_str();
    }
    else
    {
        return "<error message not found>";
    }
}

void ErrorTable::DisposeError(int32_t errorId)
{
    std::lock_guard<std::mutex> lock(mtx);
    errorMap.erase(errorId);
}

int32_t InstallError(const std::string& errorMessage)
{
    return -ErrorTable::Instance().InstallError(errorMessage);
}

const char* GetError(int32_t errorId)
{
    return ErrorTable::Instance().GetError(-errorId);
}

void DisposeError(int32_t errorId)
{
    ErrorTable::Instance().DisposeError(-errorId);
}

#ifdef _WIN32

__declspec(thread) void* currentException = nullptr;
__declspec(thread) uint64_t currentExceptionClassId[2];

#else

__thread void* currentException = nullptr;
__thread uint64_t currentExceptionClassId[2];

#endif

} // namespace cmajor::rt


const char* RtGetError(int32_t errorId)
{
    return cmajor::rt::GetError(errorId);
}

void RtDisposeError(int32_t errorId)
{
    cmajor::rt::DisposeError(errorId);
}

void RtThrowException(void* exception, void* exceptionTypeId)
{
    cmajor::rt::currentException = exception;
    util::uuid* exTypeId = reinterpret_cast<util::uuid*>(exceptionTypeId);
    boost::multiprecision::uint128_t exceptionClassId = cmajor::rt::GetClassId(*exTypeId);
    cmajor::rt::currentExceptionClassId[0] = static_cast<uint64_t>(exceptionClassId >> 64);
    cmajor::rt::currentExceptionClassId[1] = static_cast<uint64_t>(exceptionClassId);
    throw cmajor::eh::Exception();
}

void RtCaptureException(void** exception, uint64_t& exceptionClassIdHi, uint64_t& exceptionClassIdLo)
{
    *exception = cmajor::rt::currentException;
    cmajor::rt::currentException = nullptr;
    exceptionClassIdHi = static_cast<uint64_t>(cmajor::rt::currentExceptionClassId[0]);
    exceptionClassIdLo = static_cast<uint64_t>(cmajor::rt::currentExceptionClassId[1]);
    cmajor::rt::currentExceptionClassId[0] = 0;
    cmajor::rt::currentExceptionClassId[1] = 0;
}

void RtThrowCapturedException(void* exception, uint64_t exceptionClassIdHi, uint64_t exceptionClassIdLo)
{
    cmajor::rt::currentException = exception;
    cmajor::rt::currentExceptionClassId[0] = exceptionClassIdHi;
    cmajor::rt::currentExceptionClassId[1] = exceptionClassIdLo;
    throw cmajor::eh::Exception();
}

bool RtHandleException(void* exceptionTypeId)
{
    boost::multiprecision::uint128_t currentExceptionClassId = boost::multiprecision::uint128_t(cmajor::rt::currentExceptionClassId[0]) << 64 | cmajor::rt::currentExceptionClassId[1];
    util::uuid* exTypeId = reinterpret_cast<util::uuid*>(exceptionTypeId);
    boost::multiprecision::uint128_t exceptionClassId = cmajor::rt::GetClassId(*exTypeId);
    bool handle = currentExceptionClassId % exceptionClassId == 0;
    return handle;
}

void* RtGetException()
{
    return cmajor::rt::currentException;
}

void* RtGetExceptionTypeId()
{
    return EhGetExceptionTypeId();
}
