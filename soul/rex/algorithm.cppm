// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.rex.algorithm;

import std.core;
import soul.rex.nfa;

export namespace soul::rex::algorithm {

std::vector<soul::rex::nfa::NfaState*> EpsilonClosure(const std::vector<soul::rex::nfa::NfaState*>& states);
std::vector<soul::rex::nfa::NfaState*> EpsilonClosure(soul::rex::nfa::NfaState* state);
std::vector<soul::rex::nfa::NfaState*> Move(const std::vector<soul::rex::nfa::NfaState*>& states, char32_t c);
bool Match(soul::rex::nfa::Nfa& nfa, const std::u32string& s);

} // namespace soul::rex::algorithm
