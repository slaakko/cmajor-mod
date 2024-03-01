// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.error;

import cmajor.systemx.intermediate.context;
import cmajor.symbols;

namespace cmajor::systemx::intermediate {

void Error(const std::string& message, const soul::ast::SourcePos& sourcePos, Context* context)
{
    throw cmajor::symbols::Error(message + " at\n'" + context->FilePath() + "':" + std::to_string(sourcePos.line) + ":\n" +
        context->ErrorLines(sourcePos));
}

void Error(const std::string& message, const soul::ast::SourcePos& sourcePos, Context* context, const soul::ast::SourcePos& refSourcePos)
{
    throw cmajor::symbols::Error(message + " at\n'" + context->FilePath() + "':" + std::to_string(sourcePos.line) + ":\n" +
        context->ErrorLines(sourcePos) + ": see reference line " + std::to_string(refSourcePos.line) + ":\n" +
        context->ErrorLines(refSourcePos));
}

} // cmajor::systemx::intermediate
