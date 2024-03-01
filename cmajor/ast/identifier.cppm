// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.identifier;

import soul.lexer.token;
import cmajor.ast.node;

export namespace cmajor::ast {

using Token = soul::lexer::Token<char32_t, char32_t>;

class IdentifierNode : public Node
{
public:
    IdentifierNode(const soul::ast::Span& span_);
    IdentifierNode(const soul::ast::Span& span_, NodeType nodeType_);
    IdentifierNode(const soul::ast::Span& span_, const std::u32string& identifier_);
    IdentifierNode(const soul::ast::Span& span_, NodeType nodeType_, const std::u32string& identifier_);
    IdentifierNode(const soul::ast::Span& span_, const Token& token);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::u32string& Str() const { return identifier; }
    void SetStr(const std::u32string& str) { identifier = str; }
    std::string ToString() const override;
    bool IsInternal() const;
private:
    std::u32string identifier;
};

class CursorIdNode : public IdentifierNode
{
public:
    CursorIdNode(const soul::ast::Span& span_);
    CursorIdNode(const soul::ast::Span& span_, const std::u32string& identifier_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};
} // namespace cmajor::ast