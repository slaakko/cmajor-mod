// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.specifier;

import cmajor.fault.tolerant.ast.node;
import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class Specifiers : int32_t
{
    none = 0,
    public_ = 1 << 0,
    protected_ = 1 << 1,
    private_ = 1 << 2,
    internal_ = 1 << 3,
    static_ = 1 << 4,
    virtual_ = 1 << 5,
    override_ = 1 << 6,
    abstract_ = 1 << 7,
    inline_ = 1 << 8,
    explicit_ = 1 << 9,
    external_ = 1 << 10,
    suppress_ = 1 << 11,
    default_ = 1 << 12,
    constexpr_ = 1 << 13,
    cdecl_ = 1 << 14,
    nothrow_ = 1 << 15,
    throw_ = 1 << 16,
    new_ = 1 << 17,
    const_ = 1 << 18,
    unit_test_ = 1 << 19,
    winapi = 1 << 20,
    access_ = public_ | protected_ | private_ | internal_
};

constexpr Specifiers operator|(Specifiers left, Specifiers right)
{
    return static_cast<Specifiers>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
}

constexpr Specifiers operator&(Specifiers left, Specifiers right)
{
    return static_cast<Specifiers>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
}

constexpr Specifiers operator~(Specifiers operand)
{
    return static_cast<Specifiers>(~static_cast<int32_t>(operand));
}

class SpecifierNode : public SyntaxNode
{
public:
    SpecifierNode(KeywordNode* keyword_, Specifiers specifier_);
    KeywordNode* Keyword() const { return keyword.get(); }
    Specifiers GetSpecifier() const { return specifier; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<KeywordNode> keyword;
    Specifiers specifier;
};

class SpecifiersNode : public SyntaxNode
{
public:
    SpecifiersNode(const soul::ast::Span& span_, Specifiers specifiers_);
    void SetSpecifiers(Specifiers specifiers_) { specifiers = specifiers_; }
    Specifiers GetSpecifiers() const { return specifiers; }
    void AddSpecifierNode(SpecifierNode* specifierNode);
    const std::vector<std::unique_ptr<SpecifierNode>>& SpecifierNodes() const { return specifierNodes; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    Specifiers specifiers;
    std::vector<std::unique_ptr<SpecifierNode>> specifierNodes;
};

} // namespace cmajor::fault::tolerant::ast
