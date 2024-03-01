// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.re;

import soul.ast.slg;
import util;

namespace soul::ast::re {

Symbol::Symbol(SymbolKind kind_) : kind(kind_), contained(false), dontSetContained(false)
{
}

Symbol::~Symbol()
{
}

void Symbol::SetName(const std::string& name_)
{
    name = name_;
}

CharSymbol::CharSymbol(char32_t chr_) : Symbol(SymbolKind::charSymbol), chr(chr_)
{
    SetName("(" + util::ToUtf8(std::u32string(1, chr)) + ")");
}

bool CharSymbol::Match(char32_t c)
{
    return chr == c;
}

void CharSymbol::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharSymbol::Print(CodeFormatter& formatter)
{
    if (chr == eps)
    {
        formatter.Write("EPS");
    }
    else
    {
        formatter.Write(util::ToUtf8(util::CharStr(chr)));
    }
}

Any::Any() : Symbol(SymbolKind::anySymbol)
{
    SetName("(*)");
}

bool Any::Match(char32_t c)
{
    return true;
}

void Any::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void Any::Print(CodeFormatter& formatter)
{
    formatter.Write(".");
}

Range::Range() : Symbol(SymbolKind::rangeSymbol), start(), end()
{
}

Range::Range(char32_t start_, char32_t end_) : Symbol(SymbolKind::rangeSymbol), start(start_), end(end_)
{
    SetName("(" + util::ToUtf8(std::u32string(1, start)) + "-" + util::ToUtf8(std::u32string(1, end)) + ")");
}

bool Range::Match(char32_t c)
{
    return c >= start && c <= end;
}

void Range::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void Range::Print(CodeFormatter& formatter)
{
    formatter.Write(util::ToUtf8(util::CharStr(start)));
    formatter.Write("-");
    formatter.Write(util::ToUtf8(util::CharStr(end)));
}

bool Intersect(const Range& left, const Range& right)
{
    if (left.IsEmpty() || right.IsEmpty()) return false;
    if (left.Start() <= right.Start())
    {
        return right.Start() >= left.Start() && right.Start() <= left.End();
    }
    else
    {
        return left.Start() >= right.Start() && left.Start() <= right.End();
    }
}

Range operator&(const Range& left, const Range& right)
{
    if (Intersect(left, right))
    {
        Range intersection(std::max(left.Start(), right.Start()), std::min(left.End(), right.End()));
        return intersection;
    }
    return Range(static_cast<char32_t>(1), static_cast<char32_t>(0));
}

std::vector<Range> operator-(const Range& left, const Range& right)
{
    std::vector<Range> ranges;
    if (right.Start() <= left.Start() && right.End() >= left.End())
    {
        return ranges;
    }
    else if (right.Start() > left.End() || right.End() < left.Start())
    {
        ranges.push_back(left);
    }
    else
    {
        if (right.Start() >= left.Start() && right.Start() <= left.End())
        {
            if (left.Start() <= right.Start() - 1)
            {
                ranges.push_back(Range(left.Start(), right.Start() - 1));
            }
        }
        if (right.End() >= left.Start() && right.End() <= left.End())
        {
            if (right.End() + 1 <= left.End())
            {
                ranges.push_back(Range(right.End() + 1, left.End()));
            }
        }
    }
    return ranges;
}

std::vector<Range> operator~(const Range& that)
{
    std::vector<Range> result;
    if (that.Start() > char32_t(1))
    {
        result.push_back(Range(static_cast<char32_t>(1), static_cast<char32_t>(that.Start() - 1)));
    }
    if (static_cast<uint32_t>(that.End()) < 0x110000)
    {
        result.push_back(Range(static_cast<char32_t>(that.End() + 1), static_cast<char32_t>(0x10FFFF)));
    }
    return result;
}

Class::Class(int index_) : Symbol(SymbolKind::classSymbol), index(index_), inverse(false)
{
    SetName("[" + std::to_string(index) + "]");
}

void Class::SetIndex(int index_)
{
    index = index_;
    SetName("[" + std::to_string(index) + "]");
}

Class* Class::MakeCanonical(LexerContext& lexerContext)
{
    std::vector<Range> rangeVec;
    Class* canonicalClass = new Class(-1);
    for (Symbol* symbol : symbols)
    {
        if (symbol->Kind() == SymbolKind::charSymbol)
        {
            CharSymbol* chr = static_cast<CharSymbol*>(symbol);
            rangeVec.push_back(Range(chr->Chr(), chr->Chr()));
        }
        else if (symbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* range = static_cast<Range*>(symbol);
            rangeVec.push_back(*range);
        }
        else if (symbol->Kind() == SymbolKind::anySymbol)
        {
            throw std::runtime_error("class contains any");
        }
        else if (symbol->Kind() == SymbolKind::classSymbol)
        {
            throw std::runtime_error("class contains class");
        }
    }
    for (const Range& range : rangeVec)
    {
        canonicalClass->ranges.push_back(range);
    }
    if (inverse)
    {
        canonicalClass->MakeInverse(lexerContext);
    }
    return canonicalClass;
}

void Class::MakeMinimal(LexerContext& lexerContext)
{
    std::vector<Range> rangeVec;
    for (const Range& range : ranges)
    {
        rangeVec.push_back(range);
    }
    ranges.clear();
    std::sort(rangeVec.begin(), rangeVec.end());
    std::vector<Range>::iterator e = std::unique(rangeVec.begin(), rangeVec.end());
    rangeVec.erase(e, rangeVec.end());
    bool changed = true;
    while (changed)
    {
        changed = false;
        std::vector<Range> combinedRanges;
        for (int i = 0; i < rangeVec.size(); ++i)
        {
            bool combined = false;
            Range current = rangeVec[i];
            if (i > 0)
            {
                Range left = rangeVec[i - 1];
                if (left.End() + 1 == current.Start())
                {
                    combinedRanges.push_back(Range(left.Start(), current.End()));
                    combined = true;
                }
            }
            if (i < rangeVec.size() - 1)
            {
                Range right = rangeVec[i + 1];
                if (current.End() + 1 == right.Start())
                {
                    combinedRanges.push_back(Range(current.Start(), right.End()));
                    combined = true;
                }
            }
            if (combined)
            {
                changed = true;
            }
            else
            {
                combinedRanges.push_back(current);
            }
        }
        std::sort(combinedRanges.begin(), combinedRanges.end());
        std::vector<Range>::iterator e = std::unique(combinedRanges.begin(), combinedRanges.end());
        combinedRanges.erase(e, combinedRanges.end());
        std::swap(rangeVec, combinedRanges);
    }
    symbols.clear();
    chars.clear();
    for (const Range& range : rangeVec)
    {
        if (range.IsEmpty()) continue;
        ranges.push_back(range);
        if (chars.empty())
        {
            AddChar(range.Start());
        }
    }
}

void Class::MakeInverse(LexerContext& lexerContext)
{
    std::vector<Range> rangeVec;
    for (const Range& range : ranges)
    {
        rangeVec.push_back(range);
    }
    ranges.clear();
    std::vector<Range> inverseRanges;
    inverseRanges.push_back(Range(1, 0x10FFFF));
    for (const Range& range : rangeVec)
    {
        std::vector<Range> newInverse;
        for (const Range& i : inverseRanges)
        {
            if (Intersect(range, i))
            {
                Range intersection = range & i;
                Range left = Range(i.Start(), intersection.Start() - 1);
                if (!left.IsEmpty())
                {
                    newInverse.push_back(left);
                }
                Range right = Range(intersection.End() + 1, i.End());
                if (!right.IsEmpty())
                {
                    newInverse.push_back(right);
                }
            }
            else
            {
                newInverse.push_back(i);
            }
        }
        std::swap(inverseRanges, newInverse);
    }
    symbols.clear();
    chars.clear();
    for (const Range& range : inverseRanges)
    {
        ranges.push_back(range);
        if (chars.empty())
        {
            AddChar(range.Start());
        }
    }
    MakeMinimal(lexerContext);
    inverse = false;
}

Class* Class::Clone()
{
    Class* cls = new Class(-1);
    for (Symbol* symbol : symbols)
    {
        cls->AddSymbol(symbol);
    }
    for (char32_t c : chars)
    {
        cls->AddChar(c);
    }
    return cls;
}

bool Class::Match(char32_t c)
{
    bool match = false;
    for (Symbol* symbol : symbols)
    {
        if (symbol->Match(c))
        {
            match = true;
            break;
        }
    }
    return match != inverse;
}

void Class::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void Class::AddSymbol(Symbol* symbol)
{
    symbol->SetContained();
    symbols.push_back(symbol);
}

void Class::AddChar(char32_t c)
{
    chars.push_back(c);
}

void Class::Print(CodeFormatter& formatter)
{
    if (ranges.empty())
    {
        formatter.Write("[");
        if (inverse)
        {
            formatter.Write("^");
        }
        for (Symbol* symbol : symbols)
        {
            symbol->Print(formatter);
        }
        formatter.Write("]");
    }
    else
    {
        formatter.Write("{");
        for (auto& range : ranges)
        {
            range.Print(formatter);
        }
        formatter.Write("}");
    }
}

bool operator==(const Class& left, const Class& right)
{
    if (left.Symbols().size() != right.Symbols().size()) return false;
    int n = left.Symbols().size();
    for (int i = 0; i < n; ++i)
    {
        Symbol* leftSymbol = left.Symbols()[i];
        Symbol* rightSymbol = right.Symbols()[i];
        if (leftSymbol->Kind() == SymbolKind::rangeSymbol && rightSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            const Range* leftRange = static_cast<const Range*>(leftSymbol);
            const Range* rightRange = static_cast<const Range*>(rightSymbol);
            if (!(*leftRange == *rightRange))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

struct RangeEndLess
{
    bool operator()(const Range& left, const Range& right) const
    {
        if (left.End() < right.End()) return true;
        if (left.End() > right.End()) return false;
        return left.Start() < right.Start();
    }
};

bool Intersect(const Class& left, const Class& right)
{
    for (Symbol* leftSymbol : left.Symbols())
    {
        if (leftSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* leftRange = static_cast<Range*>(leftSymbol);
            for (Symbol* rightSymbol : right.Symbols())
            {
                if (rightSymbol->Kind() == SymbolKind::rangeSymbol)
                {
                    Range* rightRange = static_cast<Range*>(rightSymbol);
                    if (Intersect(*leftRange, *rightRange))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Class* MakeIntertersection(const Class& left, const Class& right, LexerContext& lexerContext)
{
    std::vector<Range> leftRanges;
    for (Symbol* leftSymbol : left.Symbols())
    {
        if (leftSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* leftRange = static_cast<Range*>(leftSymbol);
            leftRanges.push_back(*leftRange);
        }
    }
    std::vector<Range> rightRanges;
    for (Symbol* rightSymbol : right.Symbols())
    {
        if (rightSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* rightRange = static_cast<Range*>(rightSymbol);
            rightRanges.push_back(*rightRange);
        }
    }
    std::vector<Range> intersection;
    for (const Range& left : leftRanges)
    {
        std::vector<Range>::iterator start = std::lower_bound(rightRanges.begin(), rightRanges.end(), left);
        std::vector<Range>::iterator end = std::upper_bound(rightRanges.begin(), rightRanges.end(), left, RangeEndLess());
        if (start != rightRanges.begin()) --start;
        if (end < rightRanges.end()) ++end;
        for (std::vector<Range>::iterator i = start; i != end; ++i)
        {
            const Range& right = *i;
            if (left == right)
            {
                intersection.push_back(left);
                break;
            }
            else if (Intersect(left, right))
            {
                intersection.push_back(left & right);
            }
        }
    }
    std::sort(intersection.begin(), intersection.end());
    std::vector<Range>::iterator e = std::unique(intersection.begin(), intersection.end());
    intersection.erase(e, intersection.end());
    Class* cls = lexerContext.MakeClass();
    for (const Range& i : intersection)
    {
        cls->AddSymbol(lexerContext.MakeRange(i.Start(), i.End()));
        if (cls->Chars().empty())
        {
            cls->AddChar(i.Start());
        }
    }
    return cls;
}

Class* MakeDifference(const Class& left, const Class& right, LexerContext& lexerContext)
{
    std::vector<Range> leftRanges;
    for (Symbol* leftSymbol : left.Symbols())
    {
        if (leftSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* leftRange = static_cast<Range*>(leftSymbol);
            leftRanges.push_back(*leftRange);
        }
    }
    std::vector<Range> rightRanges;
    for (Symbol* rightSymbol : right.Symbols())
    {
        if (rightSymbol->Kind() == SymbolKind::rangeSymbol)
        {
            Range* rightRange = static_cast<Range*>(rightSymbol);
            rightRanges.push_back(*rightRange);
        }
    }
    std::vector<Range> difference;
    for (const Range& left : leftRanges)
    {
        bool found = false;
        std::vector<Range> diffs;
        std::vector<Range>::iterator start = std::lower_bound(rightRanges.begin(), rightRanges.end(), left);
        std::vector<Range>::iterator end = std::upper_bound(rightRanges.begin(), rightRanges.end(), left, RangeEndLess());
        if (start != rightRanges.begin()) --start;
        if (end < rightRanges.end()) ++end;
        for (std::vector<Range>::iterator i = start; i != end; ++i)
        {
            const Range& right = *i;
            if (left == right)
            {
                found = true;
                break;
            }
            else
            {
                if (Intersect(left, right))
                {
                    Range intersection = left & right;
                    Range l = Range(left.Start(), intersection.Start() - 1);
                    if (!l.IsEmpty())
                    {
                        diffs.push_back(l);
                    }
                    Range r = Range(intersection.End() + 1, left.End());
                    if (!r.IsEmpty())
                    {
                        diffs.push_back(r);
                    }
                }
            }
        }
        if (!found)
        {
            if (diffs.empty())
            {
                difference.push_back(left);
            }
            else
            {
                for (const Range& diff : diffs)
                {
                    difference.push_back(diff);
                }
            }
        }
    }
    Class* d = lexerContext.MakeClass();
    for (const Range& r : difference)
    {
        d->AddSymbol(lexerContext.MakeRange(r.Start(), r.End()));
        if (d->Chars().empty())
        {
            d->AddChar(r.Start());
        }
    }
    d->MakeMinimal(lexerContext);
    return d;
}

NfaEdge::NfaEdge(Symbol* symbol_, NfaState* next_) : symbol(symbol_), next(next_)
{
}

void NfaEdge::Print(CodeFormatter& formatter)
{
    symbol->Print(formatter);
    formatter.WriteLine(" -> " + std::to_string(next->Id()));
}

NfaState::NfaState(int id_, int ruleIndex_) : id(id_), ruleIndex(ruleIndex_), accept(false)
{
}

void NfaState::SetEdges(const std::vector<NfaEdge>& edges_)
{
    edges = edges_;
    for (NfaEdge& edge : edges)
    {
        edge.Next()->AddPrev(this);
    }
}

void NfaState::AddEdge(const NfaEdge& edge)
{
    edges.push_back(edge);
    edge.Next()->AddPrev(this);
}

void NfaState::AddPrev(NfaState* prev_)
{
    prev.insert(prev_);
}

std::vector<NfaState*> NfaState::Next(char32_t c) const
{
    std::vector<NfaState*> next;
    for (const auto& edge : edges)
    {
        Symbol* symbol = edge.GetSymbol();
        if (symbol->Match(c))
        {
            next.push_back(edge.Next());
        }
    }
    return next;
}

void NfaState::Print(CodeFormatter& formatter)
{
    std::string str = std::to_string(id);
    if (ruleIndex != -1)
    {
        str.append(" : RULE=").append(std::to_string(ruleIndex));
    }
    if (accept)
    {
        str.append(" : ACCEPT");
    }
    if (!prev.empty())
    {
        str.append(" : prev:");
        for (NfaState* p : prev)
        {
            str.append(" ").append(std::to_string(p->Id()));
        }
    }
    formatter.WriteLine(str);
    formatter.IncIndent();
    for (NfaEdge& edge : edges)
    {
        edge.Print(formatter);
    }
    formatter.DecIndent();
}

Nfa::Nfa() : start(nullptr), end(nullptr)
{
}

Nfa::Nfa(NfaState* start_, NfaState* end_) : start(start_), end(end_)
{
}

Nfa MakeNfa(LexerContext& lexerContext, Symbol* symbol)
{
    NfaState* start = lexerContext.MakeNfaState();
    NfaState* end = lexerContext.MakeNfaState();
    end->SetAccept(true);
    start->AddEdge(NfaEdge(symbol, end));
    return Nfa(start, end);
}

Nfa Cat(const Nfa& left, const Nfa& right)
{
    Nfa cat(left);
    NfaState* leftEnd = cat.End();
    leftEnd->SetAccept(false);
    NfaState* rightStart = right.Start();
    rightStart->SetRuleIndex(-1);
    leftEnd->SetEdges(rightStart->Edges());
    cat.SetEnd(right.End());
    return cat;
}

Nfa Alt(LexerContext& lexerContext, const Nfa& left, const Nfa& right)
{
    NfaState* leftStart = left.Start();
    NfaState* leftEnd = left.End();
    NfaState* rightStart = right.Start();
    NfaState* rightEnd = right.End();
    NfaState* start = lexerContext.MakeNfaState();
    NfaState* end = lexerContext.MakeNfaState();
    end->SetAccept(true);
    start->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), leftStart));
    start->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), rightStart));
    Nfa alt;
    alt.SetStart(start);
    leftEnd->SetAccept(false);
    leftEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), end));
    rightEnd->SetAccept(false);
    rightEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), end));
    alt.SetEnd(end);
    return alt;
}

