// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.serialization.ast;

import std.core;
import soul.ast.source.pos;

export namespace soul::xml::serialization::ast {

class Visitor;

class ExportModule
{
public:
    ExportModule(const std::string& moduleName_);
    const std::string& ModuleName() const { return moduleName; }
private:
    std::string moduleName;
};

enum class ImportPrefix
{
    interfacePrefix, implementationPrefix
};

class Import
{
public:
    Import(const std::string& moduleName_, ImportPrefix prefix_);
    const std::string& ModuleName() const { return moduleName; }
    ImportPrefix Prefix() const { return prefix; }
private:
    std::string moduleName;
    ImportPrefix prefix;
};

enum class NodeKind
{
    forwardClassDeclarationNode, usingAliasNode,
    boolNode, sbyteNode, byteNode, shortNode, ushortNode, intNode, uintNode, longNode, ulongNode, floatNode, doubleNode,
    charNode, wcharNode, ucharNode, uuidNode, stringNode, wstringNode, ustringNode, dateNode, datetimeNode, timePointNode, durationNode,
    enumConstantNode, enumTypeNode, classIdNode, pointerTypeNode, arrayTypeNode, memberVariableNode, cppBlockNode, classNode, namespaceNode, sourceFileNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    Node* Parent() const { return parent; }
    void SetParent(Node* parent_) { parent = parent_; }
    bool IsMemberVariableNode() const { return kind == NodeKind::memberVariableNode; }
    bool IsNamespaceNode() const { return kind == NodeKind::namespaceNode; }
    bool IsGlobalNamespace() const;
    virtual void Accept(Visitor& visitor) = 0;
private:
    NodeKind kind;
    soul::ast::SourcePos sourcePos;
    Node* parent;
};

class ForwardClassDeclarationNode : public Node
{
public:
    ForwardClassDeclarationNode(const soul::ast::SourcePos& sourcePos_, const std::string& classId_);
    const std::string& ClassId() const { return classId; }
    void Accept(Visitor& visitor) override;
private:
    std::string classId;
};

class UsingAliasNode : public Node
{
public:
    UsingAliasNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_, const std::string& qualifiedCppId_);
    const std::string& Id() const { return id; }
    const std::string& QualifiedCppId() const { return qualifiedCppId; }
    void Accept(Visitor& visitor) override;
private:
    std::string id;
    std::string qualifiedCppId;
};

class TypeNode : public Node
{
public:
    TypeNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
};

