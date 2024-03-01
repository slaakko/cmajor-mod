// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_PARSING_LOG_INCLUDED
#define LEXER_PARSING_LOG_INCLUDED
#include <string>

namespace soul::lexer {

class ParsingLog
{
public:
    ParsingLog();
    ParsingLog(int maxLineLength_);
    virtual ~ParsingLog();
    virtual void IncIndent() = 0;
    virtual void DecIndent() = 0;
    virtual void WriteBeginRule(const std::string& ruleName) = 0;
    virtual void WriteEndRule(const std::string& ruleName) = 0;
    virtual void WriteTry(const std::string& s) = 0;
    virtual void WriteSuccess(const std::string& match) = 0;
    virtual void WriteFail() = 0;
    virtual int MaxLineLength() const;
private:
    int maxLineLength;
};

} // namespace soul::lexer

#endif // LEXER_PARSING_LOG_INCLUDED
