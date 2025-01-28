// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.macro;

import cmajor.masm.assembly.value;
import std.core;

export namespace cmajor::masm::assembly {

class Macro : public Value
{
public:
    Macro(const std::string& name_);
    void SetValue(Value* value_);
    Value* GetValue() const { return value; }
    std::string ToString() const override;
private:
    Value* value;
};

} // namespace cmajor::masm::assembly
