// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.cdata.section;

import std.core;
import soul.xml.text;

export namespace soul::xml {

class CDataSection : public Text
{
public:
    CDataSection(const soul::ast::SourcePos& sourcePos_);
    CDataSection(const soul::ast::SourcePos& sourcePos_, const std::string& text_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
    Node* Clone(bool deep) const override;
};

CDataSection* MakeCDataSection(const std::string& text);

} // namespace soul::xml
