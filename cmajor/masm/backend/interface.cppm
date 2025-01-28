// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.backend.interface;

import std.core;
import cmajor.ir;

export namespace cmajor::masm::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel);
std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext);

} // namespace cmajor::masm::backend
