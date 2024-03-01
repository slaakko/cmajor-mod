// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.rex.context;

namespace soul::rex::context {

Context::Context() : any(), epsilon(soul::rex::symbol::eps)
{
}

Context::~Context()
{
    for (soul::rex::nfa::NfaState* state : nfaStates)
    {
        delete state;
    }
    for (soul::rex::symbol::Symbol* symbol : symbols)
    {
        delete symbol;
    }
}

soul::rex::nfa::NfaState* Context::MakeNfaState()
{
    soul::rex::nfa::NfaState* state = new soul::rex::nfa::NfaState();
    nfaStates.push_back(state);
    return state;
}

soul::rex::symbol::Symbol* Context::MakeChar(char32_t c)
{
    std::map<char32_t, soul::rex::symbol::Symbol*>::iterator it = charSymbols.find(c);
    if (it != charSymbols.end())
    {
        return it->second;
    }
    else
    {
        soul::rex::symbol::Symbol* symbol = new soul::rex::symbol::Char(c);
        symbols.push_back(symbol);
        charSymbols[c] = symbol;
        return symbol;
    }
}

soul::rex::symbol::Symbol* Context::MakeRange(char32_t start, char32_t end)
{
    soul::rex::symbol::Range range(start, end);
    std::map<soul::rex::symbol::Range, soul::rex::symbol::Symbol*>::iterator it = rangeSymbols.find(range);
    if (it != rangeSymbols.end())
    {
        return it->second;
    }
    else
    {
        soul::rex::symbol::Symbol* symbol = new soul::rex::symbol::Range(start, end);
        symbols.push_back(symbol);
        rangeSymbols[range] = symbol;
        return symbol;
    }
}

soul::rex::symbol::Class* Context::MakeClass()
{
    soul::rex::symbol::Class* cls = new soul::rex::symbol::Class();
    symbols.push_back(cls);
    return cls;
}

} // namespace soul::rex::context
