// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.cpp;

namespace soul::ast::cpp {

std::string OperatorStr(Operator op)
{
    switch (op)
    {
        case Operator::deref: return "*";
        case Operator::addrOf: return "&";
        case Operator::plus: return "+";
        case Operator::minus: return "-";
        case Operator::and_: return "&&";
        case Operator::or_: return "||";
        case Operator::not_: return "!";
        case Operator::cpl: return "~";
        case Operator::dotStar: return ".*";
        case Operator::arrowStar: return "->*";
        case Operator::bitand_: return "&";
        case Operator::bitxor: return "^";
        case Operator::bitor_: return "|";
        case Operator::mul: return "*";
        case Operator::div: return "/";
        case Operator::rem: return "%";
        case Operator::shiftLeft: return "<<";
        case Operator::shiftRight: return ">>";
        case Operator::less: return "<";
        case Operator::greater: return ">";
        case Operator::lessOrEq: return "<=";
        case Operator::greaterOrEq: return ">=";
        case Operator::eq: return "==";
        case Operator::notEq: return "!=";
        case Operator::comma: return ",";
        case Operator::assign: return "=";
        case Operator::mulAssign: return "*=";
        case Operator::divAssign: return "/=";
        case Operator::remAssign: return "%=";
        case Operator::plusAssign: return "+=";
        case Operator::minusAssign: return "-=";
        case Operator::shiftLeftAssign: return "<<=";
        case Operator::shiftRightAssign: return ">>=";
        case Operator::andAssign: return "&=";
        case Operator::xorAssign: return "^=";
        case Operator::orAssign: return "|=";
    }
    return "";
}

Node::Node()
{
}

Node::~Node()
{
}

UnaryNode::UnaryNode(Node* child_) : Node(), child(child_)
{
}

BinaryNode::BinaryNode(Node* left_, Node* right_) : Node(), left(left_), right(right_)
{
}

ThisNode::ThisNode() : Node()
{
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThisNode::Write(CodeFormatter& formatter)
{
    formatter.Write("this");
}

IdExprNode::IdExprNode(const std::string& value_) : Node(), value(value_)
{
}

void IdExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write(value);
}

IndexExprNode::IndexExprNode(Node* child_, Node* index_) : UnaryNode(child_), index(index_)
{
}

void IndexExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IndexExprNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write("[");
    index->Write(formatter);
    formatter.Write("]");
}

std::string IndexExprNode::ToString() const
{
    return Child()->ToString() + "[" + index->ToString() + "]";
}

InvokeNode::InvokeNode(Node* child_) : UnaryNode(child_)
{
}

void InvokeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InvokeNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write("(");
    bool first = true;
    for (const auto& arg : args)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        arg->Write(formatter);
    }
    formatter.Write(")");
}

std::string InvokeNode::ToString() const
{
    std::string str = Child()->ToString();
    str.append(1, '(');
    bool first = true;
    for (const auto& arg : args)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(", ");
        }
        str.append(arg->ToString());
    }
    str.append(1, ')');
    return str;
}

void InvokeNode::Add(Node* node)
{
    args.push_back(std::unique_ptr<Node>(node));
}

MemberAccessNode::MemberAccessNode(Node* child_, Node* member_) : UnaryNode(child_), member(member_)
{
}

void MemberAccessNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void MemberAccessNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write(".");
    member->Write(formatter);
}

std::string MemberAccessNode::ToString() const
{
    return Child()->ToString() + "." + member->ToString();
}

PtrMemberAccessNode::PtrMemberAccessNode(Node* child_, Node* member_) : UnaryNode(child_), member(member_)
{
}

void PtrMemberAccessNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PtrMemberAccessNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write("->");
    member->Write(formatter);
}

std::string PtrMemberAccessNode::ToString() const
{
    return Child()->ToString() + "->" + member->ToString();
}

PostIncrementNode::PostIncrementNode(Node* child_) : UnaryNode(child_)
{
}

void PostIncrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PostIncrementNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write("++");
}

std::string PostIncrementNode::ToString() const
{
    return Child()->ToString() + "++";
}

PostDecrementNode::PostDecrementNode(Node* child_) : UnaryNode(child_)
{
}

void PostDecrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PostDecrementNode::Write(CodeFormatter& formatter)
{
    Child()->Write(formatter);
    formatter.Write("--");
}

