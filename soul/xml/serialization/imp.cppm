// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.serialization.imp;

import std.core;
import util.time;
import util.uuid;
import soul.xml.element;
import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.xpath.object;
import soul.xml.serialization.class_registry;

export namespace soul::xml::serialization {

using date = util::Date;
using datetime = util::DateTime;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::steady_clock::duration;
using uuid = util::uuid;

void Parse(const std::string& str, bool& value);
void Parse(const std::string& str, int8_t& value);
void Parse(const std::string& str, uint8_t& value);
void Parse(const std::string& str, int16_t& value);
void Parse(const std::string& str, uint16_t& value);
void Parse(const std::string& str, int32_t& value);
void Parse(const std::string& str, uint32_t& value);
void Parse(const std::string& str, int64_t& value);
void Parse(const std::string& str, uint64_t& value);
void Parse(const std::string& str, float& value);
void Parse(const std::string& str, double& value);
void Parse(const std::string& str, char& value);
void Parse(const std::string& str, char16_t& value);
void Parse(const std::string& str, char32_t& value);
void Parse(const std::string& str, date& value);
void Parse(const std::string& str, datetime& value);
void Parse(const std::string& str, time_point& value);
void Parse(const std::string& str, duration& value);
void Parse(const std::string& str, uuid& value);
void Parse(const std::string& str, std::string& value);
void Parse(const std::string& str, std::u16string& value);
void Parse(const std::string& str, std::u32string& value);

soul::xml::Element* GetFieldElement(const std::string& fieldName, soul::xml::Element* fromElement);
std::unique_ptr<soul::xml::xpath::NodeSet> GetItemNodeSet(soul::xml::Element* fieldElement);

template<typename T>
concept XmlImportableScalarType =
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
concept XmlImportableClassType =
std::is_class_v<T> && requires(T t, soul::xml::Element* element) { t.FromXml(element); };

template<typename T>
concept XmlImportableEnumType = std::is_enum_v<T>;

template<typename T>
concept XmlImportableType = XmlImportableScalarType<T> || XmlImportableClassType<T> || XmlImportableEnumType<T>;

template<XmlImportableScalarType T>
void FromXml(soul::xml::Element* element, T& value)
{
    value = T();
    if (element)
    {
        Parse(element->GetAttribute("value"), value);
    }
}

template<XmlImportableScalarType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, T& value)
{
    FromXml(GetFieldElement(fieldName, parentElement), value);
}

template<XmlImportableClassType T>
void FromXml(soul::xml::Element* element, T& value)
{
    value = T();
    if (element)
    {
        value.FromXml(element);
    }
}

template<XmlImportableClassType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, T& value)
{
    FromXml(GetFieldElement(fieldName, parentElement), value);
}

template<XmlImportableEnumType T>
void FromXml(soul::xml::Element* element, T& value)
{
    value = T();
    if (element)
    {
        int val = 0;
        FromXml(element, val);
        value = static_cast<T>(val);
    }
}

template<XmlImportableEnumType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, T& value)
{
    FromXml(GetFieldElement(fieldName, parentElement), value);
}

template<XmlImportableClassType T>
void FromXml(soul::xml::Element* element, std::unique_ptr<T>& value)
{
    value.reset();
    if (element)
    {
        std::string val = element->GetAttribute("value");
        if (val != "null")
        {
            std::string className = element->GetAttribute("className");
            if (!className.empty())
            {
                T* v = soul::xml::serialization::CreateObject<T>(className);
                value.reset(v);
                value->FromXml(element);
            }
            else
            {
                value.reset(new T());
                value->FromXml(element);
            }
        }
    }
}

template<XmlImportableClassType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, std::unique_ptr<T>& value)
{
    FromXml(GetFieldElement(fieldName, parentElement), value);
}

template<XmlImportableType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, std::vector<T>& value)
{
    value.clear();
    soul::xml::Element* fieldElement = GetFieldElement(fieldName, parentElement);
    if (fieldElement)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = GetItemNodeSet(fieldElement);
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                T item;
                FromXml(element, item);
                value.push_back(std::move(item));
            }
        }
    }
}

template<XmlImportableClassType T>
void FromXml(soul::xml::Element* parentElement, const std::string& fieldName, std::vector<std::unique_ptr<T>>& value)
{
    value.clear();
    soul::xml::Element* fieldElement = GetFieldElement(fieldName, parentElement);
    if (fieldElement)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = GetItemNodeSet(fieldElement);
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string val = element->GetAttribute("value");
                if (val != "null")
                {
                    std::string className = element->GetAttribute("className");
                    if (!className.empty())
                    {
                        T* v = soul::xml::serialization::CreateObject<T>(className);
                        std::unique_ptr<T> uv;
                        uv.reset(v);
                        uv->FromXml(element);
                        value.push_back(std::move(uv));
                    }
                    else
                    {
                        std::unique_ptr<T> uv;
                        uv.reset(new T());
                        uv->FromXml(element);
                        value.push_back(std::move(uv));
                    }
                }
            }
        }
    }
}

template<XmlImportableClassType T>
std::unique_ptr<T> MakeObject(soul::xml::Document& xmlDoc)
{
    std::unique_ptr<T> value;
    FromXml(xmlDoc.DocumentElement(), value);
    return value;
}

template<XmlImportableClassType T>
std::unique_ptr<T> MakeObject(const std::string& xmlString, const std::string& systemId)
{
    std::unique_ptr<soul::xml::Document> doc = soul::xml::ParseXmlContent(xmlString, systemId);
    return MakeObject<T>(*doc);
}

} // namespace soul::xml::serialization
