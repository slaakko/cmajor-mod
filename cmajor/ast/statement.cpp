// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.statement;

import std.core;
import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;

namespace cmajor::ast {

LabelNode::LabelNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::labelNode, sourcePos_, moduleId_)
{
}

LabelNode::LabelNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& label_) : Node(NodeType::labelNode, sourcePos_, moduleId_), label(label_)
{
}

Node* LabelNode::Clone(CloneContext& cloneContext) const
{
    LabelNode* clone = new LabelNode(GetSourcePos(), ModuleId(), label);
    return clone;
}

void LabelNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LabelNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(label);
}

void LabelNode::Read(AstReader& reader)
{
    Node::Read(reader);
    label = reader.GetBinaryStreamReader().ReadUtf32String();
}

StatementNode::StatementNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(nodeType_, sourcePos_, moduleId_)
{
}

void StatementNode::Write(AstWriter& writer)
{
    Node::Write(writer);
}

void StatementNode::Read(AstReader& reader)
{
    Node::Read(reader);
}

LabeledStatementNode::LabeledStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::labeledStatementNode, sourcePos_, moduleId_)
{
}

LabeledStatementNode::LabeledStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, StatementNode* stmt_) :
    StatementNode(NodeType::labeledStatementNode, sourcePos_, moduleId_), stmt(stmt_)
{
    stmt->SetParent(this);
}

Node* LabeledStatementNode::Clone(CloneContext& cloneContext) const
{
    LabeledStatementNode* clone = new LabeledStatementNode(GetSourcePos(), ModuleId(), static_cast<StatementNode*>(stmt->Clone(cloneContext)));
    clone->SetLabelNode(static_cast<LabelNode*>(labelNode->Clone(cloneContext)));
    return clone;
}

void LabeledStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LabeledStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(labelNode.get());
    writer.Write(stmt.get());
}

void LabeledStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    labelNode.reset(reader.ReadLabelNode());
    labelNode->SetParent(this);
    stmt.reset(reader.ReadStatementNode());
    stmt->SetParent(this);
}

void LabeledStatementNode::SetLabelNode(LabelNode* labelNode_)
{
    labelNode.reset(labelNode_);
    labelNode->SetParent(this);
}

SyncStatementNode::SyncStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::syncStatementNode, sourcePos_, moduleId_)
{
}

Node* SyncStatementNode::Clone(CloneContext& cloneContext) const
{
    return new SyncStatementNode(GetSourcePos(), ModuleId());
}

void SyncStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CompoundStatementNode::CompoundStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::compoundStatementNode, sourcePos_, moduleId_), statements(), beginBraceSourcePos(), endBraceSourcePos(), tracerInserted(false)
{
}

Node* CompoundStatementNode::Clone(CloneContext& cloneContext) const
{
    CompoundStatementNode* clone = new CompoundStatementNode(GetSourcePos(), ModuleId());
    int n = statements.Count();
    for (int i = 0; i < n; ++i)
    {
        StatementNode* statement = statements[i];
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone(cloneContext)));
    }
    clone->beginBraceSourcePos = beginBraceSourcePos;
    clone->endBraceSourcePos = endBraceSourcePos;
    return clone;
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompoundStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    statements.Write(writer);
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(beginBraceSourcePos, convertExternal);
    //writer.Write(endBraceSourcePos, convertExternal);
}

void CompoundStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    statements.Read(reader);
    statements.SetParent(this);
    beginBraceSourcePos = reader.ReadSourcePos();
    endBraceSourcePos = reader.ReadSourcePos();
}

void CompoundStatementNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.Add(statement);
}

int CompoundStatementNode::Level() const
{
    int level = 0;
    const Node* parent = Parent();
    if (parent)
    {
        if (parent->GetNodeType() == NodeType::compoundStatementNode)
        {
            return static_cast<const CompoundStatementNode*>(parent)->Level() + 1;
        }
    }
    return level;
}

ReturnStatementNode::ReturnStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::returnStatementNode, sourcePos_, moduleId_), expression()
{
}

ReturnStatementNode::ReturnStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* expression_) :
    StatementNode(NodeType::returnStatementNode, sourcePos_, moduleId_), expression(expression_)
{
    if (expression)
    {
        expression->SetParent(this);
    }
}

Node* ReturnStatementNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedExpression = nullptr;
    if (expression)
    {
        clonedExpression = expression->Clone(cloneContext);
    }
    ReturnStatementNode* clone = new ReturnStatementNode(GetSourcePos(), ModuleId(), clonedExpression);
    return clone;
}

void ReturnStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ReturnStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    bool hasExpression = expression != nullptr;
    writer.GetBinaryStreamWriter().Write(hasExpression);
    if (hasExpression)
    {
        writer.Write(expression.get());
    }
}

void ReturnStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    bool hasExpression = reader.GetBinaryStreamReader().ReadBool();
    if (hasExpression)
    {
        expression.reset(reader.ReadNode());
        expression->SetParent(this);
    }
}

IfStatementNode::IfStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::ifStatementNode, sourcePos_, moduleId_), condition(), thenS(), elseS()
{
}

