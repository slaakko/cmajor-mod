// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.source.token;

import std.core;

export namespace cmajor::ast {

class SourceTokenFormatter
{
public:
    virtual ~SourceTokenFormatter();
    virtual void BeginFormat() {}
    virtual void EndFormat() {}
    virtual void Keyword(const std::string& keyword) {}
    virtual void Identifier(const std::string& identifier) {}
    virtual void Number(const std::string& number) {}
    virtual void Char(const std::string& char_) {}
    virtual void String(const std::string& string) {}
    virtual void Spaces(const std::string& spaces) {}
    virtual void Comment(const std::string& comment) {}
    virtual void NewLine(const std::string& newline) {}
    virtual void Other(const std::string& other) {}
};
} // namespace cmajor::ast