std::string PostDecrementNode::ToString() const
{
    return Child()->ToString() + "--";
}

CppCastNode::CppCastNode()
{
}

StaticCastNode::StaticCastNode() : CppCastNode()
{
}

void StaticCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StaticCastNode::Write(CodeFormatter& formatter)
{
    formatter.Write("static_cast");
}

DynamicCastNode::DynamicCastNode() : CppCastNode()
{
}

void DynamicCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DynamicCastNode::Write(CodeFormatter& formatter)
{
    formatter.Write("dynamic_cast");
}

ReinterpretCastNode::ReinterpretCastNode() : CppCastNode()
{
}

void ReinterpretCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ReinterpretCastNode::Write(CodeFormatter& formatter)
{
    formatter.Write("reinterpret_cast");
}

ConstCastNode::ConstCastNode() : CppCastNode()
{
}

void ConstCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstCastNode::Write(CodeFormatter& formatter)
{
    formatter.Write("const_cast");
}

PostCastNode::PostCastNode(CppCastNode* cppCastNode_, Node* type_, Node* child_) : UnaryNode(child_), cppCastNode(cppCastNode_), type(type_)
{
}

void PostCastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PostCastNode::Write(CodeFormatter& formatter)
{
    cppCastNode->Write(formatter);
    formatter.Write("<");
    type->Write(formatter);
    formatter.Write(">(");
    Child()->Write(formatter);
    formatter.Write(")");
}

std::string PostCastNode::ToString() const
{
    std::string str = cppCastNode->ToString();
    str.append(1, '<');
    str.append(type->ToString());
    str.append(1, '>');
    str.append(1, '(');
    str.append(Child()->ToString());
    str.append(1, ')');
    return str;
}

TypeIdExprNode::TypeIdExprNode(Node* child_) : UnaryNode(child_)
{
}

void TypeIdExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeIdExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write("typeid(");
    Child()->Write(formatter);
    formatter.Write(")");
}

std::string TypeIdExprNode::ToString() const
{
    return "typeid(" + Child()->ToString() + ")";
}

PreIncrementNode::PreIncrementNode(Node* child_) : UnaryNode(child_)
{
}

void PreIncrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PreIncrementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("++");
    Child()->Write(formatter);
}

std::string PreIncrementNode::ToString() const
{
    return "++" + Child()->ToString();
}

PreDecrementNode::PreDecrementNode(Node* child_) : UnaryNode(child_)
{
}

void PreDecrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PreDecrementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("--");
    Child()->Write(formatter);
}

std::string PreDecrementNode::ToString() const
{
    return "--" + Child()->ToString();
}

UnaryOpExprNode::UnaryOpExprNode(Operator op_, Node* child_) : UnaryNode(child_), op(op_)
{
}

void UnaryOpExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnaryOpExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write(OperatorStr(op));
    Child()->Write(formatter);
}

std::string UnaryOpExprNode::ToString() const
{
    return OperatorStr(op) + Child()->ToString();
}

SizeOfNode::SizeOfNode(Node* child_, bool parens_) : UnaryNode(child_), parens(parens_)
{
}

void SizeOfNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SizeOfNode::Write(CodeFormatter& formatter) 
{
    formatter.Write("sizeof");
    if (parens)
    {
        formatter.Write("(");
    }
    else
    {
        formatter.Write(" ");
    }
    Child()->Write(formatter);
    if (parens)
    {
        formatter.Write(")");
    }
}

std::string SizeOfNode::ToString() const
{
    std::string str = "sizeof";
    if (parens)
    {
        str.append(1, '(');
    }
    else
    {
        str.append(1, ' ');
    }
    str.append(Child()->ToString());
    if (parens)
    {
        str.append(1, ')');
    }
    return str;
}

CastNode::CastNode(Node* type_, Node* child_) : UnaryNode(child_), type(type_)
{
}

void CastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CastNode::Write(CodeFormatter& formatter)
{
    formatter.Write("(");
    type->Write(formatter);
    formatter.Write(")");
    Child()->Write(formatter);
}

std::string CastNode::ToString() const
{
    return "(" + type->ToString() + ")" + Child()->ToString();
}

BinaryOpExprNode::BinaryOpExprNode(Operator op_, Node* left_, Node* right_) : BinaryNode(left_, right_), op(op_)
{
}

void BinaryOpExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BinaryOpExprNode::Write(CodeFormatter& formatter)
{
    Left()->Write(formatter);
    formatter.Write(" ");
    formatter.Write(OperatorStr(op));
    formatter.Write(" ");
    Right()->Write(formatter);
}

std::string BinaryOpExprNode::ToString() const
{
    return Left()->ToString() + " " + OperatorStr(op) + " " + Right()->ToString();
}

ConditionalNode::ConditionalNode(Node* condition_, Node* thenExpr_, Node* elseExpr_) : Node(), condition(condition_), thenExpr(thenExpr_), elseExpr(elseExpr_)
{
}

void ConditionalNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionalNode::Write(CodeFormatter& formatter)
{
    condition->Write(formatter);
    formatter.Write(" ? ");
    thenExpr->Write(formatter);
    formatter.Write(" : ");
    elseExpr->Write(formatter);
}

std::string ConditionalNode::ToString() const
{
    return condition->ToString() + " ? " + thenExpr->ToString() + " : " + elseExpr->ToString();
}

ThrowExprNode::ThrowExprNode(Node* exception_) : Node(), exception(exception_)
{
}

void ThrowExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThrowExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write("throw");
    if (exception)
    {
        formatter.Write(" ");
        exception->Write(formatter);
    }
}

std::string ThrowExprNode::ToString() const
{
    std::string str = "throw";
    if (exception)
    {
        str.append(1, ' ').append(exception->ToString());
    }
    return str;
}

NewNode::NewNode(bool global_) : Node(), global(global_), parens(false), addToPlacement(false), addToInitializer(false)
{
}

void NewNode::Add(Node* node)
{
    if (addToPlacement)
    {
        placement.push_back(std::unique_ptr<Node>(node));
    }
    else if (addToInitializer)
    {
        initializer.push_back(std::unique_ptr<Node>(node));
    }
}

void NewNode::SetTypeId(Node* typeId_)
{
    typeId.reset(typeId_);
}

void NewNode::Accept(Visitor& visitor) 
{
    visitor.Visit(*this);
}

void NewNode::Write(CodeFormatter& formatter)
{
    if (global)
    {
        formatter.Write("::");
    }
    formatter.Write("new");
    if (!placement.empty())
    {
        formatter.Write("(");
        bool first = true;
        for (const auto& node : placement)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                formatter.Write(", ");
            }
            node->Write(formatter);
        }
        formatter.Write(")");
    }
    if (parens)
    {
        formatter.Write("(");
    }
    formatter.Write(" ");
    typeId->Write(formatter);
    if (parens)
    {
        formatter.Write(")");
    }
    if (!initializer.empty())
    {
        formatter.Write("(");
        bool first = true;
        for (const auto& node : initializer)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                formatter.Write(", ");
            }
            node->Write(formatter);
        }
        formatter.Write(")");
    }
}

std::string NewNode::ToString() const
{
    std::string str;
    if (global)
    {
        str.append("::");
    }
    str.append("new");
    if (!placement.empty())
    {
        str.append("(");
        bool first = true;
        for (const auto& node : placement)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(", ");
            }
            str.append(node->ToString());
        }
        str.append(")");
    }
    if (parens)
    {
        str.append("(");
    }
    str.append(" ");
    str.append(typeId->ToString());
    if (parens)
    {
        str.append(")");
    }
    if (!initializer.empty())
    {
        str.append("(");
        bool first = true;
        for (const auto& node : initializer)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(", ");
            }
            str.append(node->ToString());
        }
        str.append(")");
    }
    return str;
}

DeleteNode::DeleteNode(bool global_, bool isArray_, Node* child_) : UnaryNode(child_),  global(global_), isArray(isArray_)
{
}

void DeleteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeleteNode::Write(CodeFormatter& formatter)
{
    if (global)
    {
        formatter.Write("::");
    }
    formatter.Write("delete");
    if (isArray)
    {
        formatter.Write("[]");
    }
    formatter.Write(" ");
    Child()->Write(formatter);
}

std::string DeleteNode::ToString() const
{
    std::string str;
    if (global)
    {
        str.append("::");
    }
    str.append("delete");
    if (isArray)
    {
        str.append("[]");
    }
    str.append(1, ' ');
    str.append(Child()->ToString());
    return str;
}

ParenExprNode::ParenExprNode(Node* child_) : UnaryNode(child_)
{
}

void ParenExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write("(");
    Child()->Write(formatter);
    formatter.Write(")");
}