IfStatementNode::IfStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* condition_, StatementNode* thenS_, StatementNode* elseS_) :
    StatementNode(NodeType::ifStatementNode, sourcePos_, moduleId_), condition(condition_), thenS(thenS_), elseS(elseS_)
{
    condition->SetParent(this);
    thenS->SetParent(this);
    if (elseS)
    {
        elseS->SetParent(this);
    }
}

Node* IfStatementNode::Clone(CloneContext& cloneContext) const
{
    StatementNode* clonedElseS = nullptr;
    if (elseS)
    {
        clonedElseS = static_cast<StatementNode*>(elseS->Clone(cloneContext));
    }
    IfStatementNode* clone = new IfStatementNode(GetSourcePos(), ModuleId(), condition->Clone(cloneContext), static_cast<StatementNode*>(thenS->Clone(cloneContext)), clonedElseS);
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    clone->SetElseSourcePos(elseSourcePos);
    return clone;
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IfStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(condition.get());
    writer.Write(thenS.get());
    bool hasElseS = elseS != nullptr;
    writer.GetBinaryStreamWriter().Write(hasElseS);
    if (hasElseS)
    {
        writer.Write(elseS.get());
    }
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
    //writer.Write(elseSourcePos, convertExternal);
}

void IfStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    thenS.reset(reader.ReadStatementNode());
    thenS->SetParent(this);
    bool hasElseS = reader.GetBinaryStreamReader().ReadBool();
    if (hasElseS)
    {
        elseS.reset(reader.ReadStatementNode());
        elseS->SetParent(this);
    }
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
    elseSourcePos = reader.ReadSourcePos();
}

WhileStatementNode::WhileStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::whileStatementNode, sourcePos_, moduleId_), condition(), statement()
{
}

WhileStatementNode::WhileStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* condition_, StatementNode* statement_) :
    StatementNode(NodeType::whileStatementNode, sourcePos_, moduleId_), condition(condition_), statement(statement_)
{
    condition->SetParent(this);
    statement->SetParent(this);
}

Node* WhileStatementNode::Clone(CloneContext& cloneContext) const
{
    WhileStatementNode* clone = new WhileStatementNode(GetSourcePos(), ModuleId(), condition->Clone(cloneContext), static_cast<StatementNode*>(statement->Clone(cloneContext)));
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    return clone;
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WhileStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(condition.get());
    writer.Write(statement.get());
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
}

void WhileStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    statement.reset(reader.ReadStatementNode());
    statement->SetParent(this);
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
}

DoStatementNode::DoStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::doStatementNode, sourcePos_, moduleId_), statement(), condition()
{
}

DoStatementNode::DoStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, StatementNode* statement_, Node* condition_) :
    StatementNode(NodeType::doStatementNode, sourcePos_, moduleId_), statement(statement_), condition(condition_)
{
    statement->SetParent(this);
    condition->SetParent(this);
}

Node* DoStatementNode::Clone(CloneContext& cloneContext) const
{
    DoStatementNode* clone = new DoStatementNode(GetSourcePos(), ModuleId(), static_cast<StatementNode*>(statement->Clone(cloneContext)), condition->Clone(cloneContext));
    clone->SetWhileSourcePos(whileSourcePos);
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    return clone;
}

void DoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DoStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(statement.get());
    writer.Write(condition.get());
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(whileSourcePos, convertExternal);
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
}

void DoStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    statement.reset(reader.ReadStatementNode());
    statement->SetParent(this);
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    whileSourcePos = reader.ReadSourcePos();
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
}

ForStatementNode::ForStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::forStatementNode, sourcePos_, moduleId_), initS(), condition(), loopS(), actionS()
{
}

ForStatementNode::ForStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, StatementNode* initS_, Node* condition_, StatementNode* loopS_, StatementNode* actionS_) :
    StatementNode(NodeType::forStatementNode, sourcePos_, moduleId_), initS(initS_), condition(condition_), loopS(loopS_), actionS(actionS_)
{
    initS->SetParent(this);
    if (condition)
    {
        condition->SetParent(this);
    }
    if (loopS)
    {
        loopS->SetParent(this);
    }
    if (actionS)
    {
        actionS->SetParent(this);
    }
}

Node* ForStatementNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedCondition = nullptr;
    if (condition)
    {
        clonedCondition = condition->Clone(cloneContext);
    }
    ForStatementNode* clone = new ForStatementNode(GetSourcePos(), ModuleId(), static_cast<StatementNode*>(initS->Clone(cloneContext)), clonedCondition, static_cast<StatementNode*>(loopS->Clone(cloneContext)),
        static_cast<StatementNode*>(actionS->Clone(cloneContext)));
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    return clone;
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(initS.get());
    bool hasCondition = condition != nullptr;
    writer.GetBinaryStreamWriter().Write(hasCondition);
    if (hasCondition)
    {
        writer.Write(condition.get());
    }
    writer.Write(loopS.get());
    writer.Write(actionS.get());
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
}

void ForStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    initS.reset(reader.ReadStatementNode());
    initS->SetParent(this);
    bool hasCondition = reader.GetBinaryStreamReader().ReadBool();
    if (hasCondition)
    {
        condition.reset(reader.ReadNode());
        condition->SetParent(this);
    }
    loopS.reset(reader.ReadStatementNode());
    loopS->SetParent(this);
    actionS.reset(reader.ReadStatementNode());
    actionS->SetParent(this);
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
}

