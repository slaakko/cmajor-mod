// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.literal;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.punctuation;
import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class CharLiteralPrefix
{
    none = 0,
    utf16Prefix = 1,
    utf32Prefix = 2
};

enum class StringLiteralPrefix
{
    none = 0,
    utf16Prefix = 1,
    utf32Prefix = 2
};

class LiteralTokenNode : public TokenNode
{
public:
    LiteralTokenNode(const soul::ast::Span& span_, CompletionContext completionContext_, const std::u16string& str_);
    const std::u16string& Str() const { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BooleanLiteralNode : public SyntaxNode
{
public:
    BooleanLiteralNode(KeywordNode* rep_, bool value_);
    KeywordNode* Rep() const { return rep.get(); }
    bool Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<KeywordNode> rep;
    bool value;
};

class SByteLiteralNode : public SyntaxNode
{
public:
    SByteLiteralNode(LiteralTokenNode* rep_, int8_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    int8_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    int8_t value;
};

class ByteLiteralNode : public SyntaxNode
{
public:
    ByteLiteralNode(LiteralTokenNode* rep_, uint8_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    uint8_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    uint8_t value;
};

class ShortLiteralNode : public SyntaxNode
{
public:
    ShortLiteralNode(LiteralTokenNode* rep_, int16_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    int16_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    int16_t value;
};

class UShortLiteralNode : public SyntaxNode
{
public:
    UShortLiteralNode(LiteralTokenNode* rep_, uint16_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    uint16_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    uint16_t value;
};

class IntLiteralNode : public SyntaxNode
{
public:
    IntLiteralNode(LiteralTokenNode* rep_, int32_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    int32_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    int32_t value;
};

class UIntLiteralNode : public SyntaxNode
{
public:
    UIntLiteralNode(LiteralTokenNode* rep_, uint32_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    uint32_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    uint32_t value;
};

class LongLiteralNode : public SyntaxNode
{
public:
    LongLiteralNode(LiteralTokenNode* rep_, int64_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    int64_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    int64_t value;
};

class ULongLiteralNode : public SyntaxNode
{
public:
    ULongLiteralNode(LiteralTokenNode* rep_, uint64_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    uint64_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    uint64_t value;
};

class FloatLiteralNode : public SyntaxNode
{
public:
    FloatLiteralNode(LiteralTokenNode* rep_, float value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    float Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    float value;
};

class DoubleLiteralNode : public SyntaxNode
{
public:
    DoubleLiteralNode(LiteralTokenNode* rep_, double value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    double Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    double value;
};

class CharLiteralNode : public SyntaxNode
{
public:
    CharLiteralNode(LiteralTokenNode* rep_, char value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    char Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    char value;
};

class WCharLiteralNode : public SyntaxNode
{
public:
    WCharLiteralNode(LiteralTokenNode* rep_, char16_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    char16_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    char16_t value;
};

class UCharLiteralNode : public SyntaxNode
{
public:
    UCharLiteralNode(LiteralTokenNode* rep_, char32_t value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    char32_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    char32_t value;
};

class StringLiteralNode : public SyntaxNode
{
public:
    StringLiteralNode(LiteralTokenNode* rep_, const std::string& value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    const std::string& Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    std::string value;
};

class WStringLiteralNode : public SyntaxNode
{
public:
    WStringLiteralNode(LiteralTokenNode* rep_, const std::u16string& value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    const std::u16string& Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    std::u16string value;
};

class UStringLiteralNode : public SyntaxNode
{
public:
    UStringLiteralNode(LiteralTokenNode* rep_, const std::u32string& value_);
    LiteralTokenNode* Rep() const { return rep.get(); }
    const std::u32string& Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LiteralTokenNode> rep;
    std::u32string value;
};

class NullLiteralNode : public SyntaxNode
{
public:
    NullLiteralNode(KeywordNode* rep_);
    KeywordNode* Rep() const { return rep.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<KeywordNode> rep;
};

class ArrayLiteralNode : public SyntaxNode
{
public:
    ArrayLiteralNode();
    void AddNode(Node* node) override;
    void AddComma(CommaNode* comma) override;
    const ListNode& ValueList() const { return valueList; }
    ListNode& ValueList() { return valueList; }
    void SetLBracket(LBracketNode* lbracket_);
    LBracketNode* LBracket() const { return lbracket.get(); }
    void SetRBracket(RBracketNode* rbracket_);
    RBracketNode* RBracket() const { return rbracket.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LBracketNode> lbracket;
    std::unique_ptr<RBracketNode> rbracket;
    ListNode valueList;
};

class StructuredLiteralNode : public SyntaxNode
{
public:
    StructuredLiteralNode();
    void AddNode(Node* node) override;
    void AddComma(CommaNode* comma) override;
    const ListNode& ValueList() const { return valueList; }
    ListNode& ValueList() { return valueList; }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LBraceNode> lbrace;
    std::unique_ptr<RBraceNode> rbrace;
    ListNode valueList;
};

Node* CreateIntegerLiteralNode(LiteralTokenNode* rep, bool unsignedSuffix, uint64_t value);
Node* CreateFloatingLiteralNode(LiteralTokenNode* rep, bool isFloat, double value);
Node* CreateCharacterLiteralNode(LiteralTokenNode* rep, char16_t value, CharLiteralPrefix prefix);
Node* CreateStringLiteralNode(LiteralTokenNode* rep, const std::u16string& value, StringLiteralPrefix prefix);


} // namespace cmajor::fault::tolerant::ast
