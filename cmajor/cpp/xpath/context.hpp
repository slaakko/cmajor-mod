// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef XPATH_CONTEXT_INCLUDED
#define XPATH_CONTEXT_INCLUDED
#include <dom/node.hpp>

namespace soul::xml::xpath {

class Context
{
public:
    Context(soul::xml::Node* node_, int pos_, int size_);
    soul::xml::Node* Node() const { return node; }
    int Pos() const { return pos; }
    int Size() const { return size; }
private:
    soul::xml::Node* node;
    int pos;
    int size;
};

} // namespace soul::xml::xpath

#endif // XPATH_CONTEXT_INCLUDED
