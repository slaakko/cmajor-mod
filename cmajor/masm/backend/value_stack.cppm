// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.masm.backend.value.stack;

import cmajor.masm.ir;
import cmajor.ir.value.stack;
import std.core;

export namespace cmajor::masm::backend {

class ValueStack : public cmajor::ir::ValueStack
{
public:
    void Push(void* val) override
    {
        cmajor::masm::ir::Value* value = static_cast<cmajor::masm::ir::Value*>(val);
        s.push_back(value);
    }
    void* Pop() override
    {
        Assert(!s.empty(), "value stack is empty");
        cmajor::masm::ir::Value* top = s.back();
        s.pop_back();
        return top;
    }
    void Dup() override
    {
        s.push_back(s.back());
    }
    void Swap() override
    {
        std::swap(s.back(), s[s.size() - 2]);
    }
    void Rotate() override
    {
        std::swap(s[s.size() - 3], s[s.size() - 2]);
        std::swap(s.back(), s[s.size() - 2]);
    }
private:
    std::vector<cmajor::masm::ir::Value*> s;
};

} // namespace cmajor::masm::backend
