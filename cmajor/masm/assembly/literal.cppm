// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.literal;

import cmajor.masm.assembly.value;
import std.core;

export namespace cmajor::masm::assembly {

const int maxAssemblyLineLength = 128;

class IntegerLiteral : public Value
{
public:
    IntegerLiteral(int64_t value_, int size_);
    int64_t GetValue() const { return value; }
private:
    int64_t value;
    int size;
};

class FloatLiteral : public Value
{
public:
    FloatLiteral(float value_);
    float GetValue() const { return value; }
private:
    float value;
};

class DoubleLiteral : public Value
{
public:
    DoubleLiteral(double value_);
    double GetValue() const { return value; }
private:
    double value;
};

class StringLiteral : public Value
{
public:
    StringLiteral(const std::string& value_);
    bool CanSplit() const override { return true; }
    Value* Split(int length) override;
    bool IsEmpty() const override { return value.empty(); }
    const std::string& GetValue() const { return value; }
private:
    std::string value;
};

} // namespace cmajor::masm::assembly
