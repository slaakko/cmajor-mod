// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.value;

import std.core;

export namespace cmajor::masm::assembly {

class Value
{
public:
    Value(const std::string& name_);
    virtual ~Value();
    const std::string& Name() const { return name; }
    virtual std::string ToString() const { return Name(); }
private:
    std::string name;
};

} // namespace cmajor::masm::assembly
