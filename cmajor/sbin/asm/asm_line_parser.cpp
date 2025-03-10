// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.asm_line_parser;

import cmajor.sbin.assembly.lexer;
import cmajor.sbin.assembly.instruction.line.parser;
import soul.lexer;
import util;

namespace cmajor::sbin::assembly {

std::unique_ptr<Node> ParseAsmLine(const std::string& asmLine)
{
    std::u32string line(util::ToUtf32(asmLine));
    line.append(1, '\n');
    auto lexer = cmajor::sbin::assembly::lexer::MakeLexer(line.c_str(), line.c_str() + line.length(), "");
    soul::lexer::KeywordMap<char32_t>* keywordMap = lexer.GetKeywordMap();
    keywordMap->SetCaseInsensitive();
    using LexerType = decltype(lexer);
    std::unique_ptr<Node> node = cmajor::sbin::assembly::instruction::line::parser::AsmInstructionLineParser<LexerType>::Parse(lexer);
    return node;
}

} // namespace cmajor::sbin::assembly
