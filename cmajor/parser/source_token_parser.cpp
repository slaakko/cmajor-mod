// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.source_token_parser;

import cmajor.token.lexer;
import cmajor.token;
import util;

namespace cmajor::parser {

using namespace cmajor::token;

void ParseSourceLine(const std::string& sourceLine, cmajor::ast::SourceTokenFormatter* formatter, bool& inBlockComment)
{
    std::u32string line(util::ToUtf32(sourceLine));
    line.append(U"\n");
    auto lexer = cmajor::token::lexer::MakeLexer(line.c_str(), line.c_str() + line.length(), "");
    lexer.SetSeparatorChar('\n');
    auto vars = static_cast<cmajor::token::lexer::CmajorTokenLexer_Variables*>(lexer.GetVariables());
    vars->inBlockComment = inBlockComment;
    ++lexer;
    int i = 0;
    formatter->BeginFormat();
    while (*lexer != soul::lexer::END_TOKEN)
    {
        switch (*lexer)
        {
            case WS: { soul::lexer::Token token = lexer.GetToken(i); formatter->Spaces(util::ToUtf8(token.match.ToString())); break; }
            case LINECOMMENT: { soul::lexer::Token token = lexer.GetToken(i); formatter->Comment(util::ToUtf8(token.match.ToString())); return; }
            case BLOCKCOMMENT:
            {
                soul::lexer::Token token = lexer.GetToken(i);
                formatter->Comment(util::ToUtf8(token.match.ToString()));
                if (!util::EndsWith(util::ToUtf8(token.match.ToString()), "*/"))
                {
                    inBlockComment = true;
                    return;
                }
                break;
            }
            case BLOCKCOMMENTLINE:
            {
                soul::lexer::Token token = lexer.GetToken(i);
                formatter->Comment(util::ToUtf8(token.match.ToString()));
                if (token.match.ToString().find(U"*/") != std::u32string::npos)
                {
                    inBlockComment = false;
                }
                return;
            }
            case KEYWORD: { soul::lexer::Token token = lexer.GetToken(i); formatter->Keyword(util::ToUtf8(token.match.ToString())); break; }
            case ID: { soul::lexer::Token token = lexer.GetToken(i); formatter->Identifier(util::ToUtf8(token.match.ToString())); break; }
            case CHARLIT: { soul::lexer::Token token = lexer.GetToken(i); formatter->Char(util::ToUtf8(token.match.ToString())); break; }
            case STRINGLIT: { soul::lexer::Token token = lexer.GetToken(i); formatter->String(util::ToUtf8(token.match.ToString())); break; }
            case NUMBER: { soul::lexer::Token token = lexer.GetToken(i); formatter->Number(util::ToUtf8(token.match.ToString())); break; }
            case OTHER: { soul::lexer::Token token = lexer.GetToken(i); formatter->Other(util::ToUtf8(token.match.ToString())); break; }
        }
        ++lexer;
        ++i;
    }
    formatter->EndFormat();
}

} // namespace cmajor::parser
