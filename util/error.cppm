// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.error;

import std.core;

export namespace util {

class WindowsException : public std::runtime_error
{
public:
    WindowsException(uint64_t errorCode_);
    uint64_t ErrorCode() const { return errorCode; }
    std::string ErrorMessage() const { return what(); }
private:
    uint64_t errorCode;
};

} // util
