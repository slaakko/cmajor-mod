// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol.factory;

import cmajor.fault.tolerant.symbols.symbol;
import soul.ast.span;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

Symbol* CreateSymbol(SymbolKind kind, const std::u16string& name, const soul::ast::Span& span);

} // namespace cmajor::fault::tolerant::symbols
