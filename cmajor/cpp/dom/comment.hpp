// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_COMMENT_INCLUDED
#define DOM_COMMENT_INCLUDED
#include <dom/character_data.hpp>

namespace soul::xml {

class Comment : public CharacterData
{
public:
    Comment(const soul::ast::SourcePos& sourcePos_);
    Comment(const soul::ast::SourcePos& sourcePos_, const std::string& comment_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

Comment* MakeComment(const std::string& comment);

} // namespace soul::xml

#endif // DOM_COMMENT_INCLUDED
