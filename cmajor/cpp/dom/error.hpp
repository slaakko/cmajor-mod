// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_ERROR_INCLUDED
#define DOM_ERROR_INCLUDED
#include <ast/source_pos.hpp>
#include <string>
#include <stdexcept>

namespace soul::xml {

class XmlException : public std::runtime_error
{
public:
    XmlException(const std::string& errorMessage_, const soul::ast::SourcePos& sourcePos_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
private:
    soul::ast::SourcePos sourcePos;
};

} // namespace soul::xml

#endif // DOM_ERROR_INCLUDED
