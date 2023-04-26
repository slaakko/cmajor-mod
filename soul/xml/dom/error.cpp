// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.error;

namespace soul::xml {

XmlException::XmlException(const std::string& errorMessage_, const soul::ast::SourcePos& sourcePos_) : std::runtime_error(errorMessage_), sourcePos(sourcePos_)
{
}

} // namespace soul::xml