BreakStatementNode::BreakStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::breakStatementNode, sourcePos_, moduleId_)
{
}

Node* BreakStatementNode::Clone(CloneContext& cloneContext) const
{
    BreakStatementNode* clone = new BreakStatementNode(GetSourcePos(), ModuleId());
    return clone;
}

void BreakStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ContinueStatementNode::ContinueStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::continueStatementNode, sourcePos_, moduleId_)
{
}

Node* ContinueStatementNode::Clone(CloneContext& cloneContext) const
{
    ContinueStatementNode* clone = new ContinueStatementNode(GetSourcePos(), ModuleId());
    return clone;
}

void ContinueStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GotoStatementNode::GotoStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::gotoStatementNode, sourcePos_, moduleId_)
{
}

GotoStatementNode::GotoStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& target_) :
    StatementNode(NodeType::gotoStatementNode, sourcePos_, moduleId_), target(target_)
{
}

Node* GotoStatementNode::Clone(CloneContext& cloneContext) const
{
    GotoStatementNode* clone = new GotoStatementNode(GetSourcePos(), ModuleId(), target);
    return clone;
}

void GotoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GotoStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(target);
}

void GotoStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    target = reader.GetBinaryStreamReader().ReadUtf32String();
}

ConstructionStatementNode::ConstructionStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::constructionStatementNode, sourcePos_, moduleId_), typeExpr(), id(), arguments(), assignment(false), empty(false)
{
}

ConstructionStatementNode::ConstructionStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_) :
    StatementNode(NodeType::constructionStatementNode, sourcePos_, moduleId_), typeExpr(typeExpr_), id(id_), arguments(), assignment(false), empty(false)
{
    typeExpr->SetParent(this);
    if (id)
    {
        id->SetParent(this);
    }
}

Node* ConstructionStatementNode::Clone(CloneContext& cloneContext) const
{
    ConstructionStatementNode* clone = new ConstructionStatementNode(GetSourcePos(), ModuleId(), typeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddArgument(arguments[i]->Clone(cloneContext));
    }
    if (assignment) clone->SetAssignment();
    if (empty) clone->SetEmpty();
    return clone;
}

void ConstructionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructionStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(typeExpr.get());
    writer.Write(id.get());
    arguments.Write(writer);
    writer.GetBinaryStreamWriter().Write(assignment);
    writer.GetBinaryStreamWriter().Write(empty);
}

void ConstructionStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    arguments.Read(reader);
    arguments.SetParent(this);
    assignment = reader.GetBinaryStreamReader().ReadBool();
    empty = reader.GetBinaryStreamReader().ReadBool();
}

void ConstructionStatementNode::AddArgument(Node* argument)
{
    if (argument)
    {
        argument->SetParent(this);
        arguments.Add(argument);
    }
}

DeleteStatementNode::DeleteStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::deleteStatementNode, sourcePos_, moduleId_), expression()
{
}

DeleteStatementNode::DeleteStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* expression_) :
    StatementNode(NodeType::deleteStatementNode, sourcePos_, moduleId_), expression(expression_)
{
    expression->SetParent(this);
}

Node* DeleteStatementNode::Clone(CloneContext& cloneContext) const
{
    DeleteStatementNode* clone = new DeleteStatementNode(GetSourcePos(), ModuleId(), expression->Clone(cloneContext));
    return clone;
}

void DeleteStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeleteStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(expression.get());
}

void DeleteStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

DestroyStatementNode::DestroyStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::destroyStatementNode, sourcePos_, moduleId_), expression()
{
}

DestroyStatementNode::DestroyStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* expression_) :
    StatementNode(NodeType::destroyStatementNode, sourcePos_, moduleId_), expression(expression_)
{
    expression->SetParent(this);
}

Node* DestroyStatementNode::Clone(CloneContext& cloneContext) const
{
    DestroyStatementNode* clone = new DestroyStatementNode(GetSourcePos(), ModuleId(), expression->Clone(cloneContext));
    return clone;
}

void DestroyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DestroyStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(expression.get());
}

void DestroyStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

AssignmentStatementNode::AssignmentStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::assignmentStatementNode, sourcePos_, moduleId_), targetExpr(), sourceExpr()
{
}

AssignmentStatementNode::AssignmentStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* targetExpr_, Node* sourceExpr_) :
    StatementNode(NodeType::assignmentStatementNode, sourcePos_, moduleId_), targetExpr(targetExpr_), sourceExpr(sourceExpr_)
{
    targetExpr->SetParent(this);
    sourceExpr->SetParent(this);
}

Node* AssignmentStatementNode::Clone(CloneContext& cloneContext) const
{
    AssignmentStatementNode* clone = new AssignmentStatementNode(GetSourcePos(), ModuleId(), targetExpr->Clone(cloneContext), sourceExpr->Clone(cloneContext));
    return clone;
}

void AssignmentStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AssignmentStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(targetExpr.get());
    writer.Write(sourceExpr.get());
}

void AssignmentStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    targetExpr.reset(reader.ReadNode());
    targetExpr->SetParent(this);
    sourceExpr.reset(reader.ReadNode());
    sourceExpr->SetParent(this);
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::expressionStatementNode, sourcePos_, moduleId_), expression()
{
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* expression_) :
    StatementNode(NodeType::expressionStatementNode, sourcePos_, moduleId_), expression(expression_)
{
    expression->SetParent(this);
}