std::string ParenExprNode::ToString() const
{
    return "(" + Child()->ToString() + ")";
}

LiteralNode::LiteralNode(const std::string& rep_) : Node(), rep(rep_)
{
}

void LiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LiteralNode::Write(CodeFormatter& formatter)
{
    formatter.Write(rep);
}

StatementNode::StatementNode() : Node()
{
}

LabeledStatementNode::LabeledStatementNode(const std::string& label_, StatementNode* stmt_) : StatementNode(), label(label_), stmt(stmt_)
{
}

void LabeledStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void LabeledStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write(label);
    formatter.Write(" : ");
    stmt->Write(formatter);
}

CaseStatementNode::CaseStatementNode(Node* caseExpr_, StatementNode* stmt_) : StatementNode(), caseExpr(caseExpr_), stmt(stmt_)
{
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CaseStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("case ");
    caseExpr->Write(formatter);
    formatter.Write(": ");
    stmt->Write(formatter);
}

DefaultStatementNode::DefaultStatementNode(StatementNode* stmt_) : StatementNode(), stmt(stmt_)
{
}

void DefaultStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DefaultStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("default: ");
    stmt->Write(formatter);
}

EmptyStatementNode::EmptyStatementNode() : StatementNode()
{
}

void EmptyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EmptyStatementNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine(";");
}

ExpressionStatementNode::ExpressionStatementNode(Node* expr_) : StatementNode(), expr(expr_)
{
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExpressionStatementNode::Write(CodeFormatter& formatter)
{
    expr->Write(formatter);
    formatter.WriteLine(";");
}

CompoundStatementNode::CompoundStatementNode() : StatementNode()
{
}

void CompoundStatementNode::InsertFront(StatementNode* stmt)
{
    statements.insert(statements.begin(), std::unique_ptr<StatementNode>(stmt));
}

void CompoundStatementNode::Add(StatementNode* stmt)
{
    statements.push_back(std::unique_ptr<StatementNode>(stmt));
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompoundStatementNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& stmt : statements)
    {
        stmt->Write(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

IfStatementNode::IfStatementNode(Node* cond_, StatementNode* thenStmt_, StatementNode* elseStmt_) : StatementNode(), cond(cond_), thenStmt(thenStmt_), elseStmt(elseStmt_)
{
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IfStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("if (");
    cond->Write(formatter);
    formatter.Write(")");
    if (thenStmt->IsCompoundStatementNode())
    {
        formatter.NewLine();
    }
    else
    {
        formatter.Write(" ");
    }
    thenStmt->Write(formatter);
    if (elseStmt)
    {
        formatter.Write("else");
        if (elseStmt->IsCompoundStatementNode())
        {
            formatter.NewLine();
        }
        else
        {
            formatter.Write(" ");
        }
        elseStmt->Write(formatter);
    }
}

SwitchStatementNode::SwitchStatementNode(Node* cond_, StatementNode* stmt_) : StatementNode(), cond(cond_), stmt(stmt_)
{
}

void SwitchStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SwitchStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("switch (");
    cond->Write(formatter);
    formatter.WriteLine(")");
    stmt->Write(formatter);
}

WhileStatementNode::WhileStatementNode(Node* cond_, StatementNode* stmt_) : StatementNode(), cond(cond_), stmt(stmt_)
{
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WhileStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("while (");
    cond->Write(formatter);
    formatter.Write(")");
    if (stmt->IsCompoundStatementNode())
    {
        formatter.NewLine();
    }
    else
    {
        formatter.Write(" ");
    }
    stmt->Write(formatter);
}

DoStatementNode::DoStatementNode(Node* cond_, StatementNode* stmt_) : StatementNode(), cond(cond_), stmt(stmt_)
{
}

void DoStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DoStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("do");
    if (stmt->IsCompoundStatementNode())
    {
        formatter.NewLine();
    }
    else
    {
        formatter.Write(" ");
    }
    stmt->Write(formatter);
    formatter.Write("while (");
    cond->Write(formatter);
    formatter.WriteLine(");");
}

ForStatementNode::ForStatementNode(Node* init_, Node* cond_, Node* iter_, StatementNode* stmt_) : StatementNode(), init(init_), cond(cond_), iter(iter_), stmt(stmt_)
{
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("for (");
    if (init)
    {
        init->Write(formatter);
    }
    formatter.Write("; ");
    if (cond)
    {
        cond->Write(formatter);
    }
    formatter.Write("; ");
    if (iter)
    {
        iter->Write(formatter);
    }
    formatter.Write(")");
    if (stmt->IsCompoundStatementNode())
    {
        formatter.NewLine();
    }
    else
    {
        formatter.Write(" ");
    }
    stmt->Write(formatter);
}

BreakStatementNode::BreakStatementNode() : StatementNode()
{
}

void BreakStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BreakStatementNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("break;");
}

