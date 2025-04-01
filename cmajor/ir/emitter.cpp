// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ir.emitter;
import util;

namespace cmajor::ir {

Emitter::Emitter() : stack(nullptr), generateLocationInfo(false), context(nullptr)
{
}

Emitter::~Emitter()
{
}

} // namespace cmajor::ir
