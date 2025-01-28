// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.rex.context;

import std.core;
import soul.rex.nfa;
import soul.rex.symbol;

export namespace soul::rex::context {

class Context
{
public:
    Context();
    ~Context();
    soul::rex::nfa::NfaState* MakeNfaState();
    soul::rex::symbol::Symbol* MakeChar(char32_t c);
    soul::rex::symbol::Symbol* MakeRange(char32_t start, char32_t end);
    soul::rex::symbol::Symbol* MakeAny() { return &any; }
    soul::rex::symbol::Symbol* MakeEpsilon() { return &epsilon; }
    soul::rex::symbol::Class* MakeClass();
private:
    std::vector<soul::rex::nfa::NfaState*> nfaStates;
    std::vector<soul::rex::symbol::Symbol*> symbols;
    soul::rex::symbol::Any any;
    soul::rex::symbol::Char epsilon;
    std::map<char32_t, soul::rex::symbol::Symbol*> charSymbols;
    std::map<soul::rex::symbol::Range, soul::rex::symbol::Symbol*> rangeSymbols;
};

} // namespace soul::rex::context
