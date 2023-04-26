// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.error;

import std.core;
import soul.lexer.file.map;
import soul.ast.source.pos;

export namespace soul::xml {

class XmlException : public std::runtime_error
{
public:
    XmlException(const std::string& errorMessage_, const soul::ast::SourcePos& sourcePos_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
private:
    soul::ast::SourcePos sourcePos;
};

} // namespace soul::xml