Node* ExpressionStatementNode::Clone(CloneContext& cloneContext) const
{
    ExpressionStatementNode* clone = new ExpressionStatementNode(GetSourcePos(), ModuleId(), expression->Clone(cloneContext));
    return clone;
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExpressionStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(expression.get());
}

void ExpressionStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

EmptyStatementNode::EmptyStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::emptyStatementNode, sourcePos_, moduleId_)
{
}

Node* EmptyStatementNode::Clone(CloneContext& cloneContext) const
{
    EmptyStatementNode* clone = new EmptyStatementNode(GetSourcePos(), ModuleId());
    return clone;
}

void EmptyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RangeForStatementNode::RangeForStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::rangeForStatementNode, sourcePos_, moduleId_), typeExpr(), id(), container(), action()
{
}

RangeForStatementNode::RangeForStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_, Node* container_, StatementNode* action_) :
    StatementNode(NodeType::rangeForStatementNode, sourcePos_, moduleId_), typeExpr(typeExpr_), id(id_), container(container_), action(action_)
{
    typeExpr->SetParent(this);
    id->SetParent(this);
    container->SetParent(this);
    action->SetParent(this);
}

Node* RangeForStatementNode::Clone(CloneContext& cloneContext) const
{
    RangeForStatementNode* clone = new RangeForStatementNode(GetSourcePos(), ModuleId(), typeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)), container->Clone(cloneContext),
        static_cast<StatementNode*>(action->Clone(cloneContext)));
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    clone->SetColonSourcePos(colonSourcePos);
    return clone;
}

void RangeForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RangeForStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(typeExpr.get());
    writer.Write(id.get());
    writer.Write(container.get());
    writer.Write(action.get());
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
    //writer.Write(colonSourcePos, convertExternal);
}

void RangeForStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    container.reset(reader.ReadNode());
    container->SetParent(this);
    action.reset(reader.ReadStatementNode());
    action->SetParent(this);
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
    colonSourcePos = reader.ReadSourcePos();
}

SwitchStatementNode::SwitchStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::switchStatementNode, sourcePos_, moduleId_), condition(), cases(), defaultS()
{
}

SwitchStatementNode::SwitchStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* condition_) :
    StatementNode(NodeType::switchStatementNode, sourcePos_, moduleId_), condition(condition_), cases(), defaultS()
{
    condition->SetParent(this);
}

Node* SwitchStatementNode::Clone(CloneContext& cloneContext) const
{
    SwitchStatementNode* clone = new SwitchStatementNode(GetSourcePos(), ModuleId(), condition->Clone(cloneContext));
    int n = cases.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddCase(static_cast<CaseStatementNode*>(cases[i]->Clone(cloneContext)));
    }
    if (defaultS)
    {
        clone->SetDefault(static_cast<DefaultStatementNode*>(defaultS->Clone(cloneContext)));
    }
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    clone->SetBeginBraceSourcePos(beginBraceSourcePos);
    clone->SetEndBraceSourcePos(endBraceSourcePos);
    return clone;
}

void SwitchStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SwitchStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(condition.get());
    cases.Write(writer);
    bool hasDefault = defaultS != nullptr;
    writer.GetBinaryStreamWriter().Write(hasDefault);
    if (hasDefault)
    {
        writer.Write(defaultS.get());
    }
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
    //writer.Write(beginBraceSourcePos, convertExternal);
    //writer.Write(endBraceSourcePos, convertExternal);
}

void SwitchStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    cases.Read(reader);
    cases.SetParent(this);
    bool hasDefault = reader.GetBinaryStreamReader().ReadBool();
    if (hasDefault)
    {
        defaultS.reset(reader.ReadDefaultStatementNode());
        defaultS->SetParent(this);
    }
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
    beginBraceSourcePos = reader.ReadSourcePos();
    endBraceSourcePos = reader.ReadSourcePos();
}

void SwitchStatementNode::AddCase(CaseStatementNode* caseS)
{
    caseS->SetParent(this);
    cases.Add(caseS);
}

void SwitchStatementNode::SetDefault(DefaultStatementNode* defaultS_)
{
    defaultS.reset(defaultS_);
    defaultS->SetParent(this);
}

CaseStatementNode::CaseStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::caseStatementNode, sourcePos_, moduleId_), caseExprs(), statements()
{
}

Node* CaseStatementNode::Clone(CloneContext& cloneContext) const
{
    CaseStatementNode* clone = new CaseStatementNode(GetSourcePos(), ModuleId());
    int ne = caseExprs.Count();
    for (int i = 0; i < ne; ++i)
    {
        clone->AddCaseExpr(caseExprs[i]->Clone(cloneContext));
    }
    int ns = statements.Count();
    for (int i = 0; i < ns; ++i)
    {
        clone->AddStatement(static_cast<StatementNode*>(statements[i]->Clone(cloneContext)));
    }
    clone->caseSourcePoses = caseSourcePoses;
    return clone;
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CaseStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    caseExprs.Write(writer);
    statements.Write(writer);
    uint32_t n = static_cast<uint32_t>(caseSourcePoses.size());
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //for (uint32_t i = 0u; i < n; ++i)
    //{
    //    writer.Write(caseSpans[i], convertExternal);
    //}
}

void CaseStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    caseExprs.Read(reader);
    caseExprs.SetParent(this);
    statements.Read(reader);
    statements.SetParent(this);
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0u; i < n; ++i)
    {
        caseSourcePoses.push_back(reader.ReadSourcePos());
    }
}

void CaseStatementNode::AddCaseExpr(Node* caseExpr)
{
    caseExpr->SetParent(this);
    caseExprs.Add(caseExpr);
}

void CaseStatementNode::AddCaseSourcePos(const soul::ast::SourcePos& caseSourcePos)
{
    caseSourcePoses.push_back(caseSourcePos);
}

void CaseStatementNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.Add(statement);
}

DefaultStatementNode::DefaultStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::defaultStatementNode, sourcePos_, moduleId_), statements()
{
}

Node* DefaultStatementNode::Clone(CloneContext& cloneContext) const
{
    DefaultStatementNode* clone = new DefaultStatementNode(GetSourcePos(), ModuleId());
    int n = statements.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddStatement(static_cast<StatementNode*>(statements[i]->Clone(cloneContext)));
    }
    return clone;
}

void DefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DefaultStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    statements.Write(writer);
}

void DefaultStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    statements.Read(reader);
    statements.SetParent(this);
}

void DefaultStatementNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.Add(statement);
}

GotoCaseStatementNode::GotoCaseStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::gotoCaseStatementNode, sourcePos_, moduleId_), caseExpr()
{
}

GotoCaseStatementNode::GotoCaseStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* caseExpr_) :
    StatementNode(NodeType::gotoCaseStatementNode, sourcePos_, moduleId_), caseExpr(caseExpr_)
{
    caseExpr->SetParent(this);
}

Node* GotoCaseStatementNode::Clone(CloneContext& cloneContext) const
{
    GotoCaseStatementNode* clone = new GotoCaseStatementNode(GetSourcePos(), ModuleId(), caseExpr->Clone(cloneContext));
    return clone;
}

void GotoCaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GotoCaseStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(caseExpr.get());
}

void GotoCaseStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    caseExpr.reset(reader.ReadNode());
}

GotoDefaultStatementNode::GotoDefaultStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : StatementNode(NodeType::gotoDefaultStatementNode, sourcePos_, moduleId_)
{
}

Node* GotoDefaultStatementNode::Clone(CloneContext& cloneContext) const
{
    GotoDefaultStatementNode* clone = new GotoDefaultStatementNode(GetSourcePos(), ModuleId());
    return clone;
}

void GotoDefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThrowStatementNode::ThrowStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::throwStatementNode, sourcePos_, moduleId_), expression()
{
}

ThrowStatementNode::ThrowStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* expression_) :
    StatementNode(NodeType::throwStatementNode, sourcePos_, moduleId_), expression(expression_)
{
    if (expression)
    {
        expression->SetParent(this);
    }
}

Node* ThrowStatementNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedExpression = nullptr;
    if (expression)
    {
        clonedExpression = expression->Clone(cloneContext);
    }
    ThrowStatementNode* clone = new ThrowStatementNode(GetSourcePos(), ModuleId(), clonedExpression);
    return clone;
}

void ThrowStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThrowStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    bool hasExpression = expression != nullptr;
    writer.GetBinaryStreamWriter().Write(hasExpression);
    if (hasExpression)
    {
        writer.Write(expression.get());
    }
}

void ThrowStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    bool hasExpression = reader.GetBinaryStreamReader().ReadBool();
    if (hasExpression)
    {
        expression.reset(reader.ReadNode());
        expression->SetParent(this);
    }
}

CatchNode::CatchNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::catchNode, sourcePos_, moduleId_), typeExpr(), id(), catchBlock()
{
}

CatchNode::CatchNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* typeExpr_, IdentifierNode* id_, CompoundStatementNode* catchBlock_) :
    Node(NodeType::catchNode, sourcePos_, moduleId_), typeExpr(typeExpr_), id(id_), catchBlock(catchBlock_)
{
    typeExpr->SetParent(this);
    if (id)
    {
        id->SetParent(this);
    }
    catchBlock->SetParent(this);
}

Node* CatchNode::Clone(CloneContext& cloneContext) const
{
    IdentifierNode* clonedId = nullptr;
    if (id)
    {
        clonedId = static_cast<IdentifierNode*>(id->Clone(cloneContext));
    }
    CatchNode* clone = new CatchNode(GetSourcePos(), ModuleId(), typeExpr->Clone(cloneContext), clonedId, static_cast<CompoundStatementNode*>(catchBlock->Clone(cloneContext)));
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    return clone;
}

void CatchNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CatchNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(typeExpr.get());
    bool hasId = id != nullptr;
    writer.GetBinaryStreamWriter().Write(hasId);
    if (hasId)
    {
        writer.Write(id.get());
    }
    writer.Write(catchBlock.get());
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
}

void CatchNode::Read(AstReader& reader)
{
    Node::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    bool hasId = reader.GetBinaryStreamReader().ReadBool();
    if (hasId)
    {
        id.reset(reader.ReadIdentifierNode());
        id->SetParent(this);
    }
    catchBlock.reset(reader.ReadCompoundStatementNode());
    catchBlock->SetParent(this);
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
}

