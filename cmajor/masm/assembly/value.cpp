// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.value;

namespace cmajor::masm::assembly {

Value::Value(ValueKind kind_, const std::string& name_) : kind(kind_), name(name_)
{
}

Value::~Value()
{
}

void Value::SetName(const std::string& name_)
{
    name = name_;
}

} // cmajor::masm::assembly
