module cmajor.ast.compile.unit;

// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.classes;
import std.core;
import util;

namespace cmajor::ast {

CompileUnitNode::CompileUnitNode(const soul::ast::Span& span_) : 
    Node(NodeType::compileUnitNode, span_), globalNs(), isSynthesizedUnit(false), isProgramMainUnit(false), fileIndex(-1)
{
}

void CompileUnitNode::CreateGlobalNs()
{
    globalNs.reset(new NamespaceNode(soul::ast::Span(), new IdentifierNode(soul::ast::Span(), U"")));
}

CompileUnitNode::CompileUnitNode(const soul::ast::Span& span_, const std::string& filePath_) :
    Node(NodeType::compileUnitNode, span_), filePath(filePath_), globalNs(new NamespaceNode(span_, new IdentifierNode(span_, U""))), 
    isSynthesizedUnit(false), isProgramMainUnit(false), fileIndex(-1)
{
    globalNs->SetParent(this);
}

Node* CompileUnitNode::Clone(CloneContext& cloneContext) const
{
    CompileUnitNode* clone = new CompileUnitNode(GetSpan(), filePath);
    clone->globalNs.reset(static_cast<NamespaceNode*>(globalNs->Clone(cloneContext)));
    clone->globalNs->SetParent(clone);
    clone->SetModuleId(moduleId);
    clone->SetFileIndex(fileIndex);
    return clone;
}

void CompileUnitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompileUnitNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(filePath);
    writer.Write(globalNs.get());
    int32_t n = lineStarts.size();
    writer.GetBinaryStreamWriter().Write(n);
    for (auto s : lineStarts)
    {
        writer.GetBinaryStreamWriter().Write(s);
    }
    writer.GetBinaryStreamWriter().Write(isSynthesizedUnit);
    writer.GetBinaryStreamWriter().Write(id);
    writer.GetBinaryStreamWriter().Write(hash);
    writer.GetBinaryStreamWriter().Write(isProgramMainUnit);
    writer.GetBinaryStreamWriter().Write(moduleId);
    writer.GetBinaryStreamWriter().Write(fileIndex);
}

void CompileUnitNode::Read(AstReader& reader)
{
    Node::Read(reader);
    filePath = reader.GetBinaryStreamReader().ReadUtf8String();
    globalNs.reset(reader.ReadNamespaceNode());
    int32_t n = reader.GetBinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        lineStarts.push_back(reader.GetBinaryStreamReader().ReadInt());
    }
    isSynthesizedUnit = reader.GetBinaryStreamReader().ReadBool();
    id = reader.GetBinaryStreamReader().ReadUtf8String();
    hash = reader.GetBinaryStreamReader().ReadUtf8String();
    isProgramMainUnit = reader.GetBinaryStreamReader().ReadBool();
    reader.GetBinaryStreamReader().ReadUuid(moduleId);
    fileIndex = reader.GetBinaryStreamReader().ReadInt();
}

void CompileUnitNode::ResetGlobalNs(NamespaceNode* ns)
{
    globalNs.reset(ns);
}

soul::ast::Span CompileUnitNode::GlobalNsSpan() const
{
    if (globalNs)
    {
        return globalNs->GetSpan();
    }
    else
    {
        return soul::ast::Span();
    }
}

void CompileUnitNode::ComputeLineStarts(const std::u32string& sourceFileContent)
{
    lineStarts.push_back(0);
    int32_t index = 0;
    bool startOfLine = true;
    for (const char32_t& c : sourceFileContent)
    {
        if (startOfLine)
        {
            lineStarts.push_back(index);
            startOfLine = false;
        }
        if (c == U'\n')
        {
            startOfLine = true;
        }
        ++index;
    }
    lineStarts.push_back(index);
}

int CompileUnitNode::GetColumn(const soul::ast::Span& span) const
{
    if (span.IsValid())
    {
        int32_t pos = span.pos;
        auto it = std::lower_bound(lineStarts.cbegin(), lineStarts.cend(), pos);
        if (it != lineStarts.cend())
        {
            int32_t start = *it;
            if (start != pos && it != lineStarts.cbegin())
            {
                --it;
                start = *it;
            }
            int column = 1 + pos - start;
            return column;
        }
    }
    return 1;
}

