// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.function;

import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.visitor;
import cmajor.ast.attribute;
import cmajor.ast.specifier;
import cmajor.ast.templates;
import cmajor.ast.concepts;
import cmajor.ast.parameter;
import cmajor.ast.statement;
import cmajor.ast.identifier;

namespace cmajor::ast {

FunctionNode::FunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : 
    Node(NodeType::functionNode, sourcePos_, moduleId_), specifiers(Specifiers::none), returnTypeExpr(), groupId(),
    parameters(), body(), bodySource(), programMain(false)
{
}

FunctionNode::FunctionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : 
    Node(nodeType_, sourcePos_, moduleId_), specifiers(Specifiers::none), returnTypeExpr(), groupId(),
    parameters(), body(), bodySource(), programMain(false)
{
}

FunctionNode::FunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, 
    AttributesNode* attributes_) :
    Node(NodeType::functionNode, sourcePos_, moduleId_), specifiers(specifiers_), returnTypeExpr(returnTypeExpr_), groupId(groupId_), templateParameters(), parameters(), body(), 
    bodySource(), attributes(attributes_), programMain(false)
{
    if (returnTypeExpr)
    {
        returnTypeExpr->SetParent(this);
    }
}

FunctionNode::FunctionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, 
    const std::u32string& groupId_, AttributesNode* attributes_) :
    Node(nodeType_, sourcePos_, moduleId_), specifiers(specifiers_), returnTypeExpr(returnTypeExpr_), groupId(groupId_), templateParameters(), parameters(), body(), 
    bodySource(), attributes(attributes_), programMain(false)
{
    if (returnTypeExpr)
    {
        returnTypeExpr->SetParent(this);
    }
}

Node* FunctionNode::Clone(CloneContext& cloneContext) const
{
    AttributesNode* clonedAttributes = nullptr;
    if (attributes)
    {
        clonedAttributes = static_cast<AttributesNode*>(attributes->Clone(cloneContext));
    }
    Node* clonedReturnTypeExpr = nullptr;
    if (returnTypeExpr)
    {
        clonedReturnTypeExpr = returnTypeExpr->Clone(cloneContext);
    }
    FunctionNode* clone = new FunctionNode(GetSourcePos(), ModuleId(), specifiers, clonedReturnTypeExpr, groupId, clonedAttributes);
    if (!cloneContext.InstantiateFunctionNode())
    {
        int nt = templateParameters.Count();
        for (int i = 0; i < nt; ++i)
        {
            clone->AddTemplateParameter(static_cast<TemplateParameterNode*>(templateParameters[i]->Clone(cloneContext)));
        }
    }
    int np = parameters.Count();
    for (int i = 0; i < np; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    if (body)
    {
        if (cloneContext.InstantiateClassNode())
        {
            CloneContext bodyCloneContext;
            clone->SetBodySource(static_cast<CompoundStatementNode*>(body->Clone(bodyCloneContext)));
        }
        else
        {
            clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone(cloneContext)));
        }
    }
    if (whereConstraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(whereConstraint->Clone(cloneContext)));
    }
    return clone;
}