TryStatementNode::TryStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::tryStatementNode, sourcePos_, moduleId_), tryBlock(), catches()
{
}

TryStatementNode::TryStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, CompoundStatementNode* tryBlock_) :
    StatementNode(NodeType::tryStatementNode, sourcePos_, moduleId_), tryBlock(tryBlock_), catches()
{
    tryBlock->SetParent(this);
}

Node* TryStatementNode::Clone(CloneContext& cloneContext) const
{
    TryStatementNode* clone = new TryStatementNode(GetSourcePos(), ModuleId(), static_cast<CompoundStatementNode*>(tryBlock->Clone(cloneContext)));
    int n = catches.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddCatch(static_cast<CatchNode*>(catches[i]->Clone(cloneContext)));
    }
    return clone;
}

void TryStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TryStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(tryBlock.get());
    catches.Write(writer);
}

void TryStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    tryBlock.reset(reader.ReadCompoundStatementNode());
    tryBlock->SetParent(this);
    catches.Read(reader);
    catches.SetParent(this);
}

void TryStatementNode::AddCatch(CatchNode* catch_)
{
    catch_->SetParent(this);
    catches.Add(catch_);
}

AssertStatementNode::AssertStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::assertStatementNode, sourcePos_, moduleId_), assertExpr()
{
}

AssertStatementNode::AssertStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Node* assertExpr_) :
    StatementNode(NodeType::assertStatementNode, sourcePos_, moduleId_), assertExpr(assertExpr_)
{
    assertExpr->SetParent(this);
}

Node* AssertStatementNode::Clone(CloneContext& cloneContext) const
{
    AssertStatementNode* clone = new AssertStatementNode(GetSourcePos(), ModuleId(), assertExpr->Clone(cloneContext));
    return clone;
}

void AssertStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AssertStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(assertExpr.get());
}

void AssertStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    assertExpr.reset(reader.ReadNode());
    assertExpr->SetParent(this);
}

ConditionalCompilationExpressionNode::ConditionalCompilationExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(nodeType_, sourcePos_, moduleId_)
{
}

ConditionalCompilationBinaryExpressionNode::ConditionalCompilationBinaryExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationExpressionNode(nodeType_, sourcePos_, moduleId_)
{
}

ConditionalCompilationBinaryExpressionNode::ConditionalCompilationBinaryExpressionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_) :
    ConditionalCompilationExpressionNode(nodeType_, sourcePos_, moduleId_), left(left_), right(right_)
{
    left->SetParent(this);
    right->SetParent(this);
}

void ConditionalCompilationBinaryExpressionNode::Write(AstWriter& writer)
{
    ConditionalCompilationExpressionNode::Write(writer);
    writer.Write(left.get());
    writer.Write(right.get());
}

void ConditionalCompilationBinaryExpressionNode::Read(AstReader& reader)
{
    ConditionalCompilationExpressionNode::Read(reader);
    left.reset(reader.ReadConditionalCompilationExpressionNode());
    left->SetParent(this);
    right.reset(reader.ReadConditionalCompilationExpressionNode());
    right->SetParent(this);
}

ConditionalCompilationDisjunctionNode::ConditionalCompilationDisjunctionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationBinaryExpressionNode(NodeType::conditionalCompilationDisjunctionNode, sourcePos_, moduleId_)
{
}

ConditionalCompilationDisjunctionNode::ConditionalCompilationDisjunctionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_,
    ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_) :
    ConditionalCompilationBinaryExpressionNode(NodeType::conditionalCompilationDisjunctionNode, sourcePos_, moduleId_, left_, right_)
{
}

Node* ConditionalCompilationDisjunctionNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationDisjunctionNode* clone = new ConditionalCompilationDisjunctionNode(GetSourcePos(), ModuleId(), static_cast<ConditionalCompilationExpressionNode*>(Left()->Clone(cloneContext)), static_cast<ConditionalCompilationExpressionNode*>(Right()->Clone(cloneContext)));
    return clone;
}

void ConditionalCompilationDisjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConditionalCompilationConjunctionNode::ConditionalCompilationConjunctionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationBinaryExpressionNode(NodeType::conditionalCompilationConjunctionNode, sourcePos_, moduleId_)
{
}

ConditionalCompilationConjunctionNode::ConditionalCompilationConjunctionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_,
    ConditionalCompilationExpressionNode* left_, ConditionalCompilationExpressionNode* right_) :
    ConditionalCompilationBinaryExpressionNode(NodeType::conditionalCompilationConjunctionNode, sourcePos_, moduleId_, left_, right_)
{
}

Node* ConditionalCompilationConjunctionNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationConjunctionNode* clone = new ConditionalCompilationConjunctionNode(GetSourcePos(), ModuleId(), static_cast<ConditionalCompilationExpressionNode*>(Left()->Clone(cloneContext)), static_cast<ConditionalCompilationExpressionNode*>(Right()->Clone(cloneContext)));
    return clone;
}

void ConditionalCompilationConjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConditionalCompilationNotNode::ConditionalCompilationNotNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationExpressionNode(NodeType::conditionalCompilationNotNode, sourcePos_, moduleId_)
{
}