Nfa Kleene(LexerContext& lexerContext, const Nfa& nfa)
{
    Nfa kleene;
    NfaState* start = lexerContext.MakeNfaState();
    NfaState* end = lexerContext.MakeNfaState();
    end->SetAccept(true);
    start->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), end));
    kleene.SetStart(start);
    NfaState* nfaStart = nfa.Start();
    start->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), nfaStart));
    NfaState* nfaEnd = nfa.End();
    nfaEnd->SetAccept(false);
    nfaEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), end));
    nfaEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), nfaStart));
    kleene.SetEnd(end);
    return kleene;
}

Nfa Pos(LexerContext& lexerContext, const Nfa& nfa)
{
    Nfa pos;
    NfaState* start = lexerContext.MakeNfaState();
    NfaState* end = lexerContext.MakeNfaState();
    end->SetAccept(true);
    pos.SetStart(start);
    NfaState* nfaStart = nfa.Start();
    start->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), nfaStart));
    NfaState* nfaEnd = nfa.End();
    nfaEnd->SetAccept(false);
    nfaEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), end));
    nfaEnd->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), nfaStart));
    pos.SetEnd(end);
    return pos;
}

Nfa Opt(LexerContext& lexerContext, const Nfa& nfa)
{
    Nfa opt(nfa);
    opt.Start()->AddEdge(NfaEdge(lexerContext.MakeEpsilon(), opt.End()));
    return opt;
}