void FunctionNode::CloneContent(FunctionNode* clone, CloneContext& cloneContext) const
{
    if (attributes)
    {
        AttributesNode* clonedAttributes = static_cast<AttributesNode*>(attributes->Clone(cloneContext));
        clone->attributes.reset(clonedAttributes);
    }
    clone->specifiers = specifiers;
    Node* clonedReturnTypeExpr = nullptr;
    if (returnTypeExpr)
    {
        clone->SetReturnTypeExpr(returnTypeExpr->Clone(cloneContext));
    }
    clone->groupId = groupId;
    int nt = templateParameters.Count();
    for (int i = 0; i < nt; ++i)
    {
        clone->AddTemplateParameter(static_cast<TemplateParameterNode*>(templateParameters[i]->Clone(cloneContext)));
    }
    int np = parameters.Count();
    for (int i = 0; i < np; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    if (body)
    {
        if (cloneContext.InstantiateClassNode())
        {
            CloneContext bodyCloneContext;
            clone->SetBodySource(static_cast<CompoundStatementNode*>(body->Clone(bodyCloneContext)));
        }
        else
        {
            clone->SetBody(static_cast<CompoundStatementNode*>(body->Clone(cloneContext)));
        }
    }
    if (whereConstraint)
    {
        clone->SetConstraint(static_cast<WhereConstraintNode*>(whereConstraint->Clone(cloneContext)));
    }
}

void FunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    bool hasAttributes = attributes != nullptr;
    writer.GetBinaryStreamWriter().Write(hasAttributes);
    if (hasAttributes)
    {
        writer.Write(attributes.get());
    }
    writer.Write(specifiers);
    bool hasReturnTypeExpr = returnTypeExpr != nullptr;
    writer.GetBinaryStreamWriter().Write(hasReturnTypeExpr);
    if (hasReturnTypeExpr)
    {
        writer.Write(returnTypeExpr.get());
    }
    writer.GetBinaryStreamWriter().Write(groupId);
    templateParameters.Write(writer);
    parameters.Write(writer);
    bool hasConstraint = whereConstraint != nullptr;
    writer.GetBinaryStreamWriter().Write(hasConstraint);
    if (hasConstraint)
    {
        writer.Write(whereConstraint.get());
    }
    bool hasBody = body != nullptr;
    writer.GetBinaryStreamWriter().Write(hasBody);
    if (hasBody)
    {
        writer.Write(body.get());
    }
    bool hasBodySource = bodySource != nullptr;
    writer.GetBinaryStreamWriter().Write(hasBodySource);
    if (hasBodySource)
    {
        writer.Write(bodySource.get());
    }
}

void FunctionNode::Read(AstReader& reader)
{
    Node::Read(reader);
    bool hasAttributes = reader.GetBinaryStreamReader().ReadBool();
    if (hasAttributes)
    {
        attributes.reset(reader.ReadAttributesNode());
    }
    specifiers = reader.ReadSpecifiers();
    bool hasReturnTypeExpr = reader.GetBinaryStreamReader().ReadBool();
    if (hasReturnTypeExpr)
    {
        returnTypeExpr.reset(reader.ReadNode());
        returnTypeExpr->SetParent(this);
    }
    groupId = reader.GetBinaryStreamReader().ReadUtf32String();
    templateParameters.Read(reader);
    templateParameters.SetParent(this);
    parameters.Read(reader);
    parameters.SetParent(this);
    bool hasConstraint = reader.GetBinaryStreamReader().ReadBool();
    if (hasConstraint)
    {
        whereConstraint.reset(reader.ReadWhereConstraintNode());
    }
    bool hasBody = reader.GetBinaryStreamReader().ReadBool();
    if (hasBody)
    {
        body.reset(reader.ReadCompoundStatementNode());
        body->SetParent(this);
    }
    bool hasBodySource = reader.GetBinaryStreamReader().ReadBool();
    if (hasBodySource)
    {
        bodySource.reset(reader.ReadCompoundStatementNode());
        bodySource->SetParent(this);
    }
}

void FunctionNode::AddTemplateParameter(TemplateParameterNode* templateParameter)
{
    templateParameter->SetParent(this);
    templateParameters.Add(templateParameter);
}

void FunctionNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

void FunctionNode::SwitchToBody()
{
    if (bodySource && !body)
    {
        SetBody(bodySource.release());
    }
}

void FunctionNode::SetReturnTypeExpr(Node* returnTypeExpr_)
{
    returnTypeExpr.reset(returnTypeExpr_);
    returnTypeExpr->SetParent(returnTypeExpr_);
}

void FunctionNode::SetSpecifiers(Specifiers specifiers_)
{
    specifiers = specifiers_;
}

void FunctionNode::SetConstraint(WhereConstraintNode* whereConstraint_)
{
    whereConstraint.reset(whereConstraint_);
    whereConstraint->SetParent(this);
}

void FunctionNode::SetBody(CompoundStatementNode* body_)
{
    body.reset(body_);
    body->SetParent(this);
}

void FunctionNode::SetBodySource(CompoundStatementNode* bodySource_)
{
    bodySource.reset(bodySource_);
    bodySource->SetParent(this);
}

FunctionPtrNode::FunctionPtrNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : 
    Node(NodeType::functionPtrNode, sourcePos_, moduleId_), boundExpression(nullptr)
{
}

Node* FunctionPtrNode::Clone(CloneContext& cloneContext) const
{
    throw std::runtime_error("internal error: function ptr node cannot be cloned");
}

void FunctionPtrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::ast
