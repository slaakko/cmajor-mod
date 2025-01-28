// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_CDATA_SECTION_INCLUDED
#define DOM_CDATA_SECTION_INCLUDED
#include <dom/text.hpp>

namespace soul::xml {

class CDataSection : public Text
{
public:
    CDataSection(const soul::ast::SourcePos& sourcePos_);
    CDataSection(const soul::ast::SourcePos& sourcePos_, const std::string& text_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

CDataSection* MakeCDataSection(const std::string& text);

} // namespace soul::xml

#endif // DOM_CDATA_SECTION_INCLUDED
