// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.sync.node;

import cmajor.ast.node;
//import soul.ast.source.pos;
import soul.ast.span;

export namespace cmajor::ast {

class SyncNode : public Node
{
public:
    SyncNode(const soul::ast::Span& span_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

} // namespace cmajor::ast