ConditionalCompilationNotNode::ConditionalCompilationNotNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_) :
    ConditionalCompilationExpressionNode(NodeType::conditionalCompilationNotNode, sourcePos_, moduleId_), expr(expr_)
{
    expr->SetParent(this);
}

Node* ConditionalCompilationNotNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationNotNode* clone = new ConditionalCompilationNotNode(GetSourcePos(), ModuleId(), static_cast<ConditionalCompilationExpressionNode*>(expr->Clone(cloneContext)));
    return clone;
}

void ConditionalCompilationNotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalCompilationNotNode::Write(AstWriter& writer)
{
    ConditionalCompilationExpressionNode::Write(writer);
    writer.Write(expr.get());
}

void ConditionalCompilationNotNode::Read(AstReader& reader)
{
    ConditionalCompilationExpressionNode::Read(reader);
    expr.reset(reader.ReadConditionalCompilationExpressionNode());
    expr->SetParent(this);
}

ConditionalCompilationPrimaryNode::ConditionalCompilationPrimaryNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationExpressionNode(NodeType::conditionalCompilationPrimaryNode, sourcePos_, moduleId_)
{
}

ConditionalCompilationPrimaryNode::ConditionalCompilationPrimaryNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, const std::u32string& symbol_) :
    ConditionalCompilationExpressionNode(NodeType::conditionalCompilationPrimaryNode, sourcePos_, moduleId_), symbol(symbol_)
{
}

Node* ConditionalCompilationPrimaryNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationPrimaryNode* clone = new ConditionalCompilationPrimaryNode(GetSourcePos(), ModuleId(), symbol);
    return clone;
}

void ConditionalCompilationPrimaryNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalCompilationPrimaryNode::Write(AstWriter& writer)
{
    ConditionalCompilationExpressionNode::Write(writer);
    writer.GetBinaryStreamWriter().Write(symbol);
}

void ConditionalCompilationPrimaryNode::Read(AstReader& reader)
{
    ConditionalCompilationExpressionNode::Read(reader);
    symbol = reader.GetBinaryStreamReader().ReadUtf32String();
}

ParenthesizedConditionalCompilationExpressionNode::ParenthesizedConditionalCompilationExpressionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    ConditionalCompilationExpressionNode(NodeType::parenthesizedCondCompExpressionNode, sourcePos_, moduleId_)
{
}

ParenthesizedConditionalCompilationExpressionNode::ParenthesizedConditionalCompilationExpressionNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_) :
    ConditionalCompilationExpressionNode(NodeType::parenthesizedCondCompExpressionNode, sourcePos_, moduleId_), expr(expr_)
{
}

Node* ParenthesizedConditionalCompilationExpressionNode::Clone(CloneContext& cloneContext) const
{
    ParenthesizedConditionalCompilationExpressionNode* clone = new ParenthesizedConditionalCompilationExpressionNode(GetSourcePos(), ModuleId(), static_cast<ConditionalCompilationExpressionNode*>(expr->Clone(cloneContext)));
    return clone;
}

void ParenthesizedConditionalCompilationExpressionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedConditionalCompilationExpressionNode::Write(AstWriter& writer)
{
    ConditionalCompilationExpressionNode::Write(writer);
    writer.Write(expr.get());
}

void ParenthesizedConditionalCompilationExpressionNode::Read(AstReader& reader)
{
    ConditionalCompilationExpressionNode::Read(reader);
    expr.reset(reader.ReadConditionalCompilationExpressionNode());
    expr->SetParent(this);
}

ConditionalCompilationPartNode::ConditionalCompilationPartNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::conditionalCompilationPartNode, sourcePos_, moduleId_)
{
}

ConditionalCompilationPartNode::ConditionalCompilationPartNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* expr_) :
    Node(NodeType::conditionalCompilationPartNode, sourcePos_, moduleId_), expr(expr_)
{
    if (expr)
    {
        expr->SetParent(this);
    }
}

void ConditionalCompilationPartNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.Add(statement);
}

Node* ConditionalCompilationPartNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationExpressionNode* clonedIfExpr = nullptr;
    if (expr)
    {
        clonedIfExpr = static_cast<ConditionalCompilationExpressionNode*>(expr->Clone(cloneContext));
    }
    ConditionalCompilationPartNode* clone = new ConditionalCompilationPartNode(GetSourcePos(), ModuleId(), clonedIfExpr);
    int n = statements.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddStatement(static_cast<StatementNode*>(statements[i]->Clone(cloneContext)));
    }
    clone->SetKeywordSourcePos(keywordSourcePos);
    clone->SetLeftParenSourcePos(leftParenSourcePos);
    clone->SetRightParenSourcePos(rightParenSourcePos);
    return clone;
}

void ConditionalCompilationPartNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalCompilationPartNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    bool hasExpr = expr != nullptr;
    writer.GetBinaryStreamWriter().Write(hasExpr);
    if (hasExpr)
    {
        writer.Write(expr.get());
    }
    statements.Write(writer);
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(keywordSourcePos, convertExternal);
    //writer.Write(leftParenSourcePos, convertExternal);
    //writer.Write(rightParenSourcePos, convertExternal);
}

