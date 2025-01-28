// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.main.parser;

import cmajor.masm.intermediate.lexer;
import cmajor.masm.intermediate.parser.rules;
import cmajor.masm.intermediate.parser;
import util;

namespace cmajor::masm::intermediate {

void Parse(int logStreamId, const std::string& filePath, Context& context, bool verbose)
{
    if (verbose)
    {
        util::LogMessage(logStreamId, "> " + filePath);
    }
    int fileIndex = context.GetFileMap().MapFile(filePath);
    context.SetFileId(fileIndex);
    std::u32string content;
    try
    {
        content = util::ToUtf32(util::ReadFile(filePath));
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + filePath);
    }
    auto lexer = cmajor::masm::intermediate::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), filePath);
    lexer.SetFile(fileIndex);
    using LexerType = decltype(lexer);
    lexer.SetRuleNameMapPtr(cmajor::masm::intermediate::parser::rules::GetRuleNameMapPtr());
    cmajor::masm::intermediate::parser::IntermediateParser<LexerType>::Parse(lexer, &context);
    context.GetFileMap().AddFileContent(fileIndex, std::move(content), lexer.GetLineStartIndeces());
}

} // cmajor::masm::intermediate
