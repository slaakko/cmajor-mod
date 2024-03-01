// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.serialization.ast;

namespace soul::xml::serialization::ast {

ExportModule::ExportModule(const std::string& moduleName_) : moduleName(moduleName_)
{
}

Import::Import(const std::string& moduleName_, ImportPrefix prefix_) : moduleName(moduleName_), prefix(prefix_)
{
}

Node::Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_) : kind(kind_), sourcePos(sourcePos_), parent(nullptr)
{
}

Node::~Node()
{
}

bool Node::IsGlobalNamespace() const 
{ 
    if (IsNamespaceNode())
    {
        const NamespaceNode* ns = static_cast<const NamespaceNode*>(this);
        return ns->IsGlobal();
    }
    return false;
}

ForwardClassDeclarationNode::ForwardClassDeclarationNode(const soul::ast::SourcePos& sourcePos_, const std::string& classId_) : 
    Node(NodeKind::forwardClassDeclarationNode, sourcePos_), classId(classId_)
{
}

void ForwardClassDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingAliasNode::UsingAliasNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_, const std::string& qualifiedCppId_) :
    Node(NodeKind::usingAliasNode, sourcePos_), id(id_), qualifiedCppId(qualifiedCppId_)
{
}

void UsingAliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNode::TypeNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_) : Node(kind_, sourcePos_)
{
}

BoolNode::BoolNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::boolNode, sourcePos_)
{
}

void BoolNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SByteNode::SByteNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::sbyteNode, sourcePos_)
{
}

void SByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByteNode::ByteNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::byteNode, sourcePos_)
{
}

void ByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShortNode::ShortNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::shortNode, sourcePos_)
{
}

void ShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UShortNode::UShortNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::ushortNode, sourcePos_)
{
}

void UShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntNode::IntNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::intNode, sourcePos_)
{
}

void IntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UIntNode::UIntNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::uintNode, sourcePos_)
{
}

void UIntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LongNode::LongNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::longNode, sourcePos_)
{
}

void LongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ULongNode::ULongNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::ulongNode, sourcePos_)
{
}

void ULongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatNode::FloatNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::floatNode, sourcePos_)
{
}

void FloatNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoubleNode::DoubleNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::doubleNode, sourcePos_)
{
}

void DoubleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharNode::CharNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::charNode, sourcePos_)
{
}

void CharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WCharNode::WCharNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::wcharNode, sourcePos_)
{
}

void WCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UCharNode::UCharNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::ucharNode, sourcePos_)
{
}

void UCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UuidNode::UuidNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::uuidNode, sourcePos_)
{
}

void UuidNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StringNode::StringNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::stringNode, sourcePos_)
{
}

void StringNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WStringNode::WStringNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::wstringNode, sourcePos_)
{
}

void WStringNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UStringNode::UStringNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::ustringNode, sourcePos_)
{
}

void UStringNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DateNode::DateNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::dateNode, sourcePos_)
{
}

void DateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DateTimeNode::DateTimeNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::datetimeNode, sourcePos_)
{
}

void DateTimeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TimePointNode::TimePointNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::timePointNode, sourcePos_)
{
}

void TimePointNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DurationNode::DurationNode(const soul::ast::SourcePos& sourcePos_) : TypeNode(NodeKind::durationNode, sourcePos_)
{
}

void DurationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EnumConstantNode::EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_) : Node(NodeKind::enumConstantNode, sourcePos_), id(id_)
{
}

void EnumConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EnumTypeNode::EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_) : TypeNode(NodeKind::enumTypeNode, sourcePos_), id(id_)
{
}

void EnumTypeNode::AddEnumConstant(EnumConstantNode* enumConstant)
{
    enumConstants.push_back(std::unique_ptr<EnumConstantNode>(enumConstant));
}

void EnumTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ClassIdNode::ClassIdNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_) : TypeNode(NodeKind::classIdNode, sourcePos_), id(id_)
{
}

void ClassIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PointerTypeNode::PointerTypeNode(const soul::ast::SourcePos& sourcePos_, TypeNode* pointedToType_) : TypeNode(NodeKind::pointerTypeNode, sourcePos_), pointedToType(pointedToType_)
{
}

void PointerTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayTypeNode::ArrayTypeNode(const soul::ast::SourcePos& sourcePos_, TypeNode* elementType_) : TypeNode(NodeKind::arrayTypeNode, sourcePos_), elementType(elementType_)
{
}

void ArrayTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberVariableNode::MemberVariableNode(const soul::ast::SourcePos& sourcePos_, TypeNode* type_, const std::string& id_) : 
    Node(NodeKind::memberVariableNode, sourcePos_), type(type_), id(id_)
{
}

void MemberVariableNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CppBlockNode::CppBlockNode(const soul::ast::SourcePos& sourcePos_, const std::string& cppText_) : Node(NodeKind::cppBlockNode, sourcePos_), cppText(cppText_)
{
}

void CppBlockNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ClassNode::ClassNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_) : Node(NodeKind::classNode, sourcePos_), id(id_)
{
}

void ClassNode::AddNode(Node* node)
{
    node->SetParent(this);
    nodes.push_back(std::unique_ptr<Node>(node));
}

void ClassNode::SetBaseClass(const std::string& baseClassId_)
{
    baseClassId = baseClassId_;
}

void ClassNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ClassNode::FullClassName() const
{
    std::string fullNsName;
    Node* parent = Parent();
    while (parent)
    {
        if (parent->IsNamespaceNode())
        {
            NamespaceNode* nsNode = static_cast<NamespaceNode*>(parent);
            if (!nsNode->IsGlobal())
            {
                if (fullNsName.empty())
                {
                    fullNsName = nsNode->Id();
                }
                else
                {
                    fullNsName = nsNode->Id() + "::" + fullNsName;
                }
            }
        }
        parent = parent->Parent();
    }
    if (fullNsName.empty())
    {
        return id;
    }
    else
    {
        return fullNsName + "::" + id;
    }
}

NamespaceNode::NamespaceNode(const soul::ast::SourcePos& sourcePos_, const std::string& id_) : Node(NodeKind::namespaceNode, sourcePos_), id(id_)
{
}

void NamespaceNode::AddNode(Node* node)
{
    node->SetParent(this);
    nodes.push_back(std::unique_ptr<Node>(node));
}

void NamespaceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SourceFileNode::SourceFileNode(const soul::ast::SourcePos& sourcePos_) : Node(NodeKind::sourceFileNode, sourcePos_), globalNs(soul::ast::SourcePos(), std::string())
{
}

void SourceFileNode::SetModule(ExportModule* module_)
{
    module.reset(module_);
}

void SourceFileNode::AddImport(Import* import)
{
    imports.push_back(std::unique_ptr<Import>(import));
}

void SourceFileNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Visitor::~Visitor()
{
}

void DefaultVisitor::Visit(SourceFileNode& node) 
{
    node.GlobalNs()->Accept(*this);
}

void DefaultVisitor::Visit(NamespaceNode& node)
{
    for (const auto& n : node.Nodes())
    {
        n->Accept(*this);
    }
}

void DefaultVisitor::Visit(ClassNode& node)
{
    for (const auto& n : node.Nodes())
    {
        n->Accept(*this);
    }
}

void DefaultVisitor::Visit(MemberVariableNode& node)
{
    node.Type()->Accept(*this);
}

void DefaultVisitor::Visit(EnumTypeNode& node)
{
    for (const auto& enumConstant : node.EnumConstants())
    {
        enumConstant->Accept(*this);
    }
}

void DefaultVisitor::Visit(ArrayTypeNode& node)
{
    node.ElementType()->Accept(*this);
}

void DefaultVisitor::Visit(PointerTypeNode& node)
{
    node.PointedToType()->Accept(*this);
}

} // namespace soul::xml::serialization::ast