DfaState::DfaState(int id_, const std::vector<int>& nfaStateIds_) : id(id_), nfaStateIds(nfaStateIds_), next(), marked(false), accept(false), ruleIndex(-1)
{
}

void DfaState::AddNext(DfaState* n)
{
    next.push_back(n);
}

DfaState* DfaState::Next(int i) const
{
    if (i >= 0 && i < next.size())
    {
        return next[i];
    }
    else
    {
        return nullptr;
    }
}

void DfaState::Print(LexerContext& context, CodeFormatter& formatter)
{
    std::string str = std::to_string(id);
    str.append("[");
    bool first = true;
    for (int nfaStateId : nfaStateIds)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(", ");
        }
        str.append(std::to_string(nfaStateId));
    }
    str.append("]");
    if (accept)
    {
        str.append(" : ACCEPT");
    }
    if (ruleIndex != -1)
    {
        str.append(" : RULE=" + std::to_string(ruleIndex));
    }
    formatter.WriteLine(str);
    formatter.IncIndent();
    int n = next.size();
    for (int i = 0; i < n; ++i)
    {
        if (next[i])
        {
            formatter.Write(std::to_string(i));
            if (context.Partition()[i]->Ranges().size() == 1)
            {
                context.Partition()[i]->Print(formatter);
            }
            formatter.WriteLine(" -> " + std::to_string(next[i]->Id()));
        }
    }
    formatter.DecIndent();
}

void Dfa::AddState(DfaState* state)
{
    states.push_back(state);
}

void Dfa::Finalize(LexerContext& lexerContext)
{
    for (auto state : states)
    {
        for (auto nfaStateId : state->NfaStateIds())
        {
            NfaState* nfaState = lexerContext.GetNfaState(nfaStateId);
            if (nfaState->Accept())
            {
                state->SetAccept(true);
                if (state->RuleIndex() == -1 || (nfaState->RuleIndex() != -1 && nfaState->RuleIndex() < state->RuleIndex()))
                {
                    state->SetRuleIndex(nfaState->RuleIndex());
                }
            }
        }
    }
}

void Dfa::Print(LexerContext& lexerContext, CodeFormatter& formatter)
{
    formatter.WriteLine("DFA:");
    for (DfaState* state : states)
    {
        state->Print(lexerContext, formatter);
    }
    formatter.WriteLine();
}

std::vector<int> EpsilonClosure(LexerContext& lexerContext, const std::vector<int>& stateIds)
{
    std::vector<int> stack;
    for (int state : stateIds)
    {
        stack.push_back(state);
    }
    std::vector<int> epsilonClosure = stateIds;
    while (!stack.empty())
    {
        int s = stack.back();
        stack.pop_back();
        NfaState* state = lexerContext.GetNfaState(s);
        std::vector<NfaState*> u = state->Next(eps);
        for (NfaState* vs : u)
        {
            int v = vs->Id();
            if (std::find(epsilonClosure.cbegin(), epsilonClosure.cend(), v) == epsilonClosure.cend())
            {
                epsilonClosure.push_back(v);
                stack.push_back(v);
            }
        }
    }
    return epsilonClosure;
}

std::vector<int> EpsilonClosure(LexerContext& lexerContext, int stateId)
{
    std::vector<int> states(1, stateId);
    return EpsilonClosure(lexerContext, states);
}

std::vector<int> Move(LexerContext& lexerContext, const std::vector<int>& stateIds, char32_t c)
{
    std::vector<int> next;
    for (int stateId : stateIds)
    {
        NfaState* state = lexerContext.GetNfaState(stateId);
        std::vector<NfaState*> n = state->Next(c);
        for (NfaState* sn : n)
        {
            int s = sn->Id();
            if (std::find(next.cbegin(), next.cend(), s) == next.cend())
            {
                next.push_back(s);
            }
        }
    }
    return next;
}

Dfa Compile(LexerContext& lexerContext, Nfa& nfa)
{
    Dfa dfa;
    std::vector<int> start = EpsilonClosure(lexerContext, nfa.Start()->Id());
    DfaState* s = lexerContext.MakeDfaState(start);
    s->Mark();
    std::vector<DfaState*> stack;
    stack.push_back(s);
    while (!stack.empty())
    {
        DfaState* state = stack.back();
        stack.pop_back();
        dfa.AddState(state);
        for (Class* cls : lexerContext.Partition())
        {
            if (!cls->Chars().empty())
            {
                char32_t c = cls->Chars().front();
                std::vector<int> next = EpsilonClosure(lexerContext, Move(lexerContext, state->NfaStateIds(), c));
                if (next.empty())
                {
                    state->AddNext(nullptr);
                }
                else
                {
                    DfaState* n = lexerContext.MakeDfaState(next);
                    state->AddNext(n);
                    if (!n->IsMarked())
                    {
                        n->Mark();
                        stack.push_back(n);
                    }
                }
            }
        }
    }
    dfa.Finalize(lexerContext);
    return dfa;
}

ExprParser::~ExprParser()
{
}

