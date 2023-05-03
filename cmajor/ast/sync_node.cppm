// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.sync.node;

import cmajor.ast.node;
import soul.ast.source.pos;

export namespace cmajor::ast {

class SyncNode : public Node
{
public:
    SyncNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
};

} // namespace cmajor::ast
