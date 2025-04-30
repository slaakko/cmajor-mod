// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.systemx.backend.value.stack;

import cmajor.systemx.intermediate;
import cmajor.ir.value.stack;
import std.core;

export namespace cmajor::systemx::backend {

class ValueStack : public cmajor::ir::ValueStack
{
public:
    void Push(void* val) override
    {
        cmajor::systemx::intermediate::Value* value = static_cast<cmajor::systemx::intermediate::Value*>(val);
        s.push_back(value);
    }
    void* Pop() override
    {
        Assert(!s.empty(), "value stack is empty");
        cmajor::systemx::intermediate::Value* top = s.back();
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
    std::vector<cmajor::systemx::intermediate::Value*> s;
};

} // namespace cmajor::systemx::backend
