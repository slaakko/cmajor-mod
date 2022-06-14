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

Node::Node() : parent(nullptr)
{
}

Node::~Node()
{
}

UnaryNode::UnaryNode(Node* child_) : Node(), child(child_)
{
    child->SetParent(this);
}

void UnaryNode::Replace(Node* node, Node* replacement)
{
    if (child.get() == node)
    {
        child.reset(replacement);
    }
}

BinaryNode::BinaryNode(Node* left_, Node* right_) : Node(), left(left_), right(right_)
{
    left->SetParent(this);
    right->SetParent(this);
}

void BinaryNode::Replace(Node* node, Node* replacement)
{
    if (left.get() == node)
    {
        left.reset(replacement);
    }
    else if (right.get() == node)
    {
        right.reset(replacement);
    }
}

ExprListNode::ExprListNode()
{
}

Node* ExprListNode::Clone() const
{
    ExprListNode* clone = new ExprListNode();
    for (const auto& expr : exprs)
    {
        clone->Add(expr->Clone());
    }
    return clone;
}

void ExprListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExprListNode::Write(CodeFormatter& formatter)
{
    formatter.Write("(");
    bool first = true;
    for (const auto& expr : exprs)
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

std::string ExprListNode::ToString() const
{
    std::string str = "(";
    bool first = true;
    for (const auto& expr : exprs)
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
    return str;
}

void ExprListNode::Add(Node* node)
{
    node->SetParent(this);
    exprs.push_back(std::unique_ptr<Node>(node));
}

void ExprListNode::Replace(Node* node, Node* replacement)
{
    for (auto& expr : exprs)
    {
        if (expr.get() == node)
        {
            expr.reset(replacement);
            break;
        }
    }
}

ThisNode::ThisNode() : Node()
{
}

Node* ThisNode::Clone() const
{
    return new ThisNode();
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThisNode::Write(CodeFormatter& formatter)
{
    formatter.Write("this");
}

IdExprNode::IdExprNode(const std::string& id_) : Node(), id(id_)
{
}

void IdExprNode::SetId(const std::string& id_)
{
    id = id_;
}

Node* IdExprNode::Clone() const
{
    return new IdExprNode(id);
}

void IdExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdExprNode::Write(CodeFormatter& formatter)
{
    formatter.Write(id);
}

IndexExprNode::IndexExprNode(Node* child_, Node* index_) : UnaryNode(child_), index(index_)
{
    index->SetParent(this);
}

Node* IndexExprNode::Clone() const
{
    return new IndexExprNode(Child()->Clone(), index->Clone());
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

Node* InvokeNode::Clone() const
{
    InvokeNode* clone = new InvokeNode(Child()->Clone());
    for (const auto& arg : args)
    {
        clone->Add(arg->Clone());
    }
    return clone;
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
    node->SetParent(this);
    args.push_back(std::unique_ptr<Node>(node));
}

void InvokeNode::Replace(Node* node, Node* replacement)
{
    for (auto& arg : args)
    {
        if (arg.get() == node)
        {
            arg.reset(replacement);
            break;
        }
    }
}

MemberAccessNode::MemberAccessNode(Node* child_, Node* member_) : UnaryNode(child_), member(member_)
{
    member->SetParent(this);
}

Node* MemberAccessNode::Clone() const
{
    return new MemberAccessNode(Child()->Clone(), member->Clone());
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
    member->SetParent(this);
}

Node* PtrMemberAccessNode::Clone() const
{
    return new PtrMemberAccessNode(Child()->Clone(), member->Clone());
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

Node* PostIncrementNode::Clone() const
{
    return new PostIncrementNode(Child()->Clone());
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

Node* PostDecrementNode::Clone() const
{
    return new PostDecrementNode(Child()->Clone());
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

Node* StaticCastNode::Clone() const
{
    return new StaticCastNode();
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

Node* DynamicCastNode::Clone() const
{
    return new DynamicCastNode();
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

Node* ReinterpretCastNode::Clone() const
{
    return new ReinterpretCastNode();
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

Node* ConstCastNode::Clone() const
{
    return new ConstCastNode();
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
    cppCastNode->SetParent(this);
    type->SetParent(this);
}

Node* PostCastNode::Clone() const
{
    return new PostCastNode(static_cast<CppCastNode*>(cppCastNode->Clone()), type->Clone(), Child()->Clone());
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

Node* TypeIdExprNode::Clone() const
{
    return new TypeIdExprNode(Child()->Clone());
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

Node* PreIncrementNode::Clone() const
{
    return new PreIncrementNode(Child()->Clone());
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

Node* PreDecrementNode::Clone() const
{
    return new PreDecrementNode(Child()->Clone());
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

Node* UnaryOpExprNode::Clone() const
{
    return new UnaryOpExprNode(op, Child()->Clone());
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

Node* SizeOfNode::Clone() const
{
    return new SizeOfNode(Child()->Clone(), parens);
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
    type->SetParent(this);
}

Node* CastNode::Clone() const
{
    return new CastNode(type->Clone(), Child()->Clone());
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

Node* BinaryOpExprNode::Clone() const
{
    return new BinaryOpExprNode(op, Left()->Clone(), Right()->Clone());
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
    condition->SetParent(this);
    thenExpr->SetParent(this);
    elseExpr->SetParent(this);
}

Node* ConditionalNode::Clone() const
{
    return new ConditionalNode(condition->Clone(), thenExpr->Clone(), elseExpr->Clone());
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
    if (exception)
    {
        exception->SetParent(this);
    }
}

Node* ThrowExprNode::Clone() const
{
    if (exception)
    {
        return new ThrowExprNode(exception->Clone());
    }
    else
    {
        return new ThrowExprNode(nullptr);
    }
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
    node->SetParent(this);
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
    typeId->SetParent(this);
}

Node* NewNode::Clone() const
{
    NewNode* clone = new NewNode(global);
    if (typeId)
    {
        clone->SetTypeId(typeId->Clone());
    }
    clone->BeginAddToPlacement();
    for (const auto& node : placement)
    {
        clone->Add(node->Clone());
    }
    clone->EndAddToPlacement();
    clone->BeginAddToInitializer();
    for (const auto& node : initializer)
    {
        clone->Add(node->Clone());
    }
    clone->EndAddToInitializer();
    return clone;
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

Node* DeleteNode::Clone() const
{
    return new DeleteNode(global, isArray, Child()->Clone());
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

Node* ParenExprNode::Clone() const
{
    return new ParenExprNode(Child()->Clone());
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

Node* LiteralNode::Clone() const
{
    return new LiteralNode(rep);
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
    stmt->SetParent(this);
}

Node* LabeledStatementNode::Clone() const
{
    return new LabeledStatementNode(label, static_cast<StatementNode*>(stmt->Clone()));
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
    caseExpr->SetParent(this);
    stmt->SetParent(this);
}

Node* CaseStatementNode::Clone() const
{
    return new CaseStatementNode(caseExpr->Clone(), static_cast<StatementNode*>(stmt->Clone()));
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
    stmt->SetParent(this);
}

Node* DefaultStatementNode::Clone() const
{
    return new DefaultStatementNode(static_cast<StatementNode*>(stmt->Clone()));
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

Node* EmptyStatementNode::Clone() const
{
    return new EmptyStatementNode();
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
    expr->SetParent(this);
}

Node* ExpressionStatementNode::Clone() const
{
    return new ExpressionStatementNode(expr->Clone());
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
    stmt->SetParent(this);
    statements.insert(statements.begin(), std::unique_ptr<StatementNode>(stmt));
}

void CompoundStatementNode::Add(StatementNode* stmt)
{
    stmt->SetParent(this);
    statements.push_back(std::unique_ptr<StatementNode>(stmt));
}

void CompoundStatementNode::Replace(Node* node, Node* replacement)
{
    for (auto& stmt : statements)
    {
        if (stmt.get() == node)
        {
            stmt.reset(static_cast<StatementNode*>(replacement));
        }
    }
}

Node* CompoundStatementNode::Clone() const
{
    CompoundStatementNode* clone = new CompoundStatementNode();
    for (const auto& stmt : statements)
    {
        clone->Add(static_cast<StatementNode*>(stmt->Clone()));
    }
    return clone;
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
    cond->SetParent(this);
    thenStmt->SetParent(this);
    if (elseStmt)
    {
        elseStmt->SetParent(this);
    }
}

Node* IfStatementNode::Clone() const
{
    StatementNode* clonedElseStmt = nullptr;
    if (elseStmt)
    {
        clonedElseStmt = static_cast<StatementNode*>(elseStmt->Clone());
    }
    return new IfStatementNode(cond->Clone(), static_cast<StatementNode*>(thenStmt->Clone()), clonedElseStmt);
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
    cond->SetParent(this);
    stmt->SetParent(this);
}

Node* SwitchStatementNode::Clone() const
{
    return new SwitchStatementNode(cond->Clone(), static_cast<StatementNode*>(stmt->Clone()));
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
    cond->SetParent(this);
    stmt->SetParent(this);
}

Node* WhileStatementNode::Clone() const
{
    return new WhileStatementNode(cond->Clone(), static_cast<StatementNode*>(stmt->Clone()));
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
    cond->SetParent(this);
    stmt->SetParent(this);
}

Node* DoStatementNode::Clone() const
{
    return new DoStatementNode(cond->Clone(), static_cast<StatementNode*>(stmt->Clone()));
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
    if (init)
    {
        init->SetParent(this);
    }
    if (cond)
    {
        cond->SetParent(this);
    }
    if (iter)
    {
        iter->SetParent(this);
    }
    stmt->SetParent(this);
}

Node* ForStatementNode::Clone() const
{
    Node* clonedInit = nullptr;
    if (init)
    {
        clonedInit = init->Clone();
    }
    Node* clonedCond = nullptr;
    if (cond)
    {
        clonedCond = cond->Clone();
    }
    Node* clonedIter = nullptr;
    if (iter)
    {
        clonedIter = iter->Clone();
    }
    return new ForStatementNode(clonedInit, clonedCond, clonedIter, static_cast<StatementNode*>(stmt->Clone()));
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

Node* BreakStatementNode::Clone() const
{
    return new BreakStatementNode();
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

Node* ContinueStatementNode::Clone() const
{
    return new ContinueStatementNode();
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

Node* GotoStatementNode::Clone() const
{
    return new GotoStatementNode(target);
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
    if (expr)
    {
        expr->SetParent(this);
    }
}

void ReturnStatementNode::SetExpr(Node* expr_)
{
    expr.reset(expr_);
}

Node* ReturnStatementNode::Clone() const
{
    Node* clonedExpr = nullptr;
    if (expr)
    {
        clonedExpr = expr->Clone();
    }
    return new ReturnStatementNode(clonedExpr);
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
    type->SetParent(this);
    expression->SetParent(this);
}

Node* ConditionWithDeclaratorNode::Clone() const
{
    return new ConditionWithDeclaratorNode(static_cast<TypeIdNode*>(type->Clone()), declarator, expression->Clone());
}

void ConditionWithDeclaratorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConditionWithDeclaratorNode::Write(CodeFormatter& formatter)
{
    type->Write(formatter);
    formatter.Write(" " + declarator + " = ");
    expression->Write(formatter);
}

ForRangeDeclarationNode::ForRangeDeclarationNode() : Node(), typeId(new TypeIdNode()), declarator()
{
    typeId->SetParent(this);
}

void ForRangeDeclarationNode::SetDeclarator(const std::string& declarator_)
{
    declarator = declarator_;
}

Node* ForRangeDeclarationNode::Clone() const
{
    ForRangeDeclarationNode* clone = new ForRangeDeclarationNode();
    clone->SetDeclarator(declarator);
    return clone;
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
    declaration->SetParent(this);
    container->SetParent(this);
    stmt->SetParent(this);
}

Node* RangeForStatementNode::Clone() const
{
    return new RangeForStatementNode(static_cast<ForRangeDeclarationNode*>(declaration->Clone()), container->Clone(), static_cast<StatementNode*>(stmt->Clone()));
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

DeclarationStatementNode::DeclarationStatementNode(Node* declaration_) : declaration(declaration_)
{
    declaration->SetParent(this);
}

Node* DeclarationStatementNode::Clone() const
{
    return new DeclarationStatementNode(declaration->Clone());
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
    typeId->SetParent(this);
}

void ExceptionDeclarationNode::SetDeclarator(const std::string& declarator_)
{
    declarator = declarator_;
}

Node* ExceptionDeclarationNode::Clone() const
{
    ExceptionDeclarationNode* clone = new ExceptionDeclarationNode();
    clone->SetDeclarator(declarator);
    if (catchAll)
    {
        clone->SetCatchAll();
    }
    return clone;
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
    exceptionDeclaration->SetParent(this);
    handlerBlock->SetParent(this);
}

Node* HandlerNode::Clone() const
{
    return new HandlerNode(static_cast<ExceptionDeclarationNode*>(exceptionDeclaration->Clone()), static_cast<CompoundStatementNode*>(handlerBlock->Clone()));
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
    tryBlock->SetParent(this);
}

void TryStatementNode::AddHandler(HandlerNode* handler)
{
    handler->SetParent(this);
    handlers.push_back(std::unique_ptr<HandlerNode>(handler));
}

Node* TryStatementNode::Clone() const
{
    TryStatementNode* clone = new TryStatementNode(static_cast<CompoundStatementNode*>(tryBlock->Clone()));
    for (const auto& handler : handlers)
    {
        clone->AddHandler(static_cast<HandlerNode*>(handler->Clone()));
    }
    return clone;
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

IfdefStatementNode::IfdefStatementNode(Node* symbol_) : StatementNode(), symbol(symbol_)
{
    symbol->SetParent(this);
}

Node* IfdefStatementNode::Clone() const
{
    return new IfdefStatementNode(symbol->Clone());
}

void IfdefStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IfdefStatementNode::Write(CodeFormatter& formatter)
{
    formatter.Write("#ifdef ");
    symbol->Write(formatter);
    formatter.WriteLine();
}

EndIfStatementNode::EndIfStatementNode(Node* comment_) : StatementNode(), comment(comment_)
{
    if (comment)
    {
        comment->SetParent(this);
    }
}

Node* EndIfStatementNode::Clone() const
{
    Node* clonedComment = nullptr;
    if (comment)
    {
        clonedComment = comment->Clone();
    }
    return new EndIfStatementNode(clonedComment);
}

void EndIfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EndIfStatementNode::Write(CodeFormatter& formatter)
{
    if (comment)
    {
        formatter.Write("#endif ");
        comment->Write(formatter);
        formatter.WriteLine();
    }
    else
    {
        formatter.WriteLine("#endif");
    }
}

AssignInitNode::AssignInitNode(Node* assignmentExpr_) : Node(), assignmentExpr(assignmentExpr_)
{
    if (assignmentExpr)
    {
        assignmentExpr->SetParent(this);
    }
}

void AssignInitNode::Add(AssignInitNode* subInit)
{
    subInit->SetParent(this);
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

Node* AssignInitNode::Clone() const
{
    Node* clonedAssignmentExprNode = nullptr;
    if (assignmentExpr)
    {
        clonedAssignmentExprNode = assignmentExpr->Clone();
    }
    AssignInitNode* clone = new AssignInitNode(clonedAssignmentExprNode);
    for (const auto& subInit : subInits)
    {
        clone->Add(static_cast<AssignInitNode*>(subInit->Clone()));
    }
    return clone;
}

void AssignInitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitializerNode::InitializerNode(AssignInitNode* assignInit_) : Node(), assignInit(assignInit_)
{
    if (assignInit)
    {
        assignInit->SetParent(this);
    }
}

void InitializerNode::Add(Node* expr)
{
    expr->SetParent(this);
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

Node* InitializerNode::Clone() const
{
    AssignInitNode* clonedAssignInitNode = nullptr;
    if (assignInit)
    {
        clonedAssignInitNode = static_cast<AssignInitNode*>(assignInit->Clone());
    }
    InitializerNode* clone = new InitializerNode(clonedAssignInitNode);
    for (const auto& expr : expressionList)
    {
        clone->Add(expr->Clone());
    }
    return clone;
}

void InitializerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InitDeclaratorNode::InitDeclaratorNode(const std::string& declarator_, InitializerNode* initializer_) : declarator(declarator_), initializer(initializer_)
{
    if (initializer)
    {
        initializer->SetParent(this);
    }
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

Node* InitDeclaratorNode::Clone() const
{
    InitializerNode* clonedInitializer = nullptr;
    if (initializer)
    {
        clonedInitializer = static_cast<InitializerNode*>(initializer->Clone());
    }
    return new InitDeclaratorNode(declarator, clonedInitializer);
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
    initDeclarator->SetParent(this);
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

Node* InitDeclaratorListNode::Clone() const
{
    InitDeclaratorListNode* clone = new InitDeclaratorListNode();
    for (const auto& initDeclarator : initDeclarators)
    {
        clone->Add(static_cast<InitDeclaratorNode*>(initDeclarator->Clone()));
    }
    return clone;
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

Node* TypedefNode::Clone() const
{
    return new TypedefNode();
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

TypeSpecifierNode::TypeSpecifierNode(const std::string& name_) : DeclSpecifierNode(name_), kind(TypeSpecifierNodeKind::typeSpecifierNode)
{
}

TypeSpecifierNode::TypeSpecifierNode(TypeSpecifierNodeKind kind_, const std::string& name_) : DeclSpecifierNode(name_), kind(kind_)
{
}

Node* TypeSpecifierNode::Clone() const
{
    return new TypeSpecifierNode(Kind(), Name());
}

void TypeSpecifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeSpecifierNode::Write(CodeFormatter& formatter)
{
    formatter.Write(Name());
}

ConstNode::ConstNode() : TypeSpecifierNode(TypeSpecifierNodeKind::constNode, "const")
{
}

Node* ConstNode::Clone() const
{
    return new ConstNode();
}

void ConstNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VolatileNode::VolatileNode() : TypeSpecifierNode(TypeSpecifierNodeKind::volatileNode, "volatile")
{
}

Node* VolatileNode::Clone() const
{
    return new VolatileNode();
}

void VolatileNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNameNode::TypeNameNode(const std::string& name_) : TypeSpecifierNode(TypeSpecifierNodeKind::typeNameNode, name_), isTemplate(false)
{
}

void TypeNameNode::AddTemplateArgument(Node* templateArgument)
{
    templateArgument->SetParent(this);
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

Node* TypeNameNode::Clone() const
{
    TypeNameNode* clone = new TypeNameNode(Name());
    if (isTemplate)
    {
        clone->SetTemplate();
    }
    for (const auto& templateArg : templateArguments)
    {
        clone->AddTemplateArgument(templateArg->Clone());
    }
    return clone;
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
    typeSpecifier->SetParent(this);
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

Node* TypeNode::Clone() const
{
    TypeNode* clone = new TypeNode();
    for (const auto& typeSpecifier : typeSpecifiers)
    {
        clone->Add(static_cast<TypeSpecifierNode*>(typeSpecifier->Clone()));
    }
    return clone;
}

void TypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StorageClassSpecifierNode::StorageClassSpecifierNode(const std::string& name_) : DeclSpecifierNode(name_)
{
}

Node* StorageClassSpecifierNode::Clone() const
{
    return new StorageClassSpecifierNode(Name());
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
    typeSpecifier->SetParent(this);
    typeSpecifiers.push_back(std::unique_ptr<TypeSpecifierNode>(typeSpecifier));
}

void TypeIdNode::SetDeclarator(const std::string& declarator_)
{
    declarator = declarator_;
}

Node* TypeIdNode::Clone() const
{
    TypeIdNode* clone = new TypeIdNode();
    clone->SetDeclarator(declarator);
    for (const auto& typeSpecifier : typeSpecifiers)
    {
        clone->Add(static_cast<TypeSpecifierNode*>(typeSpecifier->Clone()));
    }
    return clone;
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

void TypeIdNode::WriteNonPtrType(CodeFormatter& formatter)
{
    if (IsPtrType())
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
        if (!declarator.empty())
        {
            formatter.Write(declarator.substr(0, declarator.size() - 1));
        }
    }
    else
    {
        Write(formatter);
    }
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

bool TypeIdNode::IsPtrType() const
{
    if (!declarator.empty())
    {
        return declarator.back() == '*';
    }
    else
    {
        return false;
    }
}

SimpleDeclarationNode::SimpleDeclarationNode() : Node()
{
}

void SimpleDeclarationNode::Add(DeclSpecifierNode* declSpecifier)
{
    declSpecifier->SetParent(this);
    declSpecifiers.push_back(std::unique_ptr<DeclSpecifierNode>(declSpecifier));
}

void SimpleDeclarationNode::SetInitDeclaratorList(InitDeclaratorListNode* initDeclaratorList_)
{
    initDeclaratorList.reset(initDeclaratorList_);
    initDeclaratorList->SetParent(this);
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

Node* SimpleDeclarationNode::Clone() const
{
    SimpleDeclarationNode* clone = new SimpleDeclarationNode();
    for (const auto& declSpecifier : declSpecifiers)
    {
        clone->Add(static_cast<DeclSpecifierNode*>(declSpecifier->Clone()));
    }
    if (initDeclaratorList)
    {
        clone->SetInitDeclaratorList(static_cast<InitDeclaratorListNode*>(initDeclaratorList->Clone()));
    }
    return clone;
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

Node* NamespaceAliasNode::Clone() const
{
    return new NamespaceAliasNode(aliasName, namespaceName);
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

Node* UsingDeclarationNode::Clone() const
{
    return new UsingDeclarationNode(usingId);
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

Node* UsingDirectiveNode::Clone() const
{
    return new UsingDirectiveNode(usingNs);
}

void UsingDirectiveNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DefaultVisitor::Visit(ExprListNode& node)
{
    for (const auto& expr : node.Exprs())
    {
        expr->Accept(*this);
    }
}

void DefaultVisitor::Visit(IndexExprNode& node)
{
    node.Child()->Accept(*this);
    node.Index()->Accept(*this);
}

void DefaultVisitor::Visit(InvokeNode& node)
{
    node.Child()->Accept(*this);
    for (const auto& arg : node.Args())
    {
        arg->Accept(*this);
    }
}

void DefaultVisitor::Visit(MemberAccessNode& node)
{
    node.Child()->Accept(*this);
    node.Member()->Accept(*this);
}

void DefaultVisitor::Visit(PtrMemberAccessNode& node)
{
    node.Child()->Accept(*this);
    node.Member()->Accept(*this);
}

void DefaultVisitor::Visit(PostIncrementNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(PostDecrementNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(PostCastNode& node)
{
    node.Child()->Accept(*this);
    node.GetCppCastNode()->Accept(*this);
    node.Type()->Accept(*this);
}

void DefaultVisitor::Visit(TypeIdExprNode& node) 
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(PreIncrementNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(PreDecrementNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(UnaryOpExprNode& node) 
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(SizeOfNode& node) 
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(CastNode& node)
{
    node.Child()->Accept(*this);
    node.Type()->Accept(*this);
}

void DefaultVisitor::Visit(BinaryOpExprNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ConditionalNode& node)
{
    node.Condition()->Accept(*this);
    node.ThenExpr()->Accept(*this);
    node.ElseExpr()->Accept(*this);
}

void DefaultVisitor::Visit(ThrowExprNode& node)
{
    node.Exception()->Accept(*this);
}

void DefaultVisitor::Visit(NewNode& node)
{
    for (const auto& placementNode : node.Placement())
    {
        placementNode->Accept(*this);
    }
    node.TypeId()->Accept(*this);
    for (const auto& initializerNode : node.Initializer())
    {
        initializerNode->Accept(*this);
    }
}

void DefaultVisitor::Visit(DeleteNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(ParenExprNode& node)
{
    node.Child()->Accept(*this);
}

void DefaultVisitor::Visit(LabeledStatementNode& node)
{
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(CaseStatementNode& node)
{
    node.CaseExpr()->Accept(*this);
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(DefaultStatementNode& node)
{
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(ExpressionStatementNode& node)
{
    node.Expr()->Accept(*this);
}

void DefaultVisitor::Visit(CompoundStatementNode& node)
{
    for (const auto& stmt : node.Statements())
    {
        stmt->Accept(*this);
    }
}

void DefaultVisitor::Visit(IfStatementNode& node)
{
    node.Cond()->Accept(*this);
    node.ThenStmt()->Accept(*this);
    if (node.ElseStmt())
    {
        node.ElseStmt()->Accept(*this);
    }
}

void DefaultVisitor::Visit(SwitchStatementNode& node)
{
    node.Cond()->Accept(*this);
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(WhileStatementNode& node)
{
    node.Cond()->Accept(*this);
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(DoStatementNode& node)
{
    node.Cond()->Accept(*this);
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(ForStatementNode& node)
{
    if (node.Init())
    {
        node.Init()->Accept(*this);
    }
    if (node.Cond())
    {
        node.Cond()->Accept(*this);
    }
    if (node.Iter())
    {
        node.Iter()->Accept(*this);
    }
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(ReturnStatementNode& node)
{
    if (node.Expr())
    {
        node.Expr()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ConditionWithDeclaratorNode& node)
{
    node.Type()->Accept(*this);
    node.Expression()->Accept(*this);
}

void DefaultVisitor::Visit(ForRangeDeclarationNode& node) 
{
    node.TypeId()->Accept(*this);
}

void DefaultVisitor::Visit(RangeForStatementNode& node)
{
    node.Declaration()->Accept(*this);
    node.Container()->Accept(*this);
    node.Stmt()->Accept(*this);
}

void DefaultVisitor::Visit(DeclarationStatementNode& node)
{
    node.Declaration()->Accept(*this);
}

void DefaultVisitor::Visit(ExceptionDeclarationNode& node)
{
    if (node.TypeId())
    {
        node.TypeId()->Accept(*this);
    }
}

void DefaultVisitor::Visit(HandlerNode& node)
{
    node.ExceptionDeclaration()->Accept(*this);
    node.HandlerBlock()->Accept(*this);
}

void DefaultVisitor::Visit(TryStatementNode& node)
{
    node.TryBlock()->Accept(*this);
    for (const auto& handler : node.Handlers())
    {
        handler->Accept(*this);
    }
}

void DefaultVisitor::Visit(IfdefStatementNode& node)
{
    node.Symbol()->Accept(*this);
}

void DefaultVisitor::Visit(EndIfStatementNode& node)
{
    if (node.Comment())
    {
        node.Comment()->Accept(*this);
    }
}

void DefaultVisitor::Visit(AssignInitNode& node)
{
    if (node.AssignmentExpr())
    {
        node.AssignmentExpr()->Accept(*this);
    }
    for (const auto& subInit : node.SubInits())
    {
        subInit->Accept(*this);
    }
}

void DefaultVisitor::Visit(InitializerNode& node)
{
    if (node.GetAssignInit())
    {
        node.GetAssignInit()->Accept(*this);
    }
    for (const auto& expr : node.GetExpressionList())
    {
        expr->Accept(*this);
    }
}

void DefaultVisitor::Visit(InitDeclaratorNode& node)
{
    if (node.GetInitializer())
    {
        node.GetInitializer()->Accept(*this);
    }
}

void DefaultVisitor::Visit(InitDeclaratorListNode& node)
{
    for (const auto& initDeclarator : node.InitDeclarators())
    {
        initDeclarator->Accept(*this);
    }
}

void DefaultVisitor::Visit(TypeNameNode& node)
{
    for (const auto& templateArg : node.TemplateArgs())
    {
        templateArg->Accept(*this);
    }
}

void DefaultVisitor::Visit(TypeNode& node)
{
    for (const auto& typeSpecifier : node.TypeSpecifiers())
    {
        typeSpecifier->Accept(*this);
    }
}

void DefaultVisitor::Visit(TypeIdNode& node)
{
    for (const auto& typeSpecifier : node.TypeSpecifiers())
    {
        typeSpecifier->Accept(*this);
    }
}

void DefaultVisitor::Visit(SimpleDeclarationNode& node)
{
    for (const auto& declSpecifier : node.DeclSpecifiers())
    {
        declSpecifier->Accept(*this);
    }
    node.GetInitDeclaratorList()->Accept(*this);
}

CodeEvaluationVisitor::CodeEvaluationVisitor() : hasReturn(false), hasPass(false), hasVars(false)
{
}

void CodeEvaluationVisitor::Visit(ReturnStatementNode& node)
{
    DefaultVisitor::Visit(node);
    hasReturn = true;
}

void CodeEvaluationVisitor::Visit(IdExprNode& node)
{
    Visitor::Visit(node);
    if (node.Id() == "pass")
    {
        hasPass = true;
    }
    else if (node.Id() == "vars")
    {
        hasVars = true;
    }
}

} // namespace soul::ast::cpp
