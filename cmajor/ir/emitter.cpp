// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ir.emitter;
import util;

namespace cmajor::ir {

Emitter::Emitter() : stack(nullptr), generateLocationInfo(false)
{
}

Emitter::~Emitter()
{
}

} // namespace cmajor::ir
