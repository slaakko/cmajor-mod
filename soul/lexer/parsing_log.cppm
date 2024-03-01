// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.parsing.log;

import std.core;

export namespace soul::lexer {

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
