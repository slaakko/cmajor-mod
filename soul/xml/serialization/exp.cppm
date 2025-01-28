// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.serialization.exp;

import std.core;
import util.time;
import util.uuid;
import soul.xml.element;

export namespace soul::xml::serialization {

using date = util::Date;
using datetime = util::DateTime;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::steady_clock::duration;
using uuid = util::uuid;

std::string ToString(bool value);
std::string ToString(int8_t value);
std::string ToString(uint8_t value);
std::string ToString(int16_t value);
std::string ToString(uint16_t value);
std::string ToString(int32_t value);
std::string ToString(uint32_t value);
std::string ToString(int64_t value);
std::string ToString(uint64_t value);
std::string ToString(float value);
std::string ToString(double value);
std::string ToString(char value);
std::string ToString(char16_t value);
std::string ToString(char32_t value);
std::string ToString(const date& value);
std::string ToString(const datetime& value);
std::string ToString(const time_point& value);
std::string ToString(const duration& value);
std::string ToString(const uuid& value);
inline const std::string& ToString(const std::string& value) { return value; }
std::string ToString(const std::u16string& value);
std::string ToString(const std::u32string& value);

template<typename T>
concept XmlExportableScalarType =
    std::is_fundamental_v<T> ||
    std::is_same_v<T, std::string> ||
    std::is_same_v<T, std::u16string> ||
    std::is_same_v<T, std::u32string> ||
    std::is_same_v<T, uuid> ||
    std::is_same_v<T, date> ||
    std::is_same_v<T, datetime> ||
    std::is_same_v<T, time_point> ||
    std::is_same_v<T, duration>;

template<typename T>
concept XmlExportableClassType = 
    std::is_class_v<T> && requires(T t, const std::string& fieldName) { t.ToXml(fieldName); };

template<class T>
concept XmlExportableEnumType = std::is_enum_v<T>;

template<typename T>
concept XmlExportableType = XmlExportableClassType<T> || XmlExportableScalarType<T> || XmlExportableEnumType<T>;
    
template<XmlExportableScalarType T>
soul::xml::Element* ToXml(const T& value, const std::string& fieldName)
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->SetAttribute("value", ToString(value));
    return element;
}

template<XmlExportableClassType T>
soul::xml::Element* ToXml(const T& value, const std::string& fieldName)
{
    return value.ToXml(fieldName);
}

template<XmlExportableEnumType T>
soul::xml::Element* ToXml(const T& value, const std::string& fieldName)
{
    return ToXml(static_cast<int>(value), fieldName);
}

template<XmlExportableClassType T>
soul::xml::Element* ToXml(const std::unique_ptr<T>& value, const std::string& fieldName)
{
    if (value)
    {
        soul::xml::Element* element = ToXml(*value, fieldName);
        std::string className = value->ClassName();
        if (className != T::StaticClassName())
        {
            element->SetAttribute("className", className);
        }
        return element;
    }
    else
    {
        soul::xml::Element* element = soul::xml::MakeElement(fieldName);
        element->SetAttribute("value", "null");
        return element;
    }
}

template<XmlExportableType T>
soul::xml::Element* ToXml(const std::vector<T>& v, const std::string& fieldName)
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    for (const auto& item : v)
    {
        element->AppendChild(ToXml(item, "item"));
    }
    return element;
}

template<XmlExportableClassType T>
soul::xml::Element* ToXml(const std::vector<std::unique_ptr<T>>& v, const std::string& fieldName)
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    for (const auto& item : v)
    {
        element->AppendChild(ToXml(item, "item"));
    }
    return element;
}

} // namespace soul::xml::serialization
