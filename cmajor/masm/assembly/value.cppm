// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.value;

import std.core;

export namespace cmajor::masm::assembly {

enum class ValueKind
{
    integerLiteral, floatLiteral, doubleLiteral, stringLiteral, symbol, macro, reg, binaryExpr, content, sizePrefix
};

class Value
{
public:
    Value(ValueKind kind_, const std::string& name_);
    virtual ~Value();
    ValueKind Kind() const { return kind; }
    bool IsIntegerLiteral() const { return kind == ValueKind::integerLiteral; }
    bool IsFloatLiteral() const { return kind == ValueKind::floatLiteral; }
    bool IsDoubletLiteral() const { return kind == ValueKind::doubleLiteral; }
    bool IsStringLiteral() const { return kind == ValueKind::stringLiteral; }
    bool IsSymbol() const { return kind == ValueKind::symbol; }
    bool IsMacro() const { return kind == ValueKind::macro; }
    bool IsRegister() const { return kind == ValueKind::reg; }
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    virtual std::string ToString() const { return Name(); }
    int Length() const { return ToString().length(); }
    virtual bool CanSplit() const { return false; }
    virtual Value* Split(int length) { return nullptr; }
    virtual bool IsEmpty() const { return false; }
private:
    ValueKind kind;
    std::string name;
};

} // namespace cmajor::masm::assembly