ContinueStatementNode::ContinueStatementNode() : StatementNode()
{
}

void ContinueStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ContinueStatementNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("continue;");
}

GotoStatementNode::GotoStatementNode(const std::string& target_) : StatementNode(), target(target_)
{
}

void GotoStatementNode::Accept(Visitor& visitor)
{
visitor.Visit(*this);
}

void GotoStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("goto ");
    formatter.Write(target);
    formatter.WriteLine(";");
}

ReturnStatementNode::ReturnStatementNode(Node* expr_) : StatementNode(), expr(expr_)
{
}

void ReturnStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ReturnStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("return");
    if (expr)
    {
        formatter.Write(" ");
        expr->Write(formatter);
    }
    formatter.WriteLine(";");
}

ConditionWithDeclaratorNode::ConditionWithDeclaratorNode(TypeIdNode* type_, const std::string& declarator_, Node* expression_) : 
    Node(), type(type_), declarator(declarator_), expression(expression_)
{
}

void ConditionWithDeclaratorNode::Write(CodeFormatter& formatter)
{
    type->Write(formatter);
    formatter.Write(" " + declarator + " = ");
    expression->Write(formatter);
}

void ConditionWithDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ForRangeDeclarationNode::ForRangeDeclarationNode() : Node(), typeId(new TypeIdNode()), declarator()
{
}

void ForRangeDeclarationNode::SetDeclarator(const std::string& declarator_)
{
    declarator = declarator_;
}

void ForRangeDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForRangeDeclarationNode::Write(CodeFormatter& formatter)
{
    typeId->Write(formatter);
    formatter.Write(" ");
    formatter.Write(declarator);
}

RangeForStatementNode::RangeForStatementNode(ForRangeDeclarationNode* declaration_, Node* container_, StatementNode* stmt_) :
    declaration(declaration_), container(container_), stmt(stmt_)
{
}

void RangeForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RangeForStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("for (");
    declaration->Write(formatter);
    formatter.Write(" : ");
    container->Write(formatter);
    formatter.Write(")");
    if (stmt->IsCompoundStatementNode())
    {
        formatter.NewLine();
    }
    else
    {
        formatter.Write(" ");
    }
    stmt->Write(formatter);
}

DeclarationStatementNode::DeclarationStatementNode(Node* declaration_)
{
}

void DeclarationStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DeclarationStatementNode::Write(CodeFormatter& formatter)
{
    declaration->Write(formatter);
    formatter.WriteLine(";");
}

ExceptionDeclarationNode::ExceptionDeclarationNode() :
    Node(), typeId(new TypeIdNode()), declarator(), catchAll(false)
{
}

void ExceptionDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExceptionDeclarationNode::Write(CodeFormatter& formatter)
{
    if (catchAll)
    {
        formatter.Write("...");
    }
    else
    { 
        typeId->Write(formatter);
        formatter.Write(" "); 
        formatter.Write(declarator);
    }
}

HandlerNode::HandlerNode(ExceptionDeclarationNode* exceptionDeclaration_, CompoundStatementNode* handlerBlock_) : 
    Node(), exceptionDeclaration(exceptionDeclaration_), handlerBlock(handlerBlock_)
{
}

void HandlerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void HandlerNode::Write(CodeFormatter& formatter)
{
    formatter.Write("catch ("); 
    exceptionDeclaration->Write(formatter);
    formatter.Write(")");
    handlerBlock->Write(formatter);
}

TryStatementNode::TryStatementNode(CompoundStatementNode* tryBlock_) : tryBlock(tryBlock_)
{
}

void TryStatementNode::AddHandler(HandlerNode* handler)
{
    handlers.push_back(std::unique_ptr<HandlerNode>(handler));
}

void TryStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TryStatementNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("try");
    tryBlock->Write(formatter);
    for (const auto& handler : handlers)
    {
        handler->Write(formatter);
    }
}

