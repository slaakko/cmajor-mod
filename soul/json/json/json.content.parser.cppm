
// this file has been automatically generated from 'C:/work/soul-mod/soul/json/json.content.parser' using soul parser generator spg version 5.0.0

export module soul.json.content.parser;

import std.core;
import soul.lexer;
import soul.parser;
import util.json;

using namespace util::json;

export namespace soul::json::content::parser {

template<typename LexerT>
struct JsonParser
{
    static std::unique_ptr<util::JsonValue> Parse(LexerT& lexer);
    static soul::parser::Match Value(LexerT& lexer);
    static soul::parser::Match Object(LexerT& lexer);
    static soul::parser::Match Field(LexerT& lexer, util::JsonObject* o);
    static soul::parser::Match Array(LexerT& lexer);
};

} // namespace soul::json::content::parser
