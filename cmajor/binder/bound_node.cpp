// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.bound.node;

namespace cmajor::binder {

BoundNode::BoundNode(const soul::ast::Span& span_, BoundNodeType boundNodeType_) :
    span(span_), boundNodeType(boundNodeType_), parent(nullptr)
{
}

void BoundNode::SetSpan(const soul::ast::Span& span_)
{
    span = span_;
}

soul::ast::FullSpan BoundNode::GetFullSpan() const
{
    return soul::ast::FullSpan(ModuleId(), FileIndex(), span);
}

const util::uuid& BoundNode::ModuleId() const
{
    if (parent)
    {
        return parent->ModuleId();
    }
    else
    {
        static util::uuid emptyId;
        return emptyId;
    }
}

int32_t BoundNode::FileIndex() const
{
    if (parent)
    {
        return parent->FileIndex();
    }
    else
    {
        return -1;
    }
}

} // namespace cmajor::binder
