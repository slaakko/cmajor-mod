// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.text;

import std.core;
import soul.xml.character.data;
import soul.xml.node;

export namespace soul::xml {

class Text : public CharacterData
{
public:
    Text(const soul::ast::SourcePos& sourcePos_);
    Text(const soul::ast::SourcePos& sourcePos_, const std::string& text_);
    Text(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    Text(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& text_);
    void Accept(Visitor& visitor) override;
};

Text* MakeText(const std::string& text);

} // namespace soul::xml
