export module cmajor.symbols.operation;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <cmajor/symbols/FunctionSymbol.hpp>
//import cmajor.symbols.function.symbol;
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

