// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.error;

import util;
import soul.ast.source.pos;

namespace soul::lexer {

ParsingException::ParsingException(const std::string& message_, const std::string& fileName_, const soul::ast::SourcePos& sourcePos_) :
    std::runtime_error(message_), message(message_), fileName(fileName_), sourcePos(sourcePos)
{
}

void ParsingException::SetProject(const std::string& project_)
{
    project = project_;
}

std::string MakeMessage(const std::string& msgClass, const std::string& msg, const soul::ast::SourcePos& sourcePos, FileMap& fileMap)
{
    std::string message;
    message.append(msgClass).append(": ").append(msg).append(" in file '").append(fileMap.GetFilePath(sourcePos.file)).append("' line ").append(std::to_string(sourcePos.line));
    message.append(":\n").append(util::ToUtf8(fileMap.GetFileLine(sourcePos.file, sourcePos.line))).append("\n").append(sourcePos.col - 1, ' ').append(1, '^');
    return message;
}

} // namespace soul::lexer
