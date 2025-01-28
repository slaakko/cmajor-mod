// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.rex;

export import soul.rex.token;
export import soul.rex.context;
export import soul.rex.nfa;
export import soul.rex.lexer;
export import soul.rex.parser;
export import soul.rex.symbol;
export import soul.rex.algorithm;
export import soul.rex.match;

export namespace soul::rex {

using Context = soul::rex::context::Context;
using Nfa = soul::rex::nfa::Nfa;

} // namespace soul::rex
