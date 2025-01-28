// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.document.fragment;

namespace soul::xml {

DocumentFragment::DocumentFragment(const soul::ast::SourcePos& sourcePos_) : ParentNode(NodeKind::documentFragmentNode, sourcePos_, "document_fragment")
{
}

Node* DocumentFragment::Clone(bool deep) const
{
    DocumentFragment* clone = new DocumentFragment(GetSourcePos());
    if (deep)
    {
        Node* child = FirstChild();
        while (child != nullptr)
        {
            clone->AppendChild(child->Clone(deep));
            child = child->Next();
        }
    }
    return clone;
}

DocumentFragment* MakeDocumentFragment()
{
    return new DocumentFragment(soul::ast::SourcePos());
}

} // namespace soul::xml
