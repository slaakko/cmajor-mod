// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.compile.unit;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

CompileUnitNode::CompileUnitNode() : SyntaxNode(soul::ast::Span(), CompletionContext::global), compileUnitIndex(-1), valid(true), project(nullptr)
{
    globalNs.reset(new NamespaceDefinitionNode(soul::ast::Span(), CompletionContext::global));
}

void CompileUnitNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

void CompileUnitNode::SetLineStarts(std::vector<int>&& lineStarts_)
{
    lineStarts = std::move(lineStarts_);
}

Node* CompileUnitNode::Clone() const
{
    CompileUnitNode* clone = new CompileUnitNode();
    clone->globalNs.reset(static_cast<NamespaceDefinitionNode*>(globalNs->Clone()));
    clone->lineStarts = lineStarts;
    clone->valid = valid;
    return clone;
}

void CompileUnitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
