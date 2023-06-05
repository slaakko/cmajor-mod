// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.rex.match;

import util;
import soul.rex.lexer;
import soul.rex.parser;
import soul.rex.algorithm;
import soul.rex.spg.rules;

namespace soul::rex {

std::u32string MakeRegularExpressionPatternFromFilePattern(const std::u32string& filePattern)
{
    std::u32string pattern;
    for (char32_t c : filePattern)
    {
        switch (c)
        {
        case '.': pattern.append(U"\\."); break;
        case '*': pattern.append(U".*"); break;
        case '?': pattern.append(U"."); break;
        default: pattern.append(1, c); break;
        }
    }
    return pattern;
}

bool FilePatternMatch(const std::u32string& filePath, const std::u32string& filePattern)
{
    return PatternMatch(filePath, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

bool FilePatternMatch(const std::string& filePath, const std::string& filePattern)
{
    return FilePatternMatch(util::ToUtf32(filePath), util::ToUtf32(filePattern));
}

bool PatternMatch(const std::u32string& str, const std::u32string& regularExpressionPattern)
{
    soul::rex::context::Context context;
    soul::rex::nfa::Nfa nfa = CompileRegularExpressionPattern(context, regularExpressionPattern);
    return PatternMatch(str, nfa);
}

bool PatternMatch(const std::string& str, const std::string& regularExpressionPattern)
{
    return PatternMatch(util::ToUtf32(str), util::ToUtf32(regularExpressionPattern));
}

bool PatternMatch(const std::string& str, soul::rex::nfa::Nfa& nfa)
{
    return PatternMatch(util::ToUtf32(str), nfa);
}

soul::rex::nfa::Nfa CompileRegularExpressionPattern(soul::rex::context::Context& context, const std::u32string& regularExpressionPattern)
{
    auto lexer = soul::rex::lexer::MakeLexer(regularExpressionPattern.c_str(), regularExpressionPattern.c_str() + regularExpressionPattern.length(), "");
    using LexerType = decltype(lexer);
    lexer.SetRuleNameMapPtr(soul::rex::spg::rules::GetRuleNameMapPtr());
    return soul::rex::parser::RexParser<LexerType>::Parse(lexer, &context);
}

soul::rex::nfa::Nfa CompileRegularExpressionPattern(soul::rex::context::Context& context, const std::string& regularExpressionPattern)
{
    return CompileRegularExpressionPattern(context, util::ToUtf32(regularExpressionPattern));
}

soul::rex::nfa::Nfa CompileFilePattern(soul::rex::context::Context& context, const std::u32string& filePattern)
{
    return CompileRegularExpressionPattern(context, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

bool PatternMatch(const std::u32string& str, soul::rex::nfa::Nfa& nfa)
{
    return soul::rex::algorithm::Match(nfa, str);
}

soul::rex::nfa::Nfa CompileFilePattern(soul::rex::context::Context& context, const std::string& filePattern)
{
    return CompileFilePattern(context, util::ToUtf32(filePattern));
}

bool FilePatternMatch(const std::string& lexerModuleFileName, const std::u32string& filePath, const std::u32string& filePattern)
{
    return PatternMatch(lexerModuleFileName, filePath, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

bool FilePatternMatch(const std::string& lexerModuleFileName, const std::string& filePath, const std::string& filePattern)
{
    return FilePatternMatch(lexerModuleFileName, util::ToUtf32(filePath), util::ToUtf32(filePattern));
}

bool PatternMatch(const std::string& lexerModuleFileName, const std::u32string& str, const std::u32string& regularExpressionPattern)
{
    soul::rex::context::Context context;
    soul::rex::nfa::Nfa nfa = CompileRegularExpressionPattern(lexerModuleFileName, context, regularExpressionPattern);
    return PatternMatch(str, nfa);
}

bool PatternMatch(const std::string& lexerModuleFileName, const std::string& str, const std::string& regularExpressionPattern)
{
    return PatternMatch(lexerModuleFileName, util::ToUtf32(str), util::ToUtf32(regularExpressionPattern));
}

soul::rex::nfa::Nfa CompileRegularExpressionPattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::u32string& regularExpressionPattern)
{
    auto lexer = soul::rex::lexer::MakeLexer(lexerModuleFileName, regularExpressionPattern.c_str(), regularExpressionPattern.c_str() + regularExpressionPattern.length(), "");
    using LexerType = decltype(lexer);
    lexer.SetRuleNameMapPtr(soul::rex::spg::rules::GetRuleNameMapPtr());
    return soul::rex::parser::RexParser<LexerType>::Parse(lexer, &context);
}

soul::rex::nfa::Nfa CompileRegularExpressionPattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::string& regularExpressionPattern)
{
    return CompileRegularExpressionPattern(lexerModuleFileName, context, util::ToUtf32(regularExpressionPattern));
}

soul::rex::nfa::Nfa CompileFilePattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::u32string& filePattern)
{
    return CompileRegularExpressionPattern(lexerModuleFileName, context, MakeRegularExpressionPatternFromFilePattern(filePattern));
}

soul::rex::nfa::Nfa CompileFilePattern(const std::string& lexerModuleFileName, soul::rex::context::Context& context, const std::string& filePattern)
{
    return CompileFilePattern(lexerModuleFileName, context, util::ToUtf32(filePattern));
}

} // namespace soul::rex
