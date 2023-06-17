// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.cpp.backend.value.stack;

import cmajor.cpp.ir;
import cmajor.ir.value.stack;
import util;
import std.core;

export namespace cmajor::cpp::backend {

class ValueStack : public cmajor::ir::ValueStack
{
public:
    void Push(void* val) override
    {
        cmajor::cpp::ir::Value* value = static_cast<cmajor::cpp::ir::Value*>(val);
        s.push_back(value);
    }
    void* Pop() override
    {
        Assert(!s.empty(), "value stack is empty");
        cmajor::cpp::ir::Value* top = s.back();
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
    std::vector<cmajor::cpp::ir::Value*> s;
};

} // namespace cmajor::cpp::backend
