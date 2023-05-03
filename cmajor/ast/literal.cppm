// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.literal;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

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

class LiteralNode : public Node
{
public:
    LiteralNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void SetText(const std::u32string& text_);
    const std::u32string& Text() const { return text; }
private:
    std::u32string text;
};

 LiteralNode* CreateIntegerLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, uint64_t value, bool unsignedSuffix);
 LiteralNode* CreateFloatingLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, double value, bool float_);
 LiteralNode* CreateCharacterLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, char32_t value, CharLiteralPrefix prefix);
 LiteralNode* CreateStringLiteralNode(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::u32string& value, StringLiteralPrefix prefix);

class  BooleanLiteralNode : public LiteralNode
{
public:
    BooleanLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BooleanLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, bool value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    bool Value() const { return value; }
private:
    bool value;
};

class  SByteLiteralNode : public LiteralNode
{
public:
    SByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    SByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int8_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    int8_t Value() const { return value; }
private:
    int8_t value;
};

class  ByteLiteralNode : public LiteralNode
{
public:
    ByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ByteLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint8_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    uint8_t Value() const { return value; }
private:
    uint8_t value;
};

class  ShortLiteralNode : public LiteralNode
{
public:
    ShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int16_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    int16_t Value() const { return value; }
private:
    int16_t value;
};

class  UShortLiteralNode : public LiteralNode
{
public:
    UShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UShortLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint16_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    uint16_t Value() const { return value; }
private:
    uint16_t value;
};

class  IntLiteralNode : public LiteralNode
{
public:
    IntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    IntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int32_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    int32_t Value() const { return value; }
    void SetValue(int32_t value_) { value = value_; }
private:
    int32_t value;
};

class  UIntLiteralNode : public LiteralNode
{
public:
    UIntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UIntLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint32_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    uint32_t Value() const { return value; }
private:
    uint32_t value;
};

class  LongLiteralNode : public LiteralNode
{
public:
    LongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, int64_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    int64_t Value() const { return value; }
private:
    int64_t value;
};

class  ULongLiteralNode : public LiteralNode
{
public:
    ULongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ULongLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, uint64_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    uint64_t Value() const { return value; }
private:
    uint64_t value;
};

class  FloatLiteralNode : public LiteralNode
{
public:
    FloatLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    FloatLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, float value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    float Value() const { return value; }
private:
    float value;
};

class  DoubleLiteralNode : public LiteralNode
{
public:
    DoubleLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DoubleLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, double value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    double Value() const { return value; }
private:
    double value;
};

class  CharLiteralNode : public LiteralNode
{
public:
    CharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    CharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    char Value() const { return value; }
private:
    char value;
};

class  WCharLiteralNode : public LiteralNode
{
public:
    WCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    WCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char16_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    char16_t Value() const { return value; }
private:
    char16_t value;
};

class  UCharLiteralNode : public LiteralNode
{
public:
    UCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UCharLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, char32_t value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    char32_t Value() const { return value; }
private:
    char32_t value;
};

class  StringLiteralNode : public LiteralNode
{
public:
    StringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    StringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::string& value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const std::string& Value() const { return value; }
private:
    std::string value;
};

class  WStringLiteralNode : public LiteralNode
{
public:
    WStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    WStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u16string& value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const std::u16string& Value() const { return value; }
private:
    std::u16string value;
};

class  UStringLiteralNode : public LiteralNode
{
public:
    UStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UStringLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& value_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const std::u32string& Value() const { return value; }
private:
    std::u32string value;
};

class  NullLiteralNode : public LiteralNode
{
public:
    NullLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "null"; }
};

class  ArrayLiteralNode : public LiteralNode
{
public:
    ArrayLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override { return "array"; }
    void AddValue(Node* value);
    const NodeList<Node>& Values() const { return values; }
    NodeList<Node>& Values() { return values; }
private:
    NodeList<Node> values;
};

class  StructuredLiteralNode : public LiteralNode
{
public:
    StructuredLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override { return "structure"; }
    void AddMember(Node* member);
    const NodeList<Node>& Members() const { return members; }
    NodeList<Node>& Members() { return members; }
private:
    NodeList<Node> members;
};

class  UuidLiteralNode : public LiteralNode
{
public:
    UuidLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UuidLiteralNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const util::uuid& uuid_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override { return "uuid"; }
    const util::uuid& GetUuid() const { return uuid; }
private:
    util::uuid uuid;
};

    
} // namespace cmajor::ast


