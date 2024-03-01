// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.di.enum_constant;

import util;
import std.core;

export namespace cmajor::debug {

class DIEnumConstant
{
public:
    DIEnumConstant();
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    void SetName(const std::string& name_);
    const std::string& Name() const { return name; }
    void SetValue(const std::string& value_);
    const std::string& Value() const { return value; }
    void SetStrValue(const std::string& strValue_);
    const std::string& StrValue() const { return strValue; }
private:
    std::string name;
    std::string value;
    std::string strValue;
};

} // namespace cmajor::debug
