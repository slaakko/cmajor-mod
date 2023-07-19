// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_DOCUMENT_FRAGMENT
#define DOM_DOCUMENT_FRAGMENT
#include <dom/parent_node.hpp>

namespace soul::xml {

class DocumentFragment : public ParentNode
{
public:
    DocumentFragment(const soul::ast::SourcePos& sourcePos_);
};

DocumentFragment* MakeDocumentFragment();

} // namespace soul::xml

#endif // DOM_DOCUMENT_FRAGMENT
