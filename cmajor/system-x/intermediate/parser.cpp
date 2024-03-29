// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.parser;

import cmajor.systemx.intermediate.code.lexer;
import cmajor.systemx.intermediate.code.parser.rules;
import cmajor.systemx.intermediate.code.parser;
import util;

namespace cmajor::systemx::intermediate {

void Parse(int logStreamId, const std::string& filePath, Context& context, bool verbose)
{
    if (verbose)
    {
        util::LogMessage(logStreamId, "> " + filePath);
    }
    int fileIndex = context.GetFileMap().MapFile(filePath);
    std::u32string content;
    try
    {
        content = util::ToUtf32(util::ReadFile(filePath));
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + filePath);
    }
    auto lexer = cmajor::systemx::intermediate::code::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), filePath);
    lexer.SetFile(fileIndex);
    using LexerType = decltype(lexer);
    lexer.SetRuleNameMapPtr(cmajor::systemx::intermediate::code::parser::rules::GetRuleNameMapPtr());
    cmajor::systemx::intermediate::code::parser::IntermediateCodeParser<LexerType>::Parse(lexer, &context);
    context.GetFileMap().AddFileContent(fileIndex, std::move(content), lexer.GetLineStartIndeces());
}

} // cmajor::systemx::intermediate
