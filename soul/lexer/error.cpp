// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.error;

namespace soul::lexer {

ParsingException::ParsingException(const std::string& message_, const std::string& fileName_, const SourcePos& sourcePos_) :
    std::runtime_error(message_), message(message_), fileName(fileName_), sourcePos(sourcePos)
{
}

void ParsingException::SetProject(const std::string& project_)
{
    project = project_;
}

} // namespace soul::lexer
