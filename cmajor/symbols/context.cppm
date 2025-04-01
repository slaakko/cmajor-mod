// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.context;

import std.core;

export namespace cmajor::symbols {

class Module;

class Context
{
public:
    Context();
    Module* RootModule() const { return rootModule; }
    void SetRootModule(Module* rootModule_) { rootModule = rootModule_; }
private:
    Module* rootModule;
};

} // namespace cmajor::symbols
