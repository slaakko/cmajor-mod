// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.backend.interface;

import std.core;
import cmajor.ir;

export namespace cmajor::systemx::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel);
std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext);

} // namespace cmajor::systemx::backend
