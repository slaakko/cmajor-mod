// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.asm_file_parser;

import cmajor.sbin.assembly.lexer;
import cmajor.sbin.assembly.parser;
import soul.lexer;
import util;

namespace cmajor::sbin::assembly {

std::unique_ptr<AsmFileNode> ParseAsmFile(const std::string& asmFilePath, bool verbose, soul::lexer::FileMap& fileMap)
{
    int32_t fileIndex = fileMap.MapFile(asmFilePath);
    std::string asmFileContent = util::ReadFile(asmFilePath);
    std::u32string content = util::ToUtf32(asmFileContent);
    if (verbose)
    {
        util::LogMessage(-1, "> " + asmFilePath);
    }
    auto lexer = cmajor::sbin::assembly::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), asmFilePath);
    lexer.SetFile(fileIndex);
    soul::lexer::KeywordMap<char32_t>* keywordMap = lexer.GetKeywordMap();
    keywordMap->SetCaseInsensitive();
    using LexerType = decltype(lexer);
    std::unique_ptr<AsmFileNode> asmFile = cmajor::sbin::assembly::parser::AsmParser<LexerType>::Parse(lexer);
    asmFile->SetFileIndex(fileIndex);
    fileMap.AddFileContent(fileIndex, std::move(content), lexer.GetLineStartIndeces());
    return asmFile;
}

} // namespace cmajor::sbin::assembly
