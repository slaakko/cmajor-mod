// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.concepts;

import std.core;
import soul.lexer.base;

export namespace soul::lexer {

template<typename T>
concept LexicalIterator = requires(T it, int64_t pos)
{
    *it;                                        // returns current token id
    ++it;                                       // advances the lexer iterator ('eats' current token)
    it.GetPos();                                // returns current lexer position
    it.SetPos(pos);                             // backtracts to a saved lexer position
};

template<typename Machine, typename Char>
concept FiniteStateMachine = requires(Machine machine, Char chr, LexerBase<Char>& lexer)
{
    machine.NextState(int{}, chr, lexer);  // returns next state id when given current state id, current input char and lexer ref
};

template<typename Stack>
concept RuleStack = requires(Stack stack, int ruleId)
{
    stack.PushRule(ruleId); 
    stack.PopRule();
};

} // namespace soul::lexer
