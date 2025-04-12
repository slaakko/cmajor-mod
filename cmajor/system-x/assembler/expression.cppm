// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.expression;

import cmajor.systemx.assembler.node;
import std.core;

export namespace cmajor::systemx::assembler {

export namespace expression
{
}

enum class Operator : int
{
    unaryPlus, unaryMinus, complement, reg, serial,
    multiply, divide, fractional_divide, modulus, shift_left, shift_right, bitwise_and,
    add, subtract, bitwise_or, bitwise_xor
};

class UnaryExpression : public Node
{
public:
    UnaryExpression(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* operand_);
    UnaryExpression(const UnaryExpression&) = delete;
    UnaryExpression(UnaryExpression&&) = delete;
    UnaryExpression& operator=(const UnaryExpression&) = delete;
    UnaryExpression& operator=(UnaryExpression&&) = delete;
    Operator Op() const { return op; }
    Node* Operand() const { return operand.get(); }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    Operator op;
    std::unique_ptr<Node> operand;
};

class BinaryExpression : public Node
{
public:
    BinaryExpression(const soul::ast::SourcePos& sourcePos_, Operator op_, Node* left_, Node* right_);
    Operator Op() const { return op; }
    Node* Left() const { return left.get(); }
    Node* Right() const { return right.get(); }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    Operator op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class ParenthesizedExpression : public Node
{
public:
    ParenthesizedExpression(const soul::ast::SourcePos& sourcePos_, Node* expr_);
    Node* Expr() const { return expr.get(); }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::unique_ptr<Node> expr;
};

} // namespace cmajor::systemx::assembler
