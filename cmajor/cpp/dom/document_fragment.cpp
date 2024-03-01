// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <dom/document_fragment.hpp>

namespace soul::xml {

DocumentFragment::DocumentFragment(const soul::ast::SourcePos& sourcePos_) : ParentNode(NodeKind::documentFragmentNode, sourcePos_, "document_fragment")
{
}

DocumentFragment* MakeDocumentFragment()
{
    return new DocumentFragment(soul::ast::SourcePos());
}

} // namespace soul::xml
