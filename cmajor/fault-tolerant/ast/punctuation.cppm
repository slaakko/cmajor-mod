// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.punctuation;

import cmajor.fault.tolerant.ast.node;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class DotNode : public TokenNode
{
public:
    DotNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    bool IsDot() const override { return true; }
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ArrowNode : public TokenNode
{
public:
    ArrowNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class CommaNode : public TokenNode
{
public:
    CommaNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    bool IsComma() const override { return true; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ColonNode : public TokenNode
{
public:
    ColonNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class SemicolonNode : public TokenNode
{
public:
    SemicolonNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LBraceNode : public TokenNode
{
public:
    LBraceNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class RBraceNode : public TokenNode
{
public:
    RBraceNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LBracketNode : public TokenNode
{
public:
    LBracketNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class RBracketNode : public TokenNode
{
public:
    RBracketNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LParenNode : public TokenNode
{
public:
    LParenNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class RParenNode : public TokenNode
{
public:
    RParenNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AssignNode : public TokenNode
{
public:
    AssignNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LAngleNode : public TokenNode
{
public:
    LAngleNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class RAngleNode : public TokenNode
{
public:
    RAngleNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class StarNode : public TokenNode
{
public:
    StarNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AmpNode : public TokenNode
{
public:
    AmpNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class AmpAmpNode : public TokenNode
{
public:
    AmpAmpNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class HashNode : public TokenNode
{
public:
    HashNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

} // namespace cmajor::fault::tolerant::ast
