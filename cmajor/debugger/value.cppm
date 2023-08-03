// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.value;

import soul.xml.dom;
import std.core;

export namespace cmajor::debugger {

export namespace value {}

enum class ValueKind
{
    string, tuple, list
};

class Value
{
public:
    Value(ValueKind kind_);
    virtual ~Value();
    ValueKind Kind() const { return kind; }
    bool IsString() const { return kind == ValueKind::string; }
    bool IsTuple() const { return kind == ValueKind::tuple; }
    bool IsList() const { return kind == ValueKind::list; }
    virtual std::string ToString() const = 0;
    virtual std::unique_ptr<soul::xml::Element> ToXml(const std::string& elementName) const = 0;
private:
    ValueKind kind;
};

class String : public Value
{
public:
    String(const std::string& text_);
    const std::string& Text() const { return text; }
    std::string ToString() const override { return text; }
    std::unique_ptr<soul::xml::Element> ToXml(const std::string& elementName) const override;
private:
    std::string text;
};

class Tuple : public Value
{
public:
    Tuple();
    void Add(const std::string& key, Value* value);
    Value* Get(const std::string& key) const;
    std::string GetString(const std::string& key) const;
    int GetInt(const std::string& key) const;
    std::string ToString() const override;
    std::unique_ptr<soul::xml::Element> ToXml(const std::string& elementName) const override;
private:
    std::map<std::string, std::unique_ptr<Value>> map;
};

class Item
{
public:
    Item(const std::string& name_, Value* value_);
    const std::string& Name() const { return name; }
    Value* GetValue() const { return value.get(); }
    Value* ReleaseValue() { return value.release(); }
    std::string ToString() const;
    std::unique_ptr<soul::xml::Element> ToXml() const;
private:
    std::string name;
    std::unique_ptr<Value> value;
};

class List : public Value
{
public:
    List();
    void Add(Item* item);
    const std::vector<std::unique_ptr<Item>>& Items() const { return items; }
    std::string ToString() const override;
    std::unique_ptr<soul::xml::Element> ToXml(const std::string& elementName) const override;
private:
    std::vector<std::unique_ptr<Item>> items;
};

class Results
{
public:
    Results();
    bool IsEmpty() const { return items.empty(); }
    void Add(Item* item);
    Value* Get(const std::string& key) const;
    std::string GetString(const std::string& key) const;
    int GetInt(const std::string& key) const;
    std::string ToString() const;
    std::unique_ptr<soul::xml::Element> ToXml() const;
private:
    std::vector<std::unique_ptr<Item>> items;
    std::map<std::string, Value*> map;
};

} // namespace cmajor::debugger
