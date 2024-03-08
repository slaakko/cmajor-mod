// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.expr;

namespace cmajor::masm::assembly {

std::string MakeBinaryExprStr(Value* left, Value* right, Operator op)
{
    switch (op)
    {
    case Operator::add:
    {
        return left->ToString() + "+" + right->ToString();
    }
    case Operator::sub:
    {
        return left->ToString() + "-" + right->ToString();
    }
    case Operator::mul:
    {
        return left->ToString() + "*" + right->ToString();
    }
    }
    return std::string();
}

UnaryExpr::UnaryExpr(Value* value_, const std::string& str_) : Value(str_), value(value_)
{
}

BinaryExpr::BinaryExpr(Value* left_, Value* right_, Operator op_) : Value(MakeBinaryExprStr(left_, right_, op_)), left(left_), right(right_), op(op_)
{
}

std::string BinaryExpr::ToString() const
{
    return MakeBinaryExprStr(left, right, op);
}

Content::Content(Value* value_) : UnaryExpr(value_, "[" + value_->ToString() + "]")
{
}

std::string Content::ToString() const
{
    return "[" + GetValue()->ToString() + "]";
}

std::string SizePrefixStr(int size, Value* value)
{
    switch (size)
    {
    case 1: return "byte ptr " + value->ToString();
    case 2: return "word ptr " + value->ToString();
    case 4: return "dword ptr " + value->ToString();
    case 8: return "qword ptr " + value->ToString();
    }
    return std::string();
}

SizePrefix::SizePrefix(int size_, Value* value_) : UnaryExpr(value_, SizePrefixStr(size_, value_)), size(size_)
{
}

std::string SizePrefix::ToString() const
{
    return SizePrefixStr(size, GetValue());
}

} // namespace cmajor::masm::assembly
