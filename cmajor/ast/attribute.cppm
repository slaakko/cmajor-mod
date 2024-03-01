// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.attribute;

import cmajor.ast.node;

export namespace cmajor::ast {

class AttributeNode : public Node
{
public:
    AttributeNode(const soul::ast::Span& span_);
    AttributeNode(const soul::ast::Span& span_, const std::u32string& name_, const std::u32string& value_);
    AttributeNode(const AttributeNode&) = delete;
    AttributeNode& operator=(const AttributeNode&) = delete;
    const std::u32string& Name() const { return name; }
    const std::u32string& Value() const { return value; }
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::u32string name;
    std::u32string value;
};

class AttributesNode : public Node
{
public:
    AttributesNode(const soul::ast::Span& span_);
    AttributesNode(const AttributesNode&) = delete;
    AttributesNode& operator=(const AttributesNode&) = delete;
    const std::vector<std::unique_ptr<AttributeNode>>& GetAttributes() const { return attributes; }
    void AddAttribute(const soul::ast::Span& span, const std::u32string& name);
    void AddAttribute(const soul::ast::Span& span, const std::u32string& name, const std::u32string& value);
    AttributeNode* GetAttribute(const std::u32string& name) const;
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<AttributeNode>> attributes;
    std::map<std::u32string, AttributeNode*> attributeMap;
    void AddAttribute(AttributeNode* attribute);
};

class AttributeNotUniqueException : public std::runtime_error
{
public:
    AttributeNotUniqueException(const std::string& message_, const soul::ast::Span& span_,
        const soul::ast::Span& prevSpan_, const util::uuid& prevModuleId_);
    const soul::ast::Span& GetSpan() const { return span; }
    const util::uuid& ModuleId() const { return moduleId; }
    const soul::ast::Span& PrevSpan() const { return prevSpan; }
    const util::uuid& PrevModuleId() const { return prevModuleId; }
private:
    soul::ast::Span span;
    util::uuid moduleId;
    soul::ast::Span prevSpan;
    util::uuid prevModuleId;
};

} // namespace cmajor::ast


