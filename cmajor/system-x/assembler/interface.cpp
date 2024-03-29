// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.interface;

import cmajor.systemx.assembler.assembler;
import cmajor.systemx.assembler.lexer;
import cmajor.systemx.assembler.parser;
import cmajor.systemx.assembler.parser.rules;
import util;

namespace cmajor::systemx::assembler {

void Assemble(int logStreamId, const std::string& assemblyFilePath, const std::string& objectFilePath, bool verbose)
{
    if (verbose)
    {
        util::LogMessage(logStreamId, "> " + assemblyFilePath);
    }
    std::u32string assemblyContent; 
    try
    {
        assemblyContent = util::ToUtf32(util::ReadFile(assemblyFilePath));
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + assemblyFilePath);
    }
    if (!assemblyContent.empty())
    {
        auto lexer = cmajor::systemx::assembler::lexer::MakeLexer(assemblyContent.c_str(), assemblyContent.c_str() + assemblyContent.length(), assemblyFilePath);
        cmajor::systemx::assembler::assembler::Assembler assembler(assemblyFilePath, objectFilePath);
        int fileIndex = assembler.GetFileMap().MapFile(assemblyFilePath);
        lexer.SetFile(fileIndex);
        using LexerType = decltype(lexer);
        lexer.SetRuleNameMapPtr(cmajor::systemx::assembler::parser::rules::GetRuleNameMapPtr());
        cmajor::systemx::assembler::parser::AssemblyParser<LexerType>::Parse(lexer, &assembler);
        assembler.Assemble();
        assembler.GetFileMap().AddFileContent(fileIndex, std::move(assemblyContent), lexer.GetLineStartIndeces());
    }
    if (verbose)
    {
        util::LogMessage(logStreamId, "==> " + objectFilePath);
    }
}

} // namespace cmajor::systemx::assembler
