// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.value;

namespace cmajor::debugger {

Value::Value(ValueKind kind_) : kind(kind_)
{
}

Value::~Value()
{
}

String::String(const std::string& text_) : Value(ValueKind::string), text(text_)
{
}

std::unique_ptr<soul::xml::Element> String::ToXml(const std::string& elementName) const
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement(elementName));
    element->SetAttribute("value", text);
    return element;
}

Tuple::Tuple() : Value(ValueKind::tuple)
{
}

void Tuple::Add(const std::string& key, Value* value)
{
    map[key] = std::unique_ptr<Value>(value);
}

Value* Tuple::Get(const std::string& key) const
{
    auto it = map.find(key);
    if (it != map.end())
    {
        return it->second.get();
    }
    return nullptr;
}

std::string Tuple::GetString(const std::string& key) const
{
    Value* value = Get(key);
    if (value)
    {
        return value->ToString();
    }
    return std::string();
}

int Tuple::GetInt(const std::string& key) const
{
    std::string str = GetString(key);
    if (!str.empty())
    {
        return std::stoi(str);
    }
    return 0;
}

std::string Tuple::ToString() const
{
    std::string str("{");
    bool first = true;
    for (const auto& x : map)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ',');
        }
        str.append(x.first).append(1, '=').append(x.second->ToString());
    }
    str.append(1, '}');
    return str;
}

std::unique_ptr<soul::xml::Element> Tuple::ToXml(const std::string& elementName) const
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement(elementName));
    for (const auto& x : map)
    {
        Value* value = x.second.get();
        element->AppendChild(value->ToXml(x.first).release());
    }
    return element;
}

Item::Item(const std::string& name_, Value* value_) : name(name_), value(value_)
{
}

std::string Item::ToString() const
{
    std::string str;
    str.append(name).append(1, '=').append(value->ToString());
    return str;
}

std::unique_ptr<soul::xml::Element> Item::ToXml() const
{
    if (name.empty())
    {
        return value->ToXml("item");
    }
    else
    {
        return value->ToXml(name);
    }
}

List::List() : Value(ValueKind::list)
{
}

void List::Add(Item* item)
{
    items.push_back(std::unique_ptr<Item>(item));
}

std::string List::ToString() const
{
    std::string str("[");
    bool first = true;
    int n = items.size();
    for (int i = 0; i < n; ++i)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ',');
        }
        str.append(items[i]->ToString());
    }
    str.append(1, ']');
    return str;
}

std::unique_ptr<soul::xml::Element> List::ToXml(const std::string& elementName) const
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement(elementName));
    for (const auto& x : items)
    {
        element->AppendChild(x->ToXml().release());
    }
    return element;
}

Results::Results()
{
}

void Results::Add(Item* item)
{
    items.push_back(std::unique_ptr<Item>(item));
    map[item->Name()] = item->GetValue();
}

Value* Results::Get(const std::string& key) const
{
    auto it = map.find(key);
    if (it != map.end())
    {
        return it->second;
    }
    return nullptr;
}

std::string Results::GetString(const std::string& key) const
{
    Value* value = Get(key);
    if (value)
    {
        return value->ToString();
    }
    return std::string();
}

std::string Results::ToString() const
{
    std::string str;
    bool first = true;
    for (const auto& item : items)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ',');
        }
        str.append(item->Name()).append(1, '=').append(item->GetValue()->ToString());
    }
    return str;
}

std::unique_ptr<soul::xml::Element> Results::ToXml() const
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("results"));
    for (const auto& x : items)
    {
        element->AppendChild(x->ToXml().release());
    }
    return element;
}

} // namespace cmajor::debugger
