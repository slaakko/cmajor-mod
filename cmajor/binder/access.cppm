// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.access;

import std.core;
import cmajor.symbols;

export namespace cmajor::binder {

void CheckAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol, cmajor::symbols::Context* context);
bool HasAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol);

} // namespace cmajor::binder