LexerContext::LexerContext() :
    nextNfaStateId(0), nextDfaStateId(0), ruleIndex(-1), classIndex(0), any(), epsilon(eps), 
    asciiIdStart(new Class(classIndex)), asciiIdCont(new Class(classIndex + 1)), unicodeIdStart(new Class(classIndex + 2)), unicodeIdCont(new Class(classIndex + 3)),
    tokens(nullptr), keywords(nullptr), expressions(nullptr), lexer(nullptr), currentExpression(nullptr), exprParser(nullptr), masterNfaIndex(-1)
{
    classIndex += 4;
    symbols.push_back(asciiIdStart);
    symbols.push_back(asciiIdCont);
    symbols.push_back(unicodeIdStart);
    symbols.push_back(unicodeIdCont);

    asciiIdStart->SetName("{ascii_id_start}");
    asciiIdStart->AddSymbol(MakeRange('A', 'Z'));
    asciiIdStart->AddSymbol(MakeRange('a', 'z'));
    asciiIdStart->AddSymbol(MakeChar('_'));
    asciiIdCont->SetName("{ascii_id_cont}");
    asciiIdCont->AddSymbol(MakeRange('A', 'Z'));
    asciiIdCont->AddSymbol(MakeRange('a', 'z'));
    asciiIdCont->AddSymbol(MakeRange('0', '9'));
    asciiIdCont->AddSymbol(MakeChar('_'));

    unicodeIdStart->SetName("{unicode_id_start}");
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65), static_cast<char32_t>(90)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(95), static_cast<char32_t>(95)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(97), static_cast<char32_t>(122)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(170), static_cast<char32_t>(170)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(181), static_cast<char32_t>(181)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(186), static_cast<char32_t>(186)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(192), static_cast<char32_t>(214)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(216), static_cast<char32_t>(246)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(248), static_cast<char32_t>(705)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(710), static_cast<char32_t>(721)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(736), static_cast<char32_t>(740)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(748), static_cast<char32_t>(748)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(750), static_cast<char32_t>(750)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(880), static_cast<char32_t>(884)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(886), static_cast<char32_t>(887)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(890), static_cast<char32_t>(893)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(895), static_cast<char32_t>(895)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(902), static_cast<char32_t>(902)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(904), static_cast<char32_t>(906)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(908), static_cast<char32_t>(908)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(910), static_cast<char32_t>(929)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(931), static_cast<char32_t>(1013)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1015), static_cast<char32_t>(1153)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1162), static_cast<char32_t>(1327)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1329), static_cast<char32_t>(1366)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1369), static_cast<char32_t>(1369)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1376), static_cast<char32_t>(1416)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1488), static_cast<char32_t>(1514)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1519), static_cast<char32_t>(1522)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1568), static_cast<char32_t>(1610)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1646), static_cast<char32_t>(1647)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1649), static_cast<char32_t>(1747)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1749), static_cast<char32_t>(1749)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1765), static_cast<char32_t>(1766)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1774), static_cast<char32_t>(1775)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1786), static_cast<char32_t>(1788)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1791), static_cast<char32_t>(1791)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1808), static_cast<char32_t>(1808)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1810), static_cast<char32_t>(1839)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1869), static_cast<char32_t>(1957)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1969), static_cast<char32_t>(1969)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(1994), static_cast<char32_t>(2026)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2036), static_cast<char32_t>(2037)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2042), static_cast<char32_t>(2042)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2048), static_cast<char32_t>(2069)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2074), static_cast<char32_t>(2074)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2084), static_cast<char32_t>(2084)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2088), static_cast<char32_t>(2088)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2112), static_cast<char32_t>(2136)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2144), static_cast<char32_t>(2154)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2208), static_cast<char32_t>(2228)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2230), static_cast<char32_t>(2237)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2308), static_cast<char32_t>(2361)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2365), static_cast<char32_t>(2365)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2384), static_cast<char32_t>(2384)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2392), static_cast<char32_t>(2401)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2417), static_cast<char32_t>(2432)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2437), static_cast<char32_t>(2444)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2447), static_cast<char32_t>(2448)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2451), static_cast<char32_t>(2472)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2474), static_cast<char32_t>(2480)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2482), static_cast<char32_t>(2482)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2486), static_cast<char32_t>(2489)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2493), static_cast<char32_t>(2493)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2510), static_cast<char32_t>(2510)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2524), static_cast<char32_t>(2525)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2527), static_cast<char32_t>(2529)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2544), static_cast<char32_t>(2545)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2556), static_cast<char32_t>(2556)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2565), static_cast<char32_t>(2570)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2575), static_cast<char32_t>(2576)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2579), static_cast<char32_t>(2600)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2602), static_cast<char32_t>(2608)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2610), static_cast<char32_t>(2611)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2613), static_cast<char32_t>(2614)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2616), static_cast<char32_t>(2617)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2649), static_cast<char32_t>(2652)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2654), static_cast<char32_t>(2654)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2674), static_cast<char32_t>(2676)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2693), static_cast<char32_t>(2701)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2703), static_cast<char32_t>(2705)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2707), static_cast<char32_t>(2728)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2730), static_cast<char32_t>(2736)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2738), static_cast<char32_t>(2739)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2741), static_cast<char32_t>(2745)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2749), static_cast<char32_t>(2749)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2768), static_cast<char32_t>(2768)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2784), static_cast<char32_t>(2785)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2809), static_cast<char32_t>(2809)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2821), static_cast<char32_t>(2828)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2831), static_cast<char32_t>(2832)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2835), static_cast<char32_t>(2856)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2858), static_cast<char32_t>(2864)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2866), static_cast<char32_t>(2867)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2869), static_cast<char32_t>(2873)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2877), static_cast<char32_t>(2877)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2908), static_cast<char32_t>(2909)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2911), static_cast<char32_t>(2913)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2929), static_cast<char32_t>(2929)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2947), static_cast<char32_t>(2947)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2949), static_cast<char32_t>(2954)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2958), static_cast<char32_t>(2960)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2962), static_cast<char32_t>(2965)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2969), static_cast<char32_t>(2970)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2972), static_cast<char32_t>(2972)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2974), static_cast<char32_t>(2975)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2979), static_cast<char32_t>(2980)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2984), static_cast<char32_t>(2986)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(2990), static_cast<char32_t>(3001)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3024), static_cast<char32_t>(3024)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3077), static_cast<char32_t>(3084)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3086), static_cast<char32_t>(3088)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3090), static_cast<char32_t>(3112)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3114), static_cast<char32_t>(3129)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3133), static_cast<char32_t>(3133)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3160), static_cast<char32_t>(3162)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3168), static_cast<char32_t>(3169)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3200), static_cast<char32_t>(3200)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3205), static_cast<char32_t>(3212)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3214), static_cast<char32_t>(3216)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3218), static_cast<char32_t>(3240)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3242), static_cast<char32_t>(3251)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3253), static_cast<char32_t>(3257)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3261), static_cast<char32_t>(3261)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3294), static_cast<char32_t>(3294)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3296), static_cast<char32_t>(3297)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3313), static_cast<char32_t>(3314)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3333), static_cast<char32_t>(3340)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3342), static_cast<char32_t>(3344)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3346), static_cast<char32_t>(3386)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3389), static_cast<char32_t>(3389)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3406), static_cast<char32_t>(3406)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3412), static_cast<char32_t>(3414)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3423), static_cast<char32_t>(3425)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3450), static_cast<char32_t>(3455)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3461), static_cast<char32_t>(3478)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3482), static_cast<char32_t>(3505)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3507), static_cast<char32_t>(3515)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3517), static_cast<char32_t>(3517)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3520), static_cast<char32_t>(3526)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3585), static_cast<char32_t>(3632)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3634), static_cast<char32_t>(3635)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3648), static_cast<char32_t>(3654)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3713), static_cast<char32_t>(3714)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3716), static_cast<char32_t>(3716)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3718), static_cast<char32_t>(3722)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3724), static_cast<char32_t>(3747)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3749), static_cast<char32_t>(3749)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3751), static_cast<char32_t>(3760)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3762), static_cast<char32_t>(3763)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3773), static_cast<char32_t>(3773)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3776), static_cast<char32_t>(3780)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3782), static_cast<char32_t>(3782)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3804), static_cast<char32_t>(3807)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3840), static_cast<char32_t>(3840)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3904), static_cast<char32_t>(3911)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3913), static_cast<char32_t>(3948)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(3976), static_cast<char32_t>(3980)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4096), static_cast<char32_t>(4138)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4159), static_cast<char32_t>(4159)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4176), static_cast<char32_t>(4181)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4186), static_cast<char32_t>(4189)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4193), static_cast<char32_t>(4193)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4197), static_cast<char32_t>(4198)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4206), static_cast<char32_t>(4208)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4213), static_cast<char32_t>(4225)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4238), static_cast<char32_t>(4238)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4256), static_cast<char32_t>(4293)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4295), static_cast<char32_t>(4295)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4301), static_cast<char32_t>(4301)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4304), static_cast<char32_t>(4346)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4348), static_cast<char32_t>(4680)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4682), static_cast<char32_t>(4685)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4688), static_cast<char32_t>(4694)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4696), static_cast<char32_t>(4696)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4698), static_cast<char32_t>(4701)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4704), static_cast<char32_t>(4744)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4746), static_cast<char32_t>(4749)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4752), static_cast<char32_t>(4784)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4786), static_cast<char32_t>(4789)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4792), static_cast<char32_t>(4798)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4800), static_cast<char32_t>(4800)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4802), static_cast<char32_t>(4805)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4808), static_cast<char32_t>(4822)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4824), static_cast<char32_t>(4880)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4882), static_cast<char32_t>(4885)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4888), static_cast<char32_t>(4954)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(4992), static_cast<char32_t>(5007)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5024), static_cast<char32_t>(5109)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5112), static_cast<char32_t>(5117)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5121), static_cast<char32_t>(5740)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5743), static_cast<char32_t>(5759)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5761), static_cast<char32_t>(5786)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5792), static_cast<char32_t>(5866)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5870), static_cast<char32_t>(5880)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5888), static_cast<char32_t>(5900)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5902), static_cast<char32_t>(5905)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5920), static_cast<char32_t>(5937)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5952), static_cast<char32_t>(5969)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5984), static_cast<char32_t>(5996)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(5998), static_cast<char32_t>(6000)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6016), static_cast<char32_t>(6067)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6103), static_cast<char32_t>(6103)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6108), static_cast<char32_t>(6108)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6176), static_cast<char32_t>(6264)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6272), static_cast<char32_t>(6312)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6314), static_cast<char32_t>(6314)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6320), static_cast<char32_t>(6389)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6400), static_cast<char32_t>(6430)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6480), static_cast<char32_t>(6509)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6512), static_cast<char32_t>(6516)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6528), static_cast<char32_t>(6571)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6576), static_cast<char32_t>(6601)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6656), static_cast<char32_t>(6678)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6688), static_cast<char32_t>(6740)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6823), static_cast<char32_t>(6823)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6917), static_cast<char32_t>(6963)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(6981), static_cast<char32_t>(6987)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7043), static_cast<char32_t>(7072)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7086), static_cast<char32_t>(7087)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7098), static_cast<char32_t>(7141)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7168), static_cast<char32_t>(7203)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7245), static_cast<char32_t>(7247)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7258), static_cast<char32_t>(7293)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7296), static_cast<char32_t>(7304)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7312), static_cast<char32_t>(7354)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7357), static_cast<char32_t>(7359)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7401), static_cast<char32_t>(7404)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7406), static_cast<char32_t>(7411)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7413), static_cast<char32_t>(7414)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7418), static_cast<char32_t>(7418)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7424), static_cast<char32_t>(7615)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7680), static_cast<char32_t>(7957)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7960), static_cast<char32_t>(7965)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(7968), static_cast<char32_t>(8005)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8008), static_cast<char32_t>(8013)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8016), static_cast<char32_t>(8023)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8025), static_cast<char32_t>(8025)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8027), static_cast<char32_t>(8027)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8029), static_cast<char32_t>(8029)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8031), static_cast<char32_t>(8061)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8064), static_cast<char32_t>(8116)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8118), static_cast<char32_t>(8124)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8126), static_cast<char32_t>(8126)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8130), static_cast<char32_t>(8132)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8134), static_cast<char32_t>(8140)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8144), static_cast<char32_t>(8147)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8150), static_cast<char32_t>(8155)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8160), static_cast<char32_t>(8172)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8178), static_cast<char32_t>(8180)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8182), static_cast<char32_t>(8188)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8305), static_cast<char32_t>(8305)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8319), static_cast<char32_t>(8319)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8336), static_cast<char32_t>(8348)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8450), static_cast<char32_t>(8450)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8455), static_cast<char32_t>(8455)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8458), static_cast<char32_t>(8467)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8469), static_cast<char32_t>(8469)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8472), static_cast<char32_t>(8477)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8484), static_cast<char32_t>(8484)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8486), static_cast<char32_t>(8486)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8488), static_cast<char32_t>(8488)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8490), static_cast<char32_t>(8505)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8508), static_cast<char32_t>(8511)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8517), static_cast<char32_t>(8521)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8526), static_cast<char32_t>(8526)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(8544), static_cast<char32_t>(8584)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11264), static_cast<char32_t>(11310)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11312), static_cast<char32_t>(11358)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11360), static_cast<char32_t>(11492)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11499), static_cast<char32_t>(11502)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11506), static_cast<char32_t>(11507)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11520), static_cast<char32_t>(11557)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11559), static_cast<char32_t>(11559)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11565), static_cast<char32_t>(11565)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11568), static_cast<char32_t>(11623)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11631), static_cast<char32_t>(11631)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11648), static_cast<char32_t>(11670)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11680), static_cast<char32_t>(11686)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11688), static_cast<char32_t>(11694)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11696), static_cast<char32_t>(11702)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11704), static_cast<char32_t>(11710)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11712), static_cast<char32_t>(11718)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11720), static_cast<char32_t>(11726)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11728), static_cast<char32_t>(11734)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(11736), static_cast<char32_t>(11742)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12293), static_cast<char32_t>(12295)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12321), static_cast<char32_t>(12329)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12337), static_cast<char32_t>(12341)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12344), static_cast<char32_t>(12348)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12353), static_cast<char32_t>(12438)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12443), static_cast<char32_t>(12447)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12449), static_cast<char32_t>(12538)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12540), static_cast<char32_t>(12543)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12549), static_cast<char32_t>(12591)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12593), static_cast<char32_t>(12686)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12704), static_cast<char32_t>(12730)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(12784), static_cast<char32_t>(12799)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(13312), static_cast<char32_t>(19893)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(19968), static_cast<char32_t>(40943)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(40960), static_cast<char32_t>(42124)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42192), static_cast<char32_t>(42237)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42240), static_cast<char32_t>(42508)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42512), static_cast<char32_t>(42527)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42538), static_cast<char32_t>(42539)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42560), static_cast<char32_t>(42606)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42623), static_cast<char32_t>(42653)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42656), static_cast<char32_t>(42735)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42775), static_cast<char32_t>(42783)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42786), static_cast<char32_t>(42888)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42891), static_cast<char32_t>(42943)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42946), static_cast<char32_t>(42950)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(42999), static_cast<char32_t>(43009)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43011), static_cast<char32_t>(43013)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43015), static_cast<char32_t>(43018)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43020), static_cast<char32_t>(43042)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43072), static_cast<char32_t>(43123)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43138), static_cast<char32_t>(43187)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43250), static_cast<char32_t>(43255)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43259), static_cast<char32_t>(43259)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43261), static_cast<char32_t>(43262)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43274), static_cast<char32_t>(43301)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43312), static_cast<char32_t>(43334)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43360), static_cast<char32_t>(43388)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43396), static_cast<char32_t>(43442)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43471), static_cast<char32_t>(43471)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43488), static_cast<char32_t>(43492)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43494), static_cast<char32_t>(43503)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43514), static_cast<char32_t>(43518)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43520), static_cast<char32_t>(43560)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43584), static_cast<char32_t>(43586)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43588), static_cast<char32_t>(43595)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43616), static_cast<char32_t>(43638)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43642), static_cast<char32_t>(43642)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43646), static_cast<char32_t>(43695)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43697), static_cast<char32_t>(43697)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43701), static_cast<char32_t>(43702)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43705), static_cast<char32_t>(43709)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43712), static_cast<char32_t>(43712)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43714), static_cast<char32_t>(43714)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43739), static_cast<char32_t>(43741)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43744), static_cast<char32_t>(43754)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43762), static_cast<char32_t>(43764)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43777), static_cast<char32_t>(43782)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43785), static_cast<char32_t>(43790)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43793), static_cast<char32_t>(43798)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43808), static_cast<char32_t>(43814)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43816), static_cast<char32_t>(43822)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43824), static_cast<char32_t>(43866)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43868), static_cast<char32_t>(43879)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(43888), static_cast<char32_t>(44002)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(44032), static_cast<char32_t>(55203)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(55216), static_cast<char32_t>(55238)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(55243), static_cast<char32_t>(55291)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(63744), static_cast<char32_t>(64109)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64112), static_cast<char32_t>(64217)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64256), static_cast<char32_t>(64262)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64275), static_cast<char32_t>(64279)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64285), static_cast<char32_t>(64285)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64287), static_cast<char32_t>(64296)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64298), static_cast<char32_t>(64310)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64312), static_cast<char32_t>(64316)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64318), static_cast<char32_t>(64318)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64320), static_cast<char32_t>(64321)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64323), static_cast<char32_t>(64324)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64326), static_cast<char32_t>(64433)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64467), static_cast<char32_t>(64829)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64848), static_cast<char32_t>(64911)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(64914), static_cast<char32_t>(64967)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65008), static_cast<char32_t>(65019)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65136), static_cast<char32_t>(65140)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65142), static_cast<char32_t>(65276)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65313), static_cast<char32_t>(65338)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65345), static_cast<char32_t>(65370)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65382), static_cast<char32_t>(65470)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65474), static_cast<char32_t>(65479)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65482), static_cast<char32_t>(65487)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65490), static_cast<char32_t>(65495)));
    unicodeIdStart->AddSymbol(MakeRange(static_cast<char32_t>(65498), static_cast<char32_t>(65500)));

    unicodeIdCont->SetName("{unicode_id_cont}");
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(48), static_cast<char32_t>(57)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65), static_cast<char32_t>(90)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(95), static_cast<char32_t>(95)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(97), static_cast<char32_t>(122)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(170), static_cast<char32_t>(170)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(181), static_cast<char32_t>(181)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(183), static_cast<char32_t>(183)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(186), static_cast<char32_t>(186)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(192), static_cast<char32_t>(214)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(216), static_cast<char32_t>(246)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(248), static_cast<char32_t>(705)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(710), static_cast<char32_t>(721)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(736), static_cast<char32_t>(740)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(748), static_cast<char32_t>(748)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(750), static_cast<char32_t>(750)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(768), static_cast<char32_t>(884)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(886), static_cast<char32_t>(887)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(890), static_cast<char32_t>(893)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(895), static_cast<char32_t>(895)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(902), static_cast<char32_t>(906)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(908), static_cast<char32_t>(908)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(910), static_cast<char32_t>(929)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(931), static_cast<char32_t>(1013)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1015), static_cast<char32_t>(1153)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1155), static_cast<char32_t>(1159)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1162), static_cast<char32_t>(1327)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1329), static_cast<char32_t>(1366)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1369), static_cast<char32_t>(1369)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1376), static_cast<char32_t>(1416)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1425), static_cast<char32_t>(1469)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1471), static_cast<char32_t>(1471)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1473), static_cast<char32_t>(1474)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1476), static_cast<char32_t>(1477)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1479), static_cast<char32_t>(1479)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1488), static_cast<char32_t>(1514)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1519), static_cast<char32_t>(1522)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1552), static_cast<char32_t>(1562)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1568), static_cast<char32_t>(1641)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1646), static_cast<char32_t>(1747)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1749), static_cast<char32_t>(1756)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1759), static_cast<char32_t>(1768)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1770), static_cast<char32_t>(1788)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1791), static_cast<char32_t>(1791)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1808), static_cast<char32_t>(1866)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1869), static_cast<char32_t>(1969)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(1984), static_cast<char32_t>(2037)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2042), static_cast<char32_t>(2042)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2045), static_cast<char32_t>(2045)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2048), static_cast<char32_t>(2093)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2112), static_cast<char32_t>(2139)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2144), static_cast<char32_t>(2154)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2208), static_cast<char32_t>(2228)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2230), static_cast<char32_t>(2237)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2259), static_cast<char32_t>(2273)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2275), static_cast<char32_t>(2403)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2406), static_cast<char32_t>(2415)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2417), static_cast<char32_t>(2435)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2437), static_cast<char32_t>(2444)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2447), static_cast<char32_t>(2448)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2451), static_cast<char32_t>(2472)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2474), static_cast<char32_t>(2480)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2482), static_cast<char32_t>(2482)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2486), static_cast<char32_t>(2489)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2492), static_cast<char32_t>(2500)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2503), static_cast<char32_t>(2504)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2507), static_cast<char32_t>(2510)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2519), static_cast<char32_t>(2519)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2524), static_cast<char32_t>(2525)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2527), static_cast<char32_t>(2531)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2534), static_cast<char32_t>(2545)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2556), static_cast<char32_t>(2556)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2558), static_cast<char32_t>(2558)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2561), static_cast<char32_t>(2563)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2565), static_cast<char32_t>(2570)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2575), static_cast<char32_t>(2576)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2579), static_cast<char32_t>(2600)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2602), static_cast<char32_t>(2608)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2610), static_cast<char32_t>(2611)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2613), static_cast<char32_t>(2614)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2616), static_cast<char32_t>(2617)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2620), static_cast<char32_t>(2620)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2622), static_cast<char32_t>(2626)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2631), static_cast<char32_t>(2632)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2635), static_cast<char32_t>(2637)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2641), static_cast<char32_t>(2641)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2649), static_cast<char32_t>(2652)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2654), static_cast<char32_t>(2654)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2662), static_cast<char32_t>(2677)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2689), static_cast<char32_t>(2691)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2693), static_cast<char32_t>(2701)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2703), static_cast<char32_t>(2705)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2707), static_cast<char32_t>(2728)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2730), static_cast<char32_t>(2736)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2738), static_cast<char32_t>(2739)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2741), static_cast<char32_t>(2745)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2748), static_cast<char32_t>(2757)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2759), static_cast<char32_t>(2761)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2763), static_cast<char32_t>(2765)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2768), static_cast<char32_t>(2768)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2784), static_cast<char32_t>(2787)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2790), static_cast<char32_t>(2799)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2809), static_cast<char32_t>(2815)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2817), static_cast<char32_t>(2819)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2821), static_cast<char32_t>(2828)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2831), static_cast<char32_t>(2832)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2835), static_cast<char32_t>(2856)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2858), static_cast<char32_t>(2864)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2866), static_cast<char32_t>(2867)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2869), static_cast<char32_t>(2873)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2876), static_cast<char32_t>(2884)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2887), static_cast<char32_t>(2888)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2891), static_cast<char32_t>(2893)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2902), static_cast<char32_t>(2903)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2908), static_cast<char32_t>(2909)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2911), static_cast<char32_t>(2915)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2918), static_cast<char32_t>(2927)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2929), static_cast<char32_t>(2929)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2946), static_cast<char32_t>(2947)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2949), static_cast<char32_t>(2954)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2958), static_cast<char32_t>(2960)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2962), static_cast<char32_t>(2965)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2969), static_cast<char32_t>(2970)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2972), static_cast<char32_t>(2972)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2974), static_cast<char32_t>(2975)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2979), static_cast<char32_t>(2980)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2984), static_cast<char32_t>(2986)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(2990), static_cast<char32_t>(3001)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3006), static_cast<char32_t>(3010)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3014), static_cast<char32_t>(3016)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3018), static_cast<char32_t>(3021)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3024), static_cast<char32_t>(3024)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3031), static_cast<char32_t>(3031)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3046), static_cast<char32_t>(3055)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3072), static_cast<char32_t>(3084)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3086), static_cast<char32_t>(3088)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3090), static_cast<char32_t>(3112)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3114), static_cast<char32_t>(3129)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3133), static_cast<char32_t>(3140)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3142), static_cast<char32_t>(3144)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3146), static_cast<char32_t>(3149)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3157), static_cast<char32_t>(3158)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3160), static_cast<char32_t>(3162)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3168), static_cast<char32_t>(3171)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3174), static_cast<char32_t>(3183)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3200), static_cast<char32_t>(3203)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3205), static_cast<char32_t>(3212)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3214), static_cast<char32_t>(3216)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3218), static_cast<char32_t>(3240)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3242), static_cast<char32_t>(3251)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3253), static_cast<char32_t>(3257)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3260), static_cast<char32_t>(3268)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3270), static_cast<char32_t>(3272)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3274), static_cast<char32_t>(3277)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3285), static_cast<char32_t>(3286)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3294), static_cast<char32_t>(3294)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3296), static_cast<char32_t>(3299)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3302), static_cast<char32_t>(3311)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3313), static_cast<char32_t>(3314)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3328), static_cast<char32_t>(3331)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3333), static_cast<char32_t>(3340)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3342), static_cast<char32_t>(3344)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3346), static_cast<char32_t>(3396)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3398), static_cast<char32_t>(3400)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3402), static_cast<char32_t>(3406)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3412), static_cast<char32_t>(3415)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3423), static_cast<char32_t>(3427)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3430), static_cast<char32_t>(3439)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3450), static_cast<char32_t>(3455)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3458), static_cast<char32_t>(3459)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3461), static_cast<char32_t>(3478)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3482), static_cast<char32_t>(3505)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3507), static_cast<char32_t>(3515)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3517), static_cast<char32_t>(3517)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3520), static_cast<char32_t>(3526)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3530), static_cast<char32_t>(3530)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3535), static_cast<char32_t>(3540)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3542), static_cast<char32_t>(3542)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3544), static_cast<char32_t>(3551)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3558), static_cast<char32_t>(3567)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3570), static_cast<char32_t>(3571)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3585), static_cast<char32_t>(3642)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3648), static_cast<char32_t>(3662)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3664), static_cast<char32_t>(3673)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3713), static_cast<char32_t>(3714)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3716), static_cast<char32_t>(3716)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3718), static_cast<char32_t>(3722)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3724), static_cast<char32_t>(3747)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3749), static_cast<char32_t>(3749)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3751), static_cast<char32_t>(3773)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3776), static_cast<char32_t>(3780)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3782), static_cast<char32_t>(3782)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3784), static_cast<char32_t>(3789)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3792), static_cast<char32_t>(3801)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3804), static_cast<char32_t>(3807)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3840), static_cast<char32_t>(3840)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3864), static_cast<char32_t>(3865)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3872), static_cast<char32_t>(3881)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3893), static_cast<char32_t>(3893)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3895), static_cast<char32_t>(3895)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3897), static_cast<char32_t>(3897)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3902), static_cast<char32_t>(3911)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3913), static_cast<char32_t>(3948)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3953), static_cast<char32_t>(3972)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3974), static_cast<char32_t>(3991)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(3993), static_cast<char32_t>(4028)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4038), static_cast<char32_t>(4038)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4096), static_cast<char32_t>(4169)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4176), static_cast<char32_t>(4253)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4256), static_cast<char32_t>(4293)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4295), static_cast<char32_t>(4295)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4301), static_cast<char32_t>(4301)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4304), static_cast<char32_t>(4346)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4348), static_cast<char32_t>(4680)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4682), static_cast<char32_t>(4685)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4688), static_cast<char32_t>(4694)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4696), static_cast<char32_t>(4696)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4698), static_cast<char32_t>(4701)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4704), static_cast<char32_t>(4744)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4746), static_cast<char32_t>(4749)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4752), static_cast<char32_t>(4784)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4786), static_cast<char32_t>(4789)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4792), static_cast<char32_t>(4798)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4800), static_cast<char32_t>(4800)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4802), static_cast<char32_t>(4805)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4808), static_cast<char32_t>(4822)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4824), static_cast<char32_t>(4880)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4882), static_cast<char32_t>(4885)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4888), static_cast<char32_t>(4954)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4957), static_cast<char32_t>(4959)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4969), static_cast<char32_t>(4977)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(4992), static_cast<char32_t>(5007)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5024), static_cast<char32_t>(5109)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5112), static_cast<char32_t>(5117)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5121), static_cast<char32_t>(5740)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5743), static_cast<char32_t>(5759)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5761), static_cast<char32_t>(5786)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5792), static_cast<char32_t>(5866)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5870), static_cast<char32_t>(5880)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5888), static_cast<char32_t>(5900)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5902), static_cast<char32_t>(5908)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5920), static_cast<char32_t>(5940)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5952), static_cast<char32_t>(5971)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5984), static_cast<char32_t>(5996)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(5998), static_cast<char32_t>(6000)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6002), static_cast<char32_t>(6003)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6016), static_cast<char32_t>(6099)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6103), static_cast<char32_t>(6103)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6108), static_cast<char32_t>(6109)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6112), static_cast<char32_t>(6121)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6155), static_cast<char32_t>(6157)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6160), static_cast<char32_t>(6169)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6176), static_cast<char32_t>(6264)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6272), static_cast<char32_t>(6314)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6320), static_cast<char32_t>(6389)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6400), static_cast<char32_t>(6430)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6432), static_cast<char32_t>(6443)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6448), static_cast<char32_t>(6459)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6470), static_cast<char32_t>(6509)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6512), static_cast<char32_t>(6516)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6528), static_cast<char32_t>(6571)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6576), static_cast<char32_t>(6601)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6608), static_cast<char32_t>(6618)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6656), static_cast<char32_t>(6683)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6688), static_cast<char32_t>(6750)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6752), static_cast<char32_t>(6780)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6783), static_cast<char32_t>(6793)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6800), static_cast<char32_t>(6809)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6823), static_cast<char32_t>(6823)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6832), static_cast<char32_t>(6845)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6912), static_cast<char32_t>(6987)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(6992), static_cast<char32_t>(7001)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7019), static_cast<char32_t>(7027)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7040), static_cast<char32_t>(7155)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7168), static_cast<char32_t>(7223)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7232), static_cast<char32_t>(7241)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7245), static_cast<char32_t>(7293)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7296), static_cast<char32_t>(7304)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7312), static_cast<char32_t>(7354)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7357), static_cast<char32_t>(7359)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7376), static_cast<char32_t>(7378)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7380), static_cast<char32_t>(7418)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7424), static_cast<char32_t>(7673)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7675), static_cast<char32_t>(7957)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7960), static_cast<char32_t>(7965)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(7968), static_cast<char32_t>(8005)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8008), static_cast<char32_t>(8013)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8016), static_cast<char32_t>(8023)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8025), static_cast<char32_t>(8025)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8027), static_cast<char32_t>(8027)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8029), static_cast<char32_t>(8029)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8031), static_cast<char32_t>(8061)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8064), static_cast<char32_t>(8116)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8118), static_cast<char32_t>(8124)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8126), static_cast<char32_t>(8126)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8130), static_cast<char32_t>(8132)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8134), static_cast<char32_t>(8140)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8144), static_cast<char32_t>(8147)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8150), static_cast<char32_t>(8155)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8160), static_cast<char32_t>(8172)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8178), static_cast<char32_t>(8180)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8182), static_cast<char32_t>(8188)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8255), static_cast<char32_t>(8256)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8276), static_cast<char32_t>(8276)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8305), static_cast<char32_t>(8305)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8319), static_cast<char32_t>(8319)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8336), static_cast<char32_t>(8348)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8400), static_cast<char32_t>(8412)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8417), static_cast<char32_t>(8417)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8421), static_cast<char32_t>(8432)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8450), static_cast<char32_t>(8450)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8455), static_cast<char32_t>(8455)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8458), static_cast<char32_t>(8467)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8469), static_cast<char32_t>(8469)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8472), static_cast<char32_t>(8477)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8484), static_cast<char32_t>(8484)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8486), static_cast<char32_t>(8486)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8488), static_cast<char32_t>(8488)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8490), static_cast<char32_t>(8505)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8508), static_cast<char32_t>(8511)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8517), static_cast<char32_t>(8521)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8526), static_cast<char32_t>(8526)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(8544), static_cast<char32_t>(8584)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11264), static_cast<char32_t>(11310)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11312), static_cast<char32_t>(11358)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11360), static_cast<char32_t>(11492)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11499), static_cast<char32_t>(11507)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11520), static_cast<char32_t>(11557)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11559), static_cast<char32_t>(11559)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11565), static_cast<char32_t>(11565)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11568), static_cast<char32_t>(11623)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11631), static_cast<char32_t>(11631)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11647), static_cast<char32_t>(11670)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11680), static_cast<char32_t>(11686)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11688), static_cast<char32_t>(11694)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11696), static_cast<char32_t>(11702)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11704), static_cast<char32_t>(11710)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11712), static_cast<char32_t>(11718)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11720), static_cast<char32_t>(11726)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11728), static_cast<char32_t>(11734)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11736), static_cast<char32_t>(11742)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(11744), static_cast<char32_t>(11775)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12293), static_cast<char32_t>(12295)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12321), static_cast<char32_t>(12335)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12337), static_cast<char32_t>(12341)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12344), static_cast<char32_t>(12348)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12353), static_cast<char32_t>(12438)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12441), static_cast<char32_t>(12447)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12449), static_cast<char32_t>(12538)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12540), static_cast<char32_t>(12543)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12549), static_cast<char32_t>(12591)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12593), static_cast<char32_t>(12686)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12704), static_cast<char32_t>(12730)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(12784), static_cast<char32_t>(12799)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(13312), static_cast<char32_t>(19893)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(19968), static_cast<char32_t>(40943)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(40960), static_cast<char32_t>(42124)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42192), static_cast<char32_t>(42237)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42240), static_cast<char32_t>(42508)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42512), static_cast<char32_t>(42539)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42560), static_cast<char32_t>(42607)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42612), static_cast<char32_t>(42621)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42623), static_cast<char32_t>(42737)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42775), static_cast<char32_t>(42783)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42786), static_cast<char32_t>(42888)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42891), static_cast<char32_t>(42943)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42946), static_cast<char32_t>(42950)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(42999), static_cast<char32_t>(43047)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43072), static_cast<char32_t>(43123)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43136), static_cast<char32_t>(43205)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43216), static_cast<char32_t>(43225)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43232), static_cast<char32_t>(43255)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43259), static_cast<char32_t>(43259)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43261), static_cast<char32_t>(43309)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43312), static_cast<char32_t>(43347)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43360), static_cast<char32_t>(43388)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43392), static_cast<char32_t>(43456)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43471), static_cast<char32_t>(43481)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43488), static_cast<char32_t>(43518)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43520), static_cast<char32_t>(43574)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43584), static_cast<char32_t>(43597)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43600), static_cast<char32_t>(43609)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43616), static_cast<char32_t>(43638)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43642), static_cast<char32_t>(43714)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43739), static_cast<char32_t>(43741)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43744), static_cast<char32_t>(43759)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43762), static_cast<char32_t>(43766)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43777), static_cast<char32_t>(43782)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43785), static_cast<char32_t>(43790)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43793), static_cast<char32_t>(43798)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43808), static_cast<char32_t>(43814)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43816), static_cast<char32_t>(43822)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43824), static_cast<char32_t>(43866)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43868), static_cast<char32_t>(43879)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(43888), static_cast<char32_t>(44010)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(44012), static_cast<char32_t>(44013)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(44016), static_cast<char32_t>(44025)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(44032), static_cast<char32_t>(55203)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(55216), static_cast<char32_t>(55238)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(55243), static_cast<char32_t>(55291)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(63744), static_cast<char32_t>(64109)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64112), static_cast<char32_t>(64217)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64256), static_cast<char32_t>(64262)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64275), static_cast<char32_t>(64279)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64285), static_cast<char32_t>(64296)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64298), static_cast<char32_t>(64310)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64312), static_cast<char32_t>(64316)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64318), static_cast<char32_t>(64318)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64320), static_cast<char32_t>(64321)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64323), static_cast<char32_t>(64324)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64326), static_cast<char32_t>(64433)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64467), static_cast<char32_t>(64829)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64848), static_cast<char32_t>(64911)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(64914), static_cast<char32_t>(64967)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65008), static_cast<char32_t>(65019)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65024), static_cast<char32_t>(65039)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65056), static_cast<char32_t>(65071)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65075), static_cast<char32_t>(65076)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65101), static_cast<char32_t>(65103)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65136), static_cast<char32_t>(65140)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65142), static_cast<char32_t>(65276)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65296), static_cast<char32_t>(65305)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65313), static_cast<char32_t>(65338)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65343), static_cast<char32_t>(65343)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65345), static_cast<char32_t>(65370)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65382), static_cast<char32_t>(65470)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65474), static_cast<char32_t>(65479)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65482), static_cast<char32_t>(65487)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65490), static_cast<char32_t>(65495)));
    unicodeIdCont->AddSymbol(MakeRange(static_cast<char32_t>(65498), static_cast<char32_t>(65500)));
}

