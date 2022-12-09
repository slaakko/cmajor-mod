export module cmajor.symbols.error;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/SymbolsApi.hpp>
//#include <stdexcept>
import std.core;

export namespace cmajor::symbols {

class Error : public std::runtime_error
{
public:
    Error(const std::string& message_);
    const std::string& Message() const { return message; }
private:
    std::string message;
};
} // namespace cmajor::symbols
