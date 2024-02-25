// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.parser;

import soul.json.lexer;
import soul.json.content.parser;
import util;

namespace soul::json {

std::unique_ptr<util::JsonValue> ParseJsonValue(const std::u32string& jsonText, const std::string& fileName)
{
    auto lexer = soul::json::lexer::MakeLexer(jsonText.c_str(), jsonText.c_str() + jsonText.length(), fileName);
    using LexerType = decltype(lexer);
    std::unique_ptr<util::JsonValue> jsonValue = soul::json::content::parser::JsonParser<LexerType>::Parse(lexer);
    return jsonValue;
}

std::unique_ptr<util::JsonValue> ParseJsonValue(const std::string& jsonText, const std::string& fileName)
{
    return ParseJsonValue(util::ToUtf32(jsonText), fileName);
}

std::unique_ptr<util::JsonValue> ParseJsonValue(const std::string& jsonText)
{
    return ParseJsonValue(jsonText, "<inline content>");
}

std::unique_ptr<util::JsonObject> ParseJsonObject(const std::string& jsonText, const std::string& fileName)
{
    std::unique_ptr<util::JsonValue> jsonValue = ParseJsonValue(jsonText, fileName);
    if (jsonValue->IsObject())
    {
        return std::unique_ptr<util::JsonObject>(static_cast<util::JsonObject*>(jsonValue.release()));
    }
    else
    {
        throw std::runtime_error("JSON object expected in file '" + fileName + "'");
    }
}

std::unique_ptr<util::JsonObject> ParseJsonObject(const std::string& jsonText)
{
    return ParseJsonObject(jsonText, "<inline content>");
}

} // namespace soul::json
