// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.access;

import std.core;
import cmajor.symbols;

export namespace cmajor::binder {

void CheckAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol);
bool HasAccess(cmajor::symbols::FunctionSymbol* fromFunction, cmajor::symbols::Symbol* toSymbol);

} // namespace cmajor::binder
