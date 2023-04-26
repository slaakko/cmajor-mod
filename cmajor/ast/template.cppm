// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.template_;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class IdentifierNode;

class TemplateIdNode : public Node
{
public:
    TemplateIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TemplateIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* primary_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    Node* Primary() { return primary.get(); }
    void AddTemplateArgument(Node* templateArgument);
    const NodeList<Node>& TemplateArguments() const { return templateArguments; }
private:
    std::unique_ptr<Node> primary;
    NodeList<Node> templateArguments;
};

class TemplateParameterNode : public Node
{
public:
    TemplateParameterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TemplateParameterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* id_, Node* defaultTemplateArgument_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const Node* DefaultTemplateArgument() const { return defaultTemplateArgument.get(); }
    Node* DefaultTemplateArgument() { return defaultTemplateArgument.get(); }
 private:
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> defaultTemplateArgument;
};

class FullInstantiationRequestNode : public Node
{
public:
    FullInstantiationRequestNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    FullInstantiationRequestNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, TemplateIdNode* templateId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    TemplateIdNode* TemplateId() const { return templateId.get(); }
private:
    std::unique_ptr<TemplateIdNode> templateId;
};
} // namespace cmajor::ast
