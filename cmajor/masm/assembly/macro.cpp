// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.macro;

namespace cmajor::masm::assembly {

Macro::Macro(const std::string& name_) : Value(ValueKind::macro, name_), value(nullptr)
{
}

void Macro::SetValue(Value* value_)
{
    value = value_;
}

std::string Macro::ToString() const
{
    std::string s(Name());
    s.append(" = ").append(value->ToString());
    return s;
}

} // namespace cmajor::masm::assembly

