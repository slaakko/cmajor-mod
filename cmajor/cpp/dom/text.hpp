// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_TEXT_INCLUDED
#define DOM_TEXT_INCLUDED
#include <dom/character_data.hpp>

namespace soul::xml {

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

#endif // DOM_TEXT_INCLUDED