void CompileUnitNode::SetModuleId(const util::uuid& moduleId_)
{
    moduleId = moduleId_;
}

const std::string& CompileUnitNode::Id()
{
    if (id.empty())
    {
        std::string baseName = util::Path::GetFileNameWithoutExtension(filePath);
        for (char& c : baseName)
        {
            if (!std::isalnum(c))
            {
                c = '_';
            }
        }
        id = baseName + "_" + util::GetSha1MessageDigest(filePath);
    }
    return id;
}

void CompileUnitNode::SetId(const std::string& id_)
{
    id = id_;
}

class NamespaceCombiner : public Visitor
{
public:
    void Visit(CompileUnitNode& compileUnitNode) override;
    void Visit(NamespaceNode& namespaceNode) override;
};

void NamespaceCombiner::Visit(CompileUnitNode& compileUnitNode)
{
    compileUnitNode.GlobalNs()->Accept(*this);
}

void NamespaceCombiner::Visit(NamespaceNode& namespaceNode)
{
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        namespaceNode.Members()[i]->Accept(*this);
    }
    if (n == 1 && !namespaceNode.Id()->Str().empty())
    {
        Node* node = namespaceNode.Members()[0];
        if (node->GetNodeType() == NodeType::namespaceNode)
        {
            std::unique_ptr<NamespaceNode> childNs(static_cast<NamespaceNode*>(namespaceNode.Members().Release(0)));
            namespaceNode.Id()->SetStr(namespaceNode.Id()->Str() + U"." + childNs->Id()->Str());
            namespaceNode.Members().Clear();
            namespaceNode.Members().SetContent(childNs->Members().Content());
            namespaceNode.Members().SetParent(&namespaceNode);
        }
    }
}

void CombineNamespaces(CompileUnitNode& cu)
{
    NamespaceCombiner combiner;
    cu.Accept(combiner);
}

class ClassMemberArranger : public Visitor
{
public:
    void Visit(CompileUnitNode& compileUnitNode) override;
    void Visit(NamespaceNode& namespaceNode) override;
    void Visit(ClassNode& classNode) override;
};

void ClassMemberArranger::Visit(CompileUnitNode& compileUnitNode)
{
    compileUnitNode.GlobalNs()->Accept(*this);
}

void ClassMemberArranger::Visit(NamespaceNode& namespaceNode)
{
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        namespaceNode.Members()[i]->Accept(*this);
    }
}

void ClassMemberArranger::Visit(ClassNode& classNode)
{
    int n = classNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        classNode.Members()[i]->Accept(*this);
    }
    classNode.ArrangeMembers();
}

void ArrangeClassMembers(CompileUnitNode& cu)
{
    ClassMemberArranger arranger;
    cu.Accept(arranger);
}

class UnnamedNamespaceProcessor : public Visitor
{
public:
    UnnamedNamespaceProcessor();
    void Visit(CompileUnitNode& compileUnitNode) override;
    void Visit(NamespaceNode& namespaceNode) override;
private:
    std::vector<NamespaceNode*> unnamedNamespaces;
};

UnnamedNamespaceProcessor::UnnamedNamespaceProcessor()
{
}

void UnnamedNamespaceProcessor::Visit(CompileUnitNode& compileUnitNode)
{
    compileUnitNode.GlobalNs()->Accept(*this);
    int index = 0;
    for (NamespaceNode* unnamedNs : unnamedNamespaces)
    {
        CloneContext cloneContext;
        IdentifierNode* unnamedNsId = static_cast<IdentifierNode*>(unnamedNs->Id()->Clone(cloneContext));
        NamespaceImportNode* import = new NamespaceImportNode(compileUnitNode.GetSpan(), unnamedNsId);
        compileUnitNode.GlobalNs()->Members().Insert(index, import);
        ++index;
    }
}

void UnnamedNamespaceProcessor::Visit(NamespaceNode& namespaceNode)
{
    if (namespaceNode.IsUnnamedNs())
    {
        unnamedNamespaces.push_back(&namespaceNode);
    }
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        namespaceNode.Members()[i]->Accept(*this);
    }
}

void AddNamespaceImportsForUnnamedNamespaces(CompileUnitNode& cu)
{
    UnnamedNamespaceProcessor processor;
    cu.Accept(processor);
}

} // namespace cmajor::ast