LexerContext::~LexerContext()
{
    for (NfaState* state : nfaStates)
    {
        delete state;
    }
    for (DfaState* state : dfaStates)
    {
        delete state;
    }
    for (Symbol* symbol : symbols)
    {
        delete symbol;
    }
    for (Class* cls : canonicalClasses)
    {
        delete cls;
    }
}

void LexerContext::SetFileName(const std::string& fileName_)
{
    fileName = fileName_;
}

NfaState* LexerContext::MakeNfaState()
{
    NfaState* state = new NfaState(nextNfaStateId++, ruleIndex);
    nfaStates.push_back(state);
    nfaStateMap[state->Id()] = state;
    return state;
}

NfaState* LexerContext::GetNfaState(int id) const
{
    auto it = nfaStateMap.find(id);
    if (it != nfaStateMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("NFA state " + std::to_string(id) + " not found");
    }
}

DfaState* LexerContext::MakeDfaState(const std::vector<int>& nfaStates)
{
    std::vector<int> s(nfaStates);
    std::sort(s.begin(), s.end());
    for (DfaState* state : dfaStates)
    {
        if (state->NfaStateIds() == s)
        {
            return state;
        }
    }
    DfaState* state = new DfaState(nextDfaStateId++, s);
    dfaStates.push_back(state);
    return state;
}