AssignInitNode::AssignInitNode(Node* assignmentExpr_) : Node(), assignmentExpr(assignmentExpr_)
{
}

void AssignInitNode::Add(AssignInitNode* subInit)
{
    subInits.push_back(std::unique_ptr<AssignInitNode>(subInit));
}

void AssignInitNode::Write(CodeFormatter& formatter)
{
    if (assignmentExpr)
    {
        assignmentExpr->Write(formatter);
    }
    else
    {
        formatter.Write("{ ");
        bool first = true;
        for (const auto& subInit : subInits)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                formatter.Write(", ");
            }
            subInit->Write(formatter);
        }
        formatter.Write(" }");
    }
}

std::string AssignInitNode::ToString() const
{
    if (assignmentExpr)
    {
        return assignmentExpr->ToString();
    }
    else
    {
        std::string str;
        str.append("{ ");
        bool first = true;
        for (const auto& subInit : subInits)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(", ");
            }
            str.append(subInit->ToString());
        }
        str.append(" }");
        return str;
    }
}

void AssignInitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitializerNode::InitializerNode(AssignInitNode* assignInit_) : assignInit(assignInit_)
{
}

void InitializerNode::Add(Node* expr)
{
    expressionList.push_back(std::unique_ptr<Node>(expr));
}

void InitializerNode::Write(CodeFormatter& formatter)
{
    if (assignInit)
    {
        formatter.Write(" = ");
        assignInit->Write(formatter);
    }
    else if (!expressionList.empty())
    {
        formatter.Write("(");
        bool first = true;
        for (const auto& expr : expressionList)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                formatter.Write(", ");
            }
            expr->Write(formatter);
        }
        formatter.Write(")");
    }
}

std::string InitializerNode::ToString() const
{
    std::string str;
    if (assignInit)
    {
        return " = " + assignInit->ToString();
    }
    else if (!expressionList.empty())
    {
        str.append("(");
        bool first = true;
        for (const auto& expr : expressionList)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(", ");
            }
            str.append(expr->ToString());
        }
        str.append(")");
    }
    return str;
}

void InitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitDeclaratorNode::InitDeclaratorNode(const std::string& declarator_, InitializerNode* initializer_) : declarator(declarator_), initializer(initializer_)
{
}

void InitDeclaratorNode::Write(CodeFormatter& formatter)
{
    formatter.Write(declarator);
    if (initializer)
    {
        initializer->Write(formatter);
    }
}

std::string InitDeclaratorNode::ToString() const
{
    std::string str;
    str.append(declarator);
    if (initializer)
    {
        str.append(initializer->ToString());
    }
    return str;
}

void InitDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitDeclaratorListNode::InitDeclaratorListNode() : Node()
{
}

void InitDeclaratorListNode::Add(InitDeclaratorNode* initDeclarator)
{
    initDeclarators.push_back(std::unique_ptr<InitDeclaratorNode>(initDeclarator));
}

void InitDeclaratorListNode::Write(CodeFormatter& formatter)
{
    bool first = true;
    for (const auto& initDeclarator : initDeclarators)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        initDeclarator->Write(formatter);
    }
}

std::string InitDeclaratorListNode::ToString() const
{
    std::string str;
    bool first = true;
    for (const auto& initDeclarator : initDeclarators)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(", ");
        }
        str.append(initDeclarator->ToString());
    }
    return str;
}

void InitDeclaratorListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeclSpecifierNode::DeclSpecifierNode(const std::string& name_) : Node(), name(name_)
{
}

TypedefNode::TypedefNode() : DeclSpecifierNode("typedef")
{
}

void TypedefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypedefNode::Write(CodeFormatter& formatter)
{
    formatter.Write(Name());
}

std::string TypedefNode::ToString() const
{
    return Name();
}

TypeSpecifierNode::TypeSpecifierNode(const std::string& name_) : DeclSpecifierNode(name_)
{
}

void TypeSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeSpecifierNode::Write(CodeFormatter& formatter)
{
    formatter.Write(Name());
}

ConstNode::ConstNode() : TypeSpecifierNode("const")
{
}

void ConstNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VolatileNode::VolatileNode() : TypeSpecifierNode("volatile")
{
}

void VolatileNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNameNode::TypeNameNode(const std::string& name_) : TypeSpecifierNode(name_), isTemplate(false)
{
}

