// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/IR/Value.h>

export module cmajor.llvm.value.stack;

import cmajor.ir;
import std.core;

export namespace cmajor::llvm {

class ValueStack : public cmajor::ir::ValueStack
{
public:
    void Push(void* val) override
    {
        ::llvm::Value* value = static_cast<::llvm::Value*>(val);
        s.push_back(value);
    }
    void* Pop() override
    {
        if (s.empty())
        {
            throw std::runtime_error("value stack is empty");
        }
        ::llvm::Value* top = s.back();
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
    std::vector<::llvm::Value*> s;
};

} // namespace cmajor::llvm
