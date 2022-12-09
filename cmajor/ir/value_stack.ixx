// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ir.value.stack;

//#include <cmajor/ir/IrApi.hpp>

export namespace cmajor::ir {

class ValueStack
{
public:
    virtual void Push(void* value) = 0;
    virtual void* Pop() = 0;
    virtual void Dup() = 0;
    virtual void Swap() = 0;
    virtual void Rotate() = 0;
};
} // namespace cmajor::ir

