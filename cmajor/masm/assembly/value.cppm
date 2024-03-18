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
    void SetName(const std::string& name_);
    virtual std::string ToString() const { return Name(); }
    int Length() const { return ToString().length(); }
    virtual bool CanSplit() const { return false; }
    virtual Value* Split(int length) { return nullptr; }
private:
    std::string name;
};

} // namespace cmajor::masm::assembly
