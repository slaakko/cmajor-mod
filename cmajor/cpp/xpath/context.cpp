// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <xpath/context.hpp>

namespace soul::xml::xpath {

Context::Context(soul::xml::Node* node_, int pos_, int size_) : node(node_), pos(pos_), size(size_)
{
}

} // namespace soul::xml::xpath
