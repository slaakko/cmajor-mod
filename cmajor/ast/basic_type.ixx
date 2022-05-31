// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.basic.type;

import cmajor.ast.node;
import cmajor.ast.clone;

export namespace cmajor::ast {

class BoolNode : public Node
{
public:
    BoolNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "bool"; }
};

class SByteNode : public Node
{
public:
    SByteNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "sbyte"; }
};

class ByteNode : public Node
{
public:
    ByteNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsUnsignedTypeNode() const override { return true; }
    std::string ToString() const override { return "byte"; }
};

class ShortNode : public Node
{
public:
    ShortNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "short"; }
};

class UShortNode : public Node
{
public:
    UShortNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsUnsignedTypeNode() const override { return true; }
    std::string ToString() const override { return "ushort"; }
};

class IntNode : public Node
{
public:
    IntNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "int"; }
};

class UIntNode : public Node
{
public:
    UIntNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsUnsignedTypeNode() const override { return true; }
    std::string ToString() const override { return "uint"; }
};

class LongNode : public Node
{
public:
    LongNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "long"; }
};

class ULongNode : public Node
{
public:
    ULongNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    bool IsUnsignedTypeNode() const override { return true; }
    std::string ToString() const override { return "ulong"; }
};

class FloatNode : public Node
{
public:
    FloatNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "float"; }
};

class DoubleNode : public Node
{
public:
    DoubleNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "double"; }
};

class CharNode : public Node
{
public:
    CharNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "char"; }
};

class WCharNode : public Node
{
public:
    WCharNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "wchar"; }
};

class UCharNode : public Node
{
public:
    UCharNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "uchar"; }
};

class VoidNode : public Node
{
public:
    VoidNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override { return "void"; }
};

} // namespace cmajor::ast
