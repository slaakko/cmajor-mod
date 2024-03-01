// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.xpath.context;

namespace soul::xml::xpath {

Context::Context(soul::xml::Node* node_, int pos_, int size_) : node(node_), pos(pos_), size(size_)
{
}

} // namespace soul::xml::xpath
