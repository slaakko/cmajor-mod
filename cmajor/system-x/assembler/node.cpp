// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.node;

namespace cmajor::systemx::assembler {

Node::Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_) : kind(kind_), sourcePos(sourcePos_), creatorThreadId(std::this_thread::get_id()), owner(nullptr)
{
}

Node::~Node()
{
}

void Node::Write(util::CodeFormatter& formatter)
{
}

void Node::SetOwner(void* owner_)
{
    if (owner)
    {
        throw std::runtime_error("already have owner set");
    }
    owner = owner_;
}

} // namespace cmajor::systemx::assembler
