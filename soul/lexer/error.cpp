// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.error;

import util;

namespace soul::lexer {

ParsingException::ParsingException(const std::string& message_, const std::string& fileName_, const SourcePos& sourcePos_) :
    std::runtime_error(message_), message(message_), fileName(fileName_), sourcePos(sourcePos)
{
}

void ParsingException::SetProject(const std::string& project_)
{
    project = project_;
}

std::string MakeErrorMessage(const std::string& msg, const SourcePos& sourcePos, const FileMap& fileMap)
{
    std::string errorMessage;
    errorMessage.append("error: ").append(msg).append(" in file '").append(fileMap.GetFilePath(sourcePos.file)).append("' line ").append(std::to_string(sourcePos.line));
    if (fileMap.HasFileContent(sourcePos.file))
    {
        errorMessage.append(":\n").append(util::ToUtf8(fileMap.GetFileLine(sourcePos.file, sourcePos.line))).append("\n").append(sourcePos.col - 1, ' ').append(1, '^');
    }
    return errorMessage;
}

} // namespace soul::lexer