Symbol* LexerContext::MakeChar(char32_t c)
{
    auto it = charSymbols.find(c);
    if (it != charSymbols.cend())
    {
        return it->second;
    }
    Symbol* symbol = new CharSymbol(c);
    symbol->DontSetContained();
    symbols.push_back(symbol);
    charSymbols[c] = symbol;
    return symbol;
}

Symbol* LexerContext::MakeRange(char32_t start, char32_t end)
{
    Range range(start, end);
    auto it = rangeSymbols.find(range);
    if (it != rangeSymbols.cend())
    {
        return it->second;
    }
    Symbol* symbol = new Range(start, end);
    symbols.push_back(symbol);
    rangeSymbols[range] = symbol;
    return symbol;
}

Class* LexerContext::MakeClass()
{
    Class* cls = new Class(classIndex++);
    symbols.push_back(cls);
    return cls;
}

void LexerContext::AddToSymbols(Symbol* symbol)
{
    symbols.push_back(symbol);
}

void LexerContext::AddCanonicalClass(Class* cls)
{
    canonicalClasses.push_back(cls);
}

void LexerContext::AddToPartition(Class* cls)
{
    partition.push_back(cls);
}

Nfa LexerContext::MakeExpr(const std::string& id)
{
    if (id == "ascii_id_start")
    {
        return MakeNfa(*this, MakeAsciiIdStart());
    }
    else if (id == "ascii_id_cont")
    {
        return MakeNfa(*this, MakeAsciiIdCont());
    }
    else if (id == "unicode_id_start")
    {
        return MakeNfa(*this, MakeUnicodeIdStart());
    }
    else if (id == "unicode_id_cont")
    {
        return MakeNfa(*this, MakeUnicodeIdCont());
    }
    if (expressions)
    {
        soul::ast::slg::Expression* expr = expressions->GetExpression(id);
        if (expr)
        {
            if (currentExpression)
            {
                if (expr->Index() >= currentExpression->Index())
                {
                    throw std::runtime_error("error: expression '" + currentExpression->Id() + "' depends on forward reference '" + expr->Id() + "'");
                }
            }
            if (exprParser)
            {
                soul::ast::slg::Expression* prevExpression = currentExpression;
                currentExpression = expr;
                Nfa nfa = exprParser->Parse(expr->Value(), this, expr->FileName(), expr->Line());
                currentExpression = prevExpression;
                return nfa;
            }
            else
            {
                throw std::runtime_error("parser not set");
            }
        }
        else
        {
            throw std::runtime_error("error: expression '" + id + "' not found");
        }
    }
    else
    {
        throw std::runtime_error("expressions not set");
    }
}

int LexerContext::AddNfa(Nfa* nfa)
{
    int index = nfas.size();
    nfas.push_back(std::unique_ptr<Nfa>(nfa));
    nfaMap[index] = nfa;
    return index;
}

Nfa* LexerContext::GetNfa(int index) const
{
    auto it = nfaMap.find(index);
    if (it != nfaMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("NFA " + std::to_string(index) + " not found");
    }
}

void LexerContext::SetDfa(Dfa&& dfa_)
{
    dfa = std::move(dfa_);
}

} // namespace soul::ast::re
