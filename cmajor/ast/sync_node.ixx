export module cmajor.ast.sync.node;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

//#include <sngcm/ast/Node.hpp>
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