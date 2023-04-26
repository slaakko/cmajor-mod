// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.type.expr;

import cmajor.ast.node;

export namespace cmajor::ast {

class ConstNode : public Node
{
public:
    ConstNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConstNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
private:
    std::unique_ptr<Node> subject;
};

class LValueRefNode : public Node
{
public:
    LValueRefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LValueRefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
private:
    std::unique_ptr<Node> subject;
};

class RValueRefNode : public Node
{
public:
    RValueRefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    RValueRefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
private:
    std::unique_ptr<Node> subject;
};

class PointerNode : public Node
{
public:
    PointerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PointerNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
private:
    std::unique_ptr<Node> subject;
};

class ArrayNode : public Node
{
public:
    ArrayNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ArrayNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_, Node* size_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
    const Node* Size() const { return size.get(); }
    Node* Size() { return size.get(); }
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<Node> size;
};
} // namespace cmajor::ast
