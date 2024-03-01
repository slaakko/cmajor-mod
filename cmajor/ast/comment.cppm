// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.comment;

import cmajor.ast.node;

export namespace cmajor::ast {

class CommentNode : public Node
{
public:
    CommentNode(const soul::ast::Span& span_);
    CommentNode(const soul::ast::Span& span_, const std::u32string& comment_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::u32string& Comment() const { return comment; }
private:
    std::u32string comment;
};
} // namespace cmajor::ast

