// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.operation;

import std.core;

export namespace cmajor::symbols {

class FunctionSymbol;

class Operation
{
public:
    virtual ~Operation();
    FunctionSymbol* Get(std::vector<Operation*>& operations);
    void Release();
protected:
    virtual FunctionSymbol* Create() const = 0;
private:
    std::unique_ptr<FunctionSymbol> fun;
};

} // namespace cmajor::symbols