class BoolNode : public TypeNode
{
public:
    BoolNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class SByteNode : public TypeNode
{
public:
    SByteNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class ByteNode : public TypeNode
{
public:
    ByteNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class ShortNode : public TypeNode
{
public:
    ShortNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class UShortNode : public TypeNode
{
public:
    UShortNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class IntNode : public TypeNode
{
public:
    IntNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class UIntNode : public TypeNode
{
public:
    UIntNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class LongNode : public TypeNode
{
public:
    LongNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class ULongNode : public TypeNode
{
public:
    ULongNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class FloatNode : public TypeNode
{
public:
    FloatNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class DoubleNode : public TypeNode
{
public:
    DoubleNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class CharNode : public TypeNode
{
public:
    CharNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class WCharNode : public TypeNode
{
public:
    WCharNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class UCharNode : public TypeNode
{
public:
    UCharNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class UuidNode : public TypeNode
{
public:
    UuidNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class StringNode : public TypeNode
{
public:
    StringNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class WStringNode : public TypeNode
{
public:
    WStringNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class UStringNode : public TypeNode
{
public:
    UStringNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class DateNode : public TypeNode
{
public:
    DateNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class DateTimeNode : public TypeNode
{
public:
    DateTimeNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class TimePointNode : public TypeNode
{
public:
    TimePointNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class DurationNode : public TypeNode
{
public:
    DurationNode(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
};

class EnumConstantNode : public Node
{
public:
    EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void Accept(Visitor& visitor) override;
private:
    std::string id;
};

class EnumTypeNode : public TypeNode
{
public:
    EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void AddEnumConstant(EnumConstantNode* enumConstant);
    const std::vector<std::unique_ptr<EnumConstantNode>>& EnumConstants() const { return enumConstants; }
    void Accept(Visitor& visitor) override;
private:
    std::string id;
    std::vector<std::unique_ptr<EnumConstantNode>> enumConstants;
};

class ClassIdNode : public TypeNode
{
public:
    ClassIdNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void Accept(Visitor& visitor) override;
private:
    std::string id;
};

class PointerTypeNode : public TypeNode
{
public:
    PointerTypeNode(const soul::ast::SourcePos& sourcePos_, TypeNode* pointedToType_);
    TypeNode* PointedToType() const { return pointedToType.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeNode> pointedToType;
};

class ArrayTypeNode : public TypeNode
{
public:
    ArrayTypeNode(const soul::ast::SourcePos& sourcePos_, TypeNode* elementType_);
    TypeNode* ElementType() const { return elementType.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeNode> elementType;
};

class MemberVariableNode : public Node
{
public:
    MemberVariableNode(const soul::ast::SourcePos& sourcePos_, TypeNode* type_, const std::string& id_);
    TypeNode* Type() const { return type.get(); }
    const std::string& Id() const { return id; }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeNode> type;
    std::string id;
};

class CppBlockNode : public Node
{
public:
    CppBlockNode(const soul::ast::SourcePos& sourcePos_, const std::string& cppText_);
    const std::string& CppText() const { return cppText; }
    bool Implementation() const { return implementation; }
    void SetImplementation() { implementation = true; }
    void Accept(Visitor& visitor) override;
private:
    std::string cppText;
    bool implementation;
};

class ClassNode : public Node
{
public:
    ClassNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void AddNode(Node* node);
    const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes; }
    void SetBaseClass(const std::string& baseClassId_);
    const std::string& BaseClassId() const { return baseClassId; }
    std::string FullClassName() const;
    void Accept(Visitor& visitor) override;
private:
    std::string id;
    std::vector<std::unique_ptr<Node>> nodes;
    std::string baseClassId;
};

class NamespaceNode : public Node
{
public:
    NamespaceNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_);
    const std::string& Id() const { return id; }
    void AddNode(Node* node);
    const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes; }
    bool IsGlobal() const { return id.empty(); }
    void Accept(Visitor& visitor) override;
private:
    std::string id;
    std::vector<std::unique_ptr<Node>> nodes;
};

class SourceFileNode : public Node
{
public:
    SourceFileNode(const soul::ast::SourcePos& sourcePos_);
    void SetModule(ExportModule* module_);
    ExportModule* Module() const { return module.get(); }
    void AddImport(Import* import);
    const std::vector<std::unique_ptr<Import>>& Imports() const { return imports; }
    NamespaceNode* GlobalNs() { return &globalNs; }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ExportModule> module;
    std::vector<std::unique_ptr<Import>> imports;
    NamespaceNode globalNs;
};

class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(ForwardClassDeclarationNode& node) {}
    virtual void Visit(UsingAliasNode& node) {}
    virtual void Visit(BoolNode& node) {}
    virtual void Visit(SByteNode& node) {}
    virtual void Visit(ByteNode& node) {}
    virtual void Visit(ShortNode& node) {}
    virtual void Visit(UShortNode& node) {}
    virtual void Visit(IntNode& node) {}
    virtual void Visit(UIntNode& node) {}
    virtual void Visit(LongNode& node) {}
    virtual void Visit(ULongNode& node) {}
    virtual void Visit(FloatNode& node) {}
    virtual void Visit(DoubleNode& node) {}
    virtual void Visit(CharNode& node) {}
    virtual void Visit(WCharNode& node) {}
    virtual void Visit(UCharNode& node) {}
    virtual void Visit(UuidNode& node) {}
    virtual void Visit(StringNode& node) {}
    virtual void Visit(WStringNode& node) {}
    virtual void Visit(UStringNode& node) {}
    virtual void Visit(DateNode& node) {}
    virtual void Visit(DateTimeNode& node) {}
    virtual void Visit(TimePointNode& node) {}
    virtual void Visit(DurationNode& node) {}
    virtual void Visit(EnumConstantNode& node) {}
    virtual void Visit(EnumTypeNode& node) {}
    virtual void Visit(ClassIdNode& node) {}
    virtual void Visit(PointerTypeNode& node) {}
    virtual void Visit(ArrayTypeNode& node) {}
    virtual void Visit(MemberVariableNode& node) {}
    virtual void Visit(CppBlockNode& node) {}
    virtual void Visit(ClassNode& node) {}
    virtual void Visit(NamespaceNode& node) {}
    virtual void Visit(SourceFileNode& node) {}
};

class DefaultVisitor : public Visitor
{
public:
    void Visit(SourceFileNode& node) override;
    void Visit(NamespaceNode& node) override;
    void Visit(ClassNode& node) override;
    void Visit(MemberVariableNode& node) override;
    void Visit(EnumTypeNode& node) override;
    void Visit(ArrayTypeNode& node) override;
    void Visit(PointerTypeNode& node) override;
};

} // namespace soul::xml::serialization::ast
