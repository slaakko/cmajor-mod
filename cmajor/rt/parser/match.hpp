// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef PARSER_MATCH_INCLUDED
#define PARSER_MATCH_INCLUDED

namespace soul::parser {

struct Match
{
    Match(bool hit_) : hit(hit_), value(nullptr) {}
    Match(bool hit_, void* value_) : hit(hit_), value(value_) {}
    bool hit;
    void* value;
};

} // namespace soul::parser;

#endif // PARSER_MATCH_INCLUDED
