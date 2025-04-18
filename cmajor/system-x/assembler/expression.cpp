// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.expression;

import cmajor.systemx.assembler.visitor;
import util;

namespace cmajor::systemx::assembler {

UnaryExpression::UnaryExpression(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* operand_) :
    Node(NodeKind::unaryExprNode, sourcePos_), op(op_), operand(operand_)
{
#ifdef THREAD_ID_CHECK
    if (!operand || operand->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
}

void UnaryExpression::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnaryExpression::Write(util::CodeFormatter& formatter)
{
    switch (op)
    {
        case Operator::unaryPlus:
        {
            formatter.Write("+");
            break;
        }
        case Operator::unaryMinus:
        {
            formatter.Write("-");
            break;
        }
        case Operator::complement:
        {
            formatter.Write("~");
            break;
        }
        case Operator::reg:
        {
            formatter.Write("$");
            break;
        }
    }
#ifdef THREAD_ID_CHECK
    if (!operand || operand->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    operand->Write(formatter);
}

BinaryExpression::BinaryExpression(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* left_, Node* right_) :
    Node(NodeKind::binaryExprNode, sourcePos_), op(op_), left(left_), right(right_)
{
}

void BinaryExpression::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BinaryExpression::Write(util::CodeFormatter& formatter)
{
    left->Write(formatter);
    switch (op)
    {
        case Operator::multiply:
        {
            formatter.Write("*");
            break;
        }
        case Operator::divide:
        {
            formatter.Write("/");
            break;
        }
        case Operator::fractional_divide:
        {
            formatter.Write("//");
            break;
        }
        case Operator::modulus:
        {
            formatter.Write("%");
            break;
        }
        case Operator::shift_left:
        {
            formatter.Write("<<");
            break;
        }
        case Operator::shift_right:
        {
            formatter.Write(">>");
            break;
        }
        case Operator::bitwise_and:
        {
            formatter.Write("&");
            break;
        }
        case Operator::add:
        {
            formatter.Write("+");
            break;
        }
        case Operator::subtract:
        {
            formatter.Write("-");
            break;
        }
        case Operator::bitwise_or:
        {
            formatter.Write("|");
            break;
        }
        case Operator::bitwise_xor:
        {
            formatter.Write("^");
            break;
        }
    }
    right->Write(formatter);
}

ParenthesizedExpression::ParenthesizedExpression(const soul::ast::SourcePos& sourcePos_, Node* expr_) :
    Node(NodeKind::parenExprNode, sourcePos_), expr(expr_)
{
}

void ParenthesizedExpression::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedExpression::Write(util::CodeFormatter& formatter)
{
    formatter.Write("(");
    expr->Write(formatter);
    formatter.Write(")");
}

} // namespace cmajor::systemx::assembler
