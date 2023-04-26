// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ir.emitter;
import util;

namespace cmajor::ir {

Emitter::Emitter(ValueStack* stack_) : stack(stack_)
{
}

Emitter::~Emitter()
{
}

} // namespace cmajor::ir
