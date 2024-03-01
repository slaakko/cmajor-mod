// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.text;

import soul.xml.visitor;

namespace soul::xml {

Text::Text(const soul::ast::SourcePos& sourcePos_) : CharacterData(NodeKind::textNode, sourcePos_, "text")
{
}

Text::Text(const soul::ast::SourcePos& sourcePos_, const std::string& text_) : CharacterData(NodeKind::textNode, sourcePos_, "text", text_)
{
}

Text::Text(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_) : CharacterData(kind_, sourcePos_, name_)
{
}

Text::Text(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& text_) : CharacterData(kind_, sourcePos_, name_, text_)
{
}

void Text::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Text* MakeText(const std::string& text)
{
    return new Text(soul::ast::SourcePos(), text);
}

} // namespace soul::xml