void ConditionalCompilationPartNode::Read(AstReader& reader)
{
    Node::Read(reader);
    bool hasExpr = reader.GetBinaryStreamReader().ReadBool();
    if (hasExpr)
    {
        expr.reset(reader.ReadConditionalCompilationExpressionNode());
        expr->SetParent(this);
    }
    statements.Read(reader);
    statements.SetParent(this);
    keywordSourcePos = reader.ReadSourcePos();
    leftParenSourcePos = reader.ReadSourcePos();
    rightParenSourcePos = reader.ReadSourcePos();
}

ConditionalCompilationStatementNode::ConditionalCompilationStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    StatementNode(NodeType::conditionalCompilationStatementNode, sourcePos_, moduleId_), ifPart(nullptr)
{
}

ConditionalCompilationStatementNode::ConditionalCompilationStatementNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* ifExpr_) :
    StatementNode(NodeType::conditionalCompilationStatementNode, sourcePos_, moduleId_), ifPart(new ConditionalCompilationPartNode(sourcePos_, moduleId_, ifExpr_))
{
}

void ConditionalCompilationStatementNode::AddIfStatement(StatementNode* statement)
{
    ifPart->AddStatement(statement);
}

void ConditionalCompilationStatementNode::AddElifExpr(const soul::ast::SourcePos& span, const boost::uuids::uuid& moduleId_, ConditionalCompilationExpressionNode* expr)
{
    elifParts.Add(new ConditionalCompilationPartNode(span, moduleId_, expr));
}

void ConditionalCompilationStatementNode::AddElifStatement(StatementNode* statement)
{
    elifParts[elifParts.Count() - 1]->AddStatement(statement);
}

void ConditionalCompilationStatementNode::SetElifLeftParenSourcePos(const soul::ast::SourcePos& span)
{
    elifParts[elifParts.Count() - 1]->SetLeftParenSourcePos(span);
}

void ConditionalCompilationStatementNode::SetElifRightParenSourcePos(const soul::ast::SourcePos& span)
{
    elifParts[elifParts.Count() - 1]->SetRightParenSourcePos(span);
}

void ConditionalCompilationStatementNode::SetElifKeywordSourcePos(const soul::ast::SourcePos& span)
{
    elifParts[elifParts.Count() - 1]->SetKeywordSourcePos(span);
}

void ConditionalCompilationStatementNode::AddElseStatement(const soul::ast::SourcePos& span, const boost::uuids::uuid& moduleId_, StatementNode* statement)
{
    if (!elsePart)
    {
        elsePart.reset(new ConditionalCompilationPartNode(span, moduleId_));
    }
    elsePart->AddStatement(statement);
}

Node* ConditionalCompilationStatementNode::Clone(CloneContext& cloneContext) const
{
    ConditionalCompilationStatementNode* clone = new ConditionalCompilationStatementNode(GetSourcePos(), ModuleId());
    ConditionalCompilationPartNode* clonedIfPart = static_cast<ConditionalCompilationPartNode*>(ifPart->Clone(cloneContext));
    clone->ifPart.reset(clonedIfPart);
    int n = elifParts.Count();
    for (int i = 0; i < n; ++i)
    {
        ConditionalCompilationPartNode* elifPart = elifParts[i];
        ConditionalCompilationPartNode* clonedElifPart = static_cast<ConditionalCompilationPartNode*>(elifPart->Clone(cloneContext));
        clone->elifParts.Add(clonedElifPart);
    }
    if (elsePart)
    {
        ConditionalCompilationPartNode* clonedElsePart = static_cast<ConditionalCompilationPartNode*>(elsePart->Clone(cloneContext));
        clone->elsePart.reset(clonedElsePart);
    }
    clone->SetEndIfSourcePos(endifSourcePos);
    return clone;
}

void ConditionalCompilationStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalCompilationStatementNode::Write(AstWriter& writer)
{
    StatementNode::Write(writer);
    writer.Write(ifPart.get());
    elifParts.Write(writer);
    bool hasElsePart = elsePart != nullptr;
    writer.GetBinaryStreamWriter().Write(hasElsePart);
    if (hasElsePart)
    {
        writer.Write(elsePart.get());
    }
    //bool convertExternal = ModuleId() == writer.SpanConversionModuleId();
    //writer.Write(endifSpan, convertExternal);
}

void ConditionalCompilationStatementNode::Read(AstReader& reader)
{
    StatementNode::Read(reader);
    ifPart.reset(reader.ReadConditionalCompilationPartNode());
    ifPart->SetParent(this);
    elifParts.Read(reader);
    elifParts.SetParent(this);
    bool hasElsePart = reader.GetBinaryStreamReader().ReadBool();
    if (hasElsePart)
    {
        elsePart.reset(reader.ReadConditionalCompilationPartNode());
        elsePart->SetParent(this);
    }
    endifSourcePos = reader.ReadSourcePos();
}

void ConditionalCompilationStatementNode::SetIfPart(ConditionalCompilationPartNode* ifPart_)
{
    ifPart.reset(ifPart_);
}

void ConditionalCompilationStatementNode::AddElifPart(ConditionalCompilationPartNode* elifPart)
{
    elifParts.Add(elifPart);
}

void ConditionalCompilationStatementNode::SetElsePart(ConditionalCompilationPartNode* elsePart_)
{
    elsePart.reset(elsePart_);
}
} // namespace cmajor::ast
