// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_CONCEPTS_INCLUDED
#define LEXER_CONCEPTS_INCLUDED
#include <lexer/base.hpp>

namespace soul::lexer {

template<typename T>
concept LexicalIterator = requires(T it, int64_t pos)
{
    *it;                                        // returns current token id
    ++it;                                       // advances the lexer iterator ('eats' current token)
    it.GetPos();                                // returns current lexer position
    it.SetPos(pos);                             // backtracts to a saved lexer position
};

template<typename Machine, typename Char>
concept FiniteStateMachine = requires(Machine machine, Char chr, LexerBase<Char>&lexer)
{
    machine.NextState(int{}, chr, lexer);  // returns next state id when given current state id, current input char and lexer ref
    typename Variables;
};

template<typename Stack>
concept RuleStack = requires(Stack stack, int ruleId)
{
    stack.PushRule(ruleId);
    stack.PopRule();
};

} // namespace soul::lexer

#endif // LEXER_CONCEPTS_INCLUDED
