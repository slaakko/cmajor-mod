// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.json;

import std.core;

export namespace util {

enum class JsonValueType
{
    object, array, string, number, boolean, null
};

class CodeFormatter;

class JsonValue
{
public:
    JsonValue(JsonValueType type_);
    JsonValue(const JsonValue&) = delete;
    JsonValue& operator=(const JsonValue&) = delete;
    virtual ~JsonValue();
    virtual JsonValue* Clone() const = 0;
    JsonValueType Type() const { return type; }
    virtual std::string ToString() const = 0;
    virtual void Write(CodeFormatter& formatter);
private:
    JsonValueType type;
};

class JsonString : public JsonValue
{
public:
    JsonString();
    JsonString(const std::u32string& value_);
    void Append(char32_t c);
    JsonValue* Clone() const override;
    const std::u32string& Value() const { return value; }
    void SetValue(const std::u32string& value_);
    std::u16string JsonCharStr(char32_t c) const;
    std::string ToString() const override;
private:
    std::u32string value;
};

class JsonNumber : public JsonValue
{
public:
    JsonNumber();
    JsonNumber(double value_);
    JsonValue* Clone() const override;
    double Value() const { return value; }
    std::string ToString() const override;
private:
    double value;
};

class JsonBool : public JsonValue
{
public:
    JsonBool();
    JsonBool(bool value_);
    JsonValue* Clone() const override;
    bool Value() const { return value; }
    std::string ToString() const override;
private:
    bool value;
};

class JsonObject : public JsonValue
{
public:
    JsonObject();
    void AddField(const std::u32string& fieldName, std::unique_ptr<JsonValue>&& fieldValue);
    JsonValue* GetField(const std::u32string& fieldName);
    std::string GetStringField(const std::u32string& fieldName);
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> fieldValues;
    std::map<std::u32string, JsonValue*> fieldMap;
};

class JsonArray : public JsonValue
{
public:
    JsonArray();
    void AddItem(std::unique_ptr<JsonValue>&& item);
    int Count() const { return items.size(); }
    JsonValue* operator[](int index) const;
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> items;
};

class JsonNull : public JsonValue
{
public:
    JsonNull();
    JsonValue* Clone() const override;
    std::string ToString() const override;
};

} // namespace util
