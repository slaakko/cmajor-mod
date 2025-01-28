// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.error;

import std.core;

export namespace cmajor::systemx::kernel {

const int EFAIL = 1;
const int EBADF = 2;
const int EPARAM = 3;
const int EMEMORYACCESS = 4;
const int ENOTFOUND = 5;
const int EALREADYEXISTS = 6;
const int ELIMITEXCEEDED = 7;
const int ENOCHILD = 8;
const int EHOST = 9;
const int EPERMISSION = 10;
const int ESOCKET = 11;

std::string ErrorCodeStr(int errorCode);
std::string ErrorMsg(int errorCode);
std::string HostErrorMessage(uint64_t errorCode);
int64_t GetLastHostErrorCode();

class SystemError : public std::runtime_error
{
public:
    SystemError();
    SystemError(int errorCode_, const std::string& message_, const std::string& function_);
    int ErrorCode() const { return errorCode; }
    int64_t HostErrorCode() const { return hostErrorCode; }
    void SetHostErrorCode(int32_t hostErrorCode_) { hostErrorCode = hostErrorCode_; }
    const std::string& Message() const { return message; }
private:
    int errorCode;
    int64_t hostErrorCode;
    std::string message;
    std::string function;
};

void ThrowLastHostError(const std::string& filePath);

} // namespace cmajor::systemx::kernel
