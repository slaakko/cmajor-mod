// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ir.gen.object;

import cmajor.ir.emitter;

namespace cmajor::ir {

GenObject::GenObject() : type(nullptr)
{
}

GenObject::~GenObject()
{
}

NativeValue::NativeValue(void* value_) : value(value_)
{
}

void NativeValue::Load(Emitter& emitter, OperationFlags flags)
{
    emitter.Stack().Push(value);
}

void NativeValue::Store(Emitter& emitter, OperationFlags flags)
{
    void* val = emitter.Stack().Pop();
    emitter.CreateStore(val, value);
}

} // namespace cmajor::ir
