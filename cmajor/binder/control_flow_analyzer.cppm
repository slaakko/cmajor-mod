// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.control.flow.analyzer;

import std.core;

export namespace cmajor::binder {

class BoundCompileUnit;

void AnalyzeControlFlow(BoundCompileUnit& boundCompileUnit);

} // namespace cmajor::binder