void TypeNameNode::AddTemplateArgument(Node* templateArgument)
{
    templateArguments.push_back(std::unique_ptr<Node>(templateArgument));
}

std::string TypeNameNode::ToString() const
{
    std::string str(Name());
    if (isTemplate)
    {
        str.append("<");
        bool first = true;
        for (const auto& templateArgument : templateArguments)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                str.append(", ");
            }
            str.append(templateArgument->ToString());
        }
        str.append(">");
    }
    return str;
}

void TypeNameNode::Write(CodeFormatter& formatter)
{
    formatter.Write(ToString());
}

void TypeNameNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNode::TypeNode() : Node()
{
}

void TypeNode::Add(TypeSpecifierNode* typeSpecifier)
{
    typeSpecifiers.push_back(std::unique_ptr<TypeSpecifierNode>(typeSpecifier));
}

std::string TypeNode::ToString() const
{
    std::string str;
    bool first = true;
    for (const auto& typeSpecifier : typeSpecifiers)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ' ');
        }
        str.append(typeSpecifier->ToString());
    }
    return str;
}

void TypeNode::Write(CodeFormatter& formatter)
{
    formatter.Write(ToString());
}

void TypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StorageClassSpecifierNode::StorageClassSpecifierNode(const std::string& name_) : DeclSpecifierNode(name_)
{
}

void StorageClassSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StorageClassSpecifierNode::Write(CodeFormatter& formatter)
{
    formatter.Write(Name());
}

std::string StorageClassSpecifierNode::ToString() const
{
    return Name();
}

TypeIdNode::TypeIdNode() : Node()
{
}

void TypeIdNode::Add(TypeSpecifierNode* typeSpecifier)
{
    typeSpecifiers.push_back(std::unique_ptr<TypeSpecifierNode>(typeSpecifier));
}

void TypeIdNode::SetDeclarator(const std::string& declarator_)
{
    declarator = declarator_;
}

void TypeIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeIdNode::Write(CodeFormatter& formatter)
{
    bool first = true;
    for (const auto& typeSpecifier : typeSpecifiers)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(" ");
        }
        typeSpecifier->Write(formatter);
    }
    formatter.Write(declarator);
}

std::string TypeIdNode::ToString() const
{
    std::string str;
    bool first = true;
    for (const auto& typeSpecifier : typeSpecifiers)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(" ");
        }
        str.append(typeSpecifier->ToString());
    }
    str.append(declarator);
    return str;
}

SimpleDeclarationNode::SimpleDeclarationNode() : Node()
{
}

void SimpleDeclarationNode::Add(DeclSpecifierNode* declSpecifier)
{
    declSpecifiers.push_back(std::unique_ptr<DeclSpecifierNode>(declSpecifier));
}

void SimpleDeclarationNode::SetInitDeclaratorList(InitDeclaratorListNode* initDeclaratorList_)
{
    initDeclaratorList.reset(initDeclaratorList_);
}

void SimpleDeclarationNode::Write(CodeFormatter& formatter)
{
    bool first = true;
    for (const auto& declSpecifier : declSpecifiers)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(" ");
        }
        declSpecifier->Write(formatter);
    }
    if (!declSpecifiers.empty())
    {
        formatter.Write(" ");
    }
    if (initDeclaratorList)
    {
        initDeclaratorList->Write(formatter);
    }
}

void SimpleDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingObjectNode::UsingObjectNode(const std::string& name_) : Node(), name(name_)
{
}

NamespaceAliasNode::NamespaceAliasNode(const std::string& aliasName_, const std::string& namespaceName_) : 
    UsingObjectNode("namespaceAlias"), aliasName(aliasName_), namespaceName(namespaceName_)
{
}

void NamespaceAliasNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("namespace " + aliasName + " = " + namespaceName + ";");
}

void NamespaceAliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingDeclarationNode::UsingDeclarationNode(const std::string& usingId_) : UsingObjectNode("usingDeclaration"), usingId(usingId_)
{
}

void UsingDeclarationNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("using " + usingId + ";");
}

void UsingDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UsingDirectiveNode::UsingDirectiveNode(const std::string& usingNs_) : UsingObjectNode("usingDirective"), usingNs(usingNs_)
{
}

void UsingDirectiveNode::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("using namespace " + usingNs + ";");
}

void UsingDirectiveNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace soul::ast::cpp
