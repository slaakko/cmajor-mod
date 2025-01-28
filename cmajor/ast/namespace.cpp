// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.namespaces;

import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.function;
import cmajor.ast.statement;
import cmajor.ast.concepts;
import cmajor.ast.templates;
import cmajor.ast.parameter;
import util;

namespace cmajor::ast {

NamespaceNode::NamespaceNode(const soul::ast::Span& span_) : 
    Node(NodeType::namespaceNode, span_), id(), flags(), fileIndex(-1), moduleId(util::nil_uuid())
{
}

NamespaceNode::NamespaceNode(const soul::ast::Span& span_, IdentifierNode* id_) : 
    Node(NodeType::namespaceNode, span_), id(id_), flags(), fileIndex(-1), moduleId(util::nil_uuid())
{
    if (id == nullptr)
    {
        SetUnnamedNs();
        util::Sha1 sha1;
        util::uuid randomUuid = util::random_uuid();
        for (uint8_t x : randomUuid)
        {
            sha1.Process(x);
        }
        id.reset(new IdentifierNode(span_, U"unnamed_ns_" + util::ToUtf32(sha1.GetDigest())));
    }
    id->SetParent(this);
}

IdentifierNode* NamespaceNode::Id() const
{
    return id.get();
}

int32_t NamespaceNode::FileIndex() const
{
    if (fileIndex != -1)
    {
        return fileIndex;
    }
    else if (Parent())
    {
        return Parent()->FileIndex();
    }
    else
    {
        return -1;
    }
}

void NamespaceNode::SetModuleId(const util::uuid& moduleId_)
{
    moduleId = moduleId_;
}

const util::uuid& NamespaceNode::ModuleId() const
{
    if (!moduleId.is_nil())
    {
        return moduleId;
    }
    else if (Parent())
    {
        return Parent()->ModuleId();
    }
    else
    {
        static util::uuid emptyId;
        return emptyId;
    }
}

Node* NamespaceNode::Clone(CloneContext& cloneContext) const
{
    NamespaceNode* clone = nullptr;
    if (IsUnnamedNs())
    {
        clone = new NamespaceNode(GetSpan(), nullptr);
    }
    else
    {
        clone = new NamespaceNode(GetSpan(), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    }
    clone->flags = flags;
    clone->fileIndex = fileIndex;
    clone->moduleId = moduleId;
    int n = members.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* member = members[i];
        if (cloneContext.MakeTestUnits() && member->GetNodeType() == NodeType::functionNode && 
            (static_cast<FunctionNode*>(member)->GetSpecifiers() & Specifiers::unit_test_) != Specifiers::none)
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
    writer.GetBinaryStreamWriter().Write(fileIndex);
    writer.GetBinaryStreamWriter().Write(moduleId);
    members.Write(writer);
}

void NamespaceNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    flags = static_cast<NsFlags>(reader.GetBinaryStreamReader().ReadSByte());
    fileIndex = reader.GetBinaryStreamReader().ReadInt();
    reader.GetBinaryStreamReader().ReadUuid(moduleId);
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

NamespaceImportNode::NamespaceImportNode(const soul::ast::Span& span_) : 
    Node(NodeType::namespaceImportNode, span_), ns()
{
}

NamespaceImportNode::NamespaceImportNode(const soul::ast::Span& span_, IdentifierNode* ns_) : 
    Node(NodeType::namespaceImportNode, span_), ns(ns_)
{
    ns->SetParent(this);
}

Node* NamespaceImportNode::Clone(CloneContext& cloneContext) const
{
    NamespaceImportNode* clone = new NamespaceImportNode(GetSpan(), static_cast<IdentifierNode*>(ns->Clone(cloneContext)));
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
