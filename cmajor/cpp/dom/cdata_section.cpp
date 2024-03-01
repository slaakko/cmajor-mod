// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <dom/cdata_section.hpp>
#include <dom/visitor.hpp>

namespace soul::xml {

CDataSection::CDataSection(const soul::ast::SourcePos& sourcePos_) : Text(NodeKind::cdataSectionNode, sourcePos_, "cdata_section")
{
}

CDataSection::CDataSection(const soul::ast::SourcePos& sourcePos_, const std::string& text_) : Text(NodeKind::cdataSectionNode, sourcePos_, "cdata_section", text_)
{
}

void CDataSection::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CDataSection::Write(util::CodeFormatter& formatter)
{
    formatter.Write("<![CDATA[");
    formatter.Write(Data());
    formatter.Write("]]>");
}

CDataSection* MakeCDataSection(const std::string& text)
{
    return new CDataSection(soul::ast::SourcePos(), text);
}

} // namespace soul::xml
