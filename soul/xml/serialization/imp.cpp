// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.serialization.imp;

import util.unicode;
import soul.xml.xpath.evaluate;
import soul.xml.xpath.object;

namespace soul::xml::serialization {

void Parse(const std::string& str, bool& value)
{
    if (str == "true")
    {
        value = true;
    }
    else if (str == "false")
    {
        value = false;
    }
    else
    {
        throw std::runtime_error("cannot parse Boolean from string '" + str + "'");
    }
}

void Parse(const std::string& str, int8_t& value)
{
    value = static_cast<int8_t>(std::stoi(str));
}

void Parse(const std::string& str, uint8_t& value)
{
    value = static_cast<uint8_t>(std::stoi(str));
}

void Parse(const std::string& str, int16_t& value)
{
    value = static_cast<int16_t>(std::stoi(str));
}

void Parse(const std::string& str, uint16_t& value)
{
    value = static_cast<uint16_t>(std::stoi(str));
}

void Parse(const std::string& str, int32_t& value)
{
    value = static_cast<int32_t>(std::stoi(str));
}

void Parse(const std::string& str, uint32_t& value)
{
    value = static_cast<uint32_t>(std::stoi(str));
}

void Parse(const std::string& str, int64_t& value)
{
    value = static_cast<int64_t>(std::stoll(str));
}

void Parse(const std::string& str, uint64_t& value)
{
    value = static_cast<uint64_t>(std::stoll(str));
}

void Parse(const std::string& str, float& value)
{
    value = std::stof(str);
}

void Parse(const std::string& str, double& value)
{
    value = std::stod(str);
}

void Parse(const std::string& str, char& value)
{
    if (!str.empty())
    {
        value = str.front();
    }
    else
    {
        value = char();
    }
}

void Parse(const std::string& str, char16_t& value)
{
    if (!str.empty())
    {
        std::u16string s = util::ToUtf16(str);
        value = s.front();
    }
    else
    {
        value = char16_t();
    }
}

void Parse(const std::string& str, char32_t& value)
{
    if (!str.empty())
    {
        std::u32string s = util::ToUtf32(str);
        value = s.front();
    }
    else
    {
        value = char32_t();
    }
}

void Parse(const std::string& str, date& value)
{
    value = util::ParseDate(str);
}

void Parse(const std::string& str, datetime& value)
{
    value = util::ParseDateTime(str);
}

void Parse(const std::string& str, time_point& value)
{
    int64_t v = 0;
    Parse(str, v);
    value = time_point(duration(v));
}

void Parse(const std::string& str, duration& value)
{
    int64_t v = 0;
    Parse(str, v);
    value = duration(v);
}

void Parse(const std::string& str, uuid& value) 
{
    value = util::ParseUuid(str);
}

void Parse(const std::string& str, std::string& value)
{
    value = str;
}

void Parse(const std::string& str, std::u16string& value)
{
    value = util::ToUtf16(str);
}

void Parse(const std::string& str, std::u32string& value)
{
    value = util::ToUtf32(str);
}

soul::xml::Element* GetFieldElement(const std::string& fieldName, soul::xml::Element* fromElement)
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet(fieldName, fromElement);
    if (nodeSet->Count() == 1)
    {
        soul::xml::Node* node = nodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* fieldElement = static_cast<soul::xml::Element*>(node);
            return fieldElement;
        }
    }
    return nullptr;
}

std::unique_ptr<soul::xml::xpath::NodeSet> GetItemNodeSet(soul::xml::Element* fieldElement)
{
    return soul::xml::xpath::EvaluateToNodeSet("item", fieldElement);
}

} // namespace soul::xml::serialization
