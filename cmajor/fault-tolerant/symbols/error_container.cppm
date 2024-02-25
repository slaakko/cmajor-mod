// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.error.container;

import std.core;

export namespace cmajor::fault::tolerant::symbols {

class ErrorContainer
{
public:
    ErrorContainer();
    void AddError(const std::runtime_error& error);
private:
    std::vector<std::runtime_error> errors;
};

} // namespace cmajor::fault::tolerant::symbols
