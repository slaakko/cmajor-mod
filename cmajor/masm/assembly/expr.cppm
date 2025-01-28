// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.expr;

import cmajor.masm.assembly.value;
import std.core;

export namespace cmajor::masm::assembly {

enum class Operator
{
    add, sub, mul
};

class UnaryExpr : public Value
{
public:
    UnaryExpr(ValueKind kind_, Value* value_, const std::string& str_);
    Value* GetValue() const { return value; }
private:
    Value* value;
};

class BinaryExpr : public Value
{
public:
    BinaryExpr(Value* left_, Value* right_, Operator op_);
    Value* Left() { return left; }
    Value* Right() { return right; }
    Operator Op() const { return op; }
    std::string ToString() const override;
private:
    Value* left;
    Value* right;
    Operator op;
};

class Content : public UnaryExpr
{
public:
    Content(Value* value_);
    std::string ToString() const override;
};

class SizePrefix : public UnaryExpr
{
public:
    SizePrefix(int size_, Value* value_);
    int Size() const { return size; }
    std::string ToString() const override;
private:
    int size;
};

} // namespace otava::assembly
