// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.di.enum_constant;

namespace cmajor::debug {

DIEnumConstant::DIEnumConstant() : name(), value(), strValue()
{
}

void DIEnumConstant::Write(util::BinaryStreamWriter& writer)
{
    writer.Write(name);
    writer.Write(value);
    writer.Write(strValue);
}

void DIEnumConstant::Read(util::BinaryStreamReader& reader)
{
    name = reader.ReadUtf8String();
    value = reader.ReadUtf8String();
    strValue = reader.ReadUtf8String();
}

void DIEnumConstant::SetName(const std::string& name_)
{
    name = name_;
}

void DIEnumConstant::SetValue(const std::string& value_)
{
    value = value_;
}

void DIEnumConstant::SetStrValue(const std::string& strValue_)
{
    strValue = strValue_;
}

} // namespace cmajor::debug
