// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.constant;

import cmajor.systemx.assembler.node;
import std.core;

export namespace cmajor::systemx::assembler {

class Constant : public Node
{
public:
    Constant(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
};

class IntegralConstant : public Constant
{
public:
    IntegralConstant(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, uint64_t value_);
    uint64_t Value() const { return value; }
    void SetValue(uint64_t value_) { value = value_; }
private:
    uint64_t value;
};

class DecimalConstant : public IntegralConstant
{
public:
    DecimalConstant(const soul::ast::SourcePos& sourcePos_, uint64_t value_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

class HexadecimalConstant : public IntegralConstant
{
public:
    HexadecimalConstant(const soul::ast::SourcePos& sourcePos_, uint64_t value_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

class ByteConstant : public IntegralConstant
{
public:
    ByteConstant(const soul::ast::SourcePos& sourcePos_, uint8_t value_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

class WydeConstant : public IntegralConstant
{
public:
    WydeConstant(const soul::ast::SourcePos& sourcePos_, uint16_t value_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

class TetraConstant : public IntegralConstant
{
public:
    TetraConstant(const soul::ast::SourcePos& sourcePos_, uint32_t value_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

class CharacterConstant : public Constant
{
public:
    CharacterConstant(const soul::ast::SourcePos& sourcePos_, char32_t value_);
    char32_t Value() const { return value; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    char32_t value;
};

class StringConstant : public Constant
{
public:
    StringConstant(const soul::ast::SourcePos& sourcePos_, const std::u32string& value_);
    const std::u32string& Value() const { return value; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::u32string value;
};

class ClsIdConstant : public Constant
{
public:
    ClsIdConstant(const soul::ast::SourcePos& sourcePos_, const std::string& typeId_);
    const std::string& TypeId() const { return typeId; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string typeId;
};

Node* MakeConstantExpr(bool value);
Node* MakeConstantExpr(int8_t value);
Node* MakeConstantExpr(uint8_t value);
Node* MakeConstantExpr(int16_t value);
Node* MakeConstantExpr(uint16_t value);
Node* MakeConstantExpr(int32_t value);
Node* MakeConstantExpr(uint32_t value);
Node* MakeConstantExpr(int64_t value);
Node* MakeConstantExpr(uint64_t value);
Node* MakeConstantExpr(uint64_t value, bool hex);
Node* MakeConstantExpr(float value);
Node* MakeConstantExpr(double value);
Node* MakeConstantExpr(const std::string& str);

} // namespace cmsx::assembler
