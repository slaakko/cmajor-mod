// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.namespace_;

import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.function;
import util;

namespace cmajor::ast {

NamespaceNode::NamespaceNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::namespaceNode, sourcePos_, moduleId_), id(), flags()
{
}

NamespaceNode::NamespaceNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, IdentifierNode* id_) : Node(NodeType::namespaceNode, sourcePos_, moduleId_), id(id_), flags()
{
    if (id == nullptr)
    {
        SetUnnamedNs();
        util::Sha1 sha1;
        boost::uuids::uuid randomUuid;
        util::RandomUuid(randomUuid);
        for (uint8_t x : randomUuid)
        {
            sha1.Process(x);
        }
        id.reset(new IdentifierNode(sourcePos_, moduleId_, U"unnamed_ns_" + util::ToUtf32(sha1.GetDigest())));
    }
    id->SetParent(this);
}

IdentifierNode* NamespaceNode::Id() const
{
    return id.get();
}

Node* NamespaceNode::Clone(CloneContext& cloneContext) const
{
    NamespaceNode* clone = nullptr;
    if (IsUnnamedNs())
    {
        clone = new NamespaceNode(GetSourcePos(), ModuleId(), nullptr);
    }
    else
    {
        clone = new NamespaceNode(GetSourcePos(), ModuleId(), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    }
    clone->flags = flags;
    int n = members.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* member = members[i];
        if (cloneContext.MakeTestUnits() && member->GetNodeType() == NodeType::functionNode && (static_cast<FunctionNode*>(member)->GetSpecifiers() & Specifiers::unit_test_) != Specifiers::none)
        {
            FunctionNode* unitTestFunction = static_cast<FunctionNode*>(member->Clone(cloneContext));
            unitTestFunction->SetParent(const_cast<NamespaceNode*>(this));
            cloneContext.AddUnitTestFunction(unitTestFunction);
        }
        else
        {
            clone->AddMember(member->Clone(cloneContext));
        }
    }
    return clone;
}

void NamespaceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NamespaceNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    writer.GetBinaryStreamWriter().Write(static_cast<int8_t>(flags));
    members.Write(writer);
}

void NamespaceNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    flags = static_cast<NsFlags>(reader.GetBinaryStreamReader().ReadSByte());
    members.Read(reader);
    members.SetParent(this);
}

void NamespaceNode::AddMember(Node* member)
{
    member->SetParent(this);
    members.Add(member);
    if (member->GetNodeType() == NodeType::namespaceNode)
    {
        NamespaceNode* ns = static_cast<NamespaceNode*>(member);
        if (ns->IsUnnamedNs() || ns->HasUnnamedNs())
        {
            SetHasUnnamedNs();
        }
    }
}

AliasNode::AliasNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::aliasNode, sourcePos_, moduleId_), id(), qid()
{
}

AliasNode::AliasNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, IdentifierNode* id_, IdentifierNode* qid_) : Node(NodeType::aliasNode, sourcePos_, moduleId_), id(id_), qid(qid_)
{
    id->SetParent(this);
    qid->SetParent(this);
}

Node* AliasNode::Clone(CloneContext& cloneContext) const
{
    AliasNode* clone = new AliasNode(GetSourcePos(), ModuleId(), static_cast<IdentifierNode*>(id->Clone(cloneContext)), static_cast<IdentifierNode*>(qid->Clone(cloneContext)));
    return clone;
}

void AliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AliasNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    writer.Write(qid.get());
}

void AliasNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    qid.reset(reader.ReadIdentifierNode());
    qid->SetParent(this);
}

IdentifierNode* AliasNode::Id() const
{
    return id.get();
}

IdentifierNode* AliasNode::Qid() const
{
    return qid.get();
}

NamespaceImportNode::NamespaceImportNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::namespaceImportNode, sourcePos_, moduleId_), ns()
{
}

NamespaceImportNode::NamespaceImportNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, IdentifierNode* ns_) : Node(NodeType::namespaceImportNode, sourcePos_, moduleId_), ns(ns_)
{
    ns->SetParent(this);
}

Node* NamespaceImportNode::Clone(CloneContext& cloneContext) const
{
    NamespaceImportNode* clone = new NamespaceImportNode(GetSourcePos(), ModuleId(), static_cast<IdentifierNode*>(ns->Clone(cloneContext)));
    return clone;
}

void NamespaceImportNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NamespaceImportNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(ns.get());
}

void NamespaceImportNode::Read(AstReader& reader)
{
    Node::Read(reader);
    ns.reset(reader.ReadIdentifierNode());
    ns->SetParent(this);
}

IdentifierNode* NamespaceImportNode::Ns() const
{
    return ns.get();
}

} // namespace cmajor::ast